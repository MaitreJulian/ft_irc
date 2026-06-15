#include "server.hpp"
#include "client.hpp"

Server::Server(int port, const std::string& password)
{
    _port = port;
    _password = password;
    _serverFd = -1;
}

Server::~Server()
{
    for (size_t i = 0; i < _fds.size(); i++)
    {
        close(_fds[i].fd);
    }

    for (std::map<int, Client*>::iterator it = _clients.begin();
         it != _clients.end();
         ++it)
    {
        delete it->second;
    }
}


void Server::createSocket()
{
    _serverFd = socket(AF_INET, SOCK_STREAM, 0);

    if (_serverFd < 0)
        throw std::runtime_error("socket failed");
    std::cout << "Server socket created : "
    << _serverFd
    << std::endl;
}

void Server::bindSocket()
{
    sockaddr_in addr;
    
    std::memset(&addr, 0, sizeof(addr));
    
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(_port);
    
    if (bind(_serverFd, (sockaddr*)&addr, sizeof(addr)) < 0)
        throw std::runtime_error("bind failed");
    else
    {
        std::cout << "Server socket bound to port "
        << _port
        << std::endl;
    }
}
    
void Server::listenSocket()
{
    if (listen(_serverFd, SOMAXCONN) < 0)
    throw std::runtime_error("listen failed");
    else
    {
        std::cout << "Server is listening"
        << std::endl;
    }
}
void Server::initServer()
{
    createSocket();
    bindSocket();
    listenSocket();

    pollfd pfd;

    pfd.fd = _serverFd;
    pfd.events = POLLIN;
    pfd.revents = 0;

    _fds.push_back(pfd);
}
    
void Server::run()
{
    while (true)
    {
        poll(&_fds[0], _fds.size(), -1);

        for (size_t i = 0; i < _fds.size(); i++)
        {
            if (!(_fds[i].revents & POLLIN))
                continue;

            if (_fds[i].fd == _serverFd)
            {
                acceptNewClient();
            }
            else
            {
                receiveData(_fds[i].fd);
            }
        }
    }
}

void Server::acceptNewClient()
{
    int clientFd = accept(_serverFd, NULL, NULL);

    if (clientFd < 0)
        return;

    Client* client = new Client(clientFd);

    _clients[clientFd] = client;

    pollfd pfd;

    pfd.fd = clientFd;
    pfd.events = POLLIN;
    pfd.revents = 0;

    _fds.push_back(pfd);

    std::cout << "New client : "
              << clientFd
              << std::endl;
}

void Server::removeClient(int fd)
{
    std::map<int, Client*>::iterator it = _clients.find(fd);

    if (it != _clients.end())
    {
        delete it->second;
        _clients.erase(it);
    }

    for (std::vector<pollfd>::iterator it = _fds.begin();
         it != _fds.end();
         ++it)
    {
        if (it->fd == fd)
        {
            _fds.erase(it);
            break;
        }
    }

    close(fd);

    std::cout << "Client "
              << fd
              << " disconnected"
              << std::endl;
}

void Server::receiveData(int fd)
{
    char buffer[512];

    int bytes = recv(fd,
                     buffer,
                     sizeof(buffer) - 1,
                     0);

    if (bytes <= 0)
    {
        removeClient(fd);
        return;
    }

    buffer[bytes] = '\0';

    std::cout
        << "Client "
        << fd
        << " : "
        << buffer
        << std::endl;
}

