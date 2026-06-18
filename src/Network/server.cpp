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


void Server::acceptNewClient()
{
    int clientFd = accept(_serverFd, NULL, NULL);

    if (clientFd < 0)
        return;
    if (fcntl(clientFd, F_SETFL, O_NONBLOCK) == -1)
    {
        throw std::runtime_error("fcntl failed");
    }

    Client* client = new Client(clientFd);

    _clients[clientFd] = client;

    pollfd pfd;

    pfd.fd = clientFd;
    pfd.events = POLLIN;
    pfd.revents = 0;

    _fds.push_back(pfd);

    std::cout << "New client : " << clientFd << std::endl;
    send_instructions(clientFd);
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

void Server::run()
{
    while (true)
    {
        int ret = poll(&_fds[0], _fds.size(), -1);

        for (size_t i = 0; i < _fds.size(); i++)
        {
            if (_fds[i].revents & POLLHUP)
            {
                removeClient(_fds[i].fd);
                continue;
            }

            if (_fds[i].revents & POLLERR)
            {
                removeClient(_fds[i].fd);
                continue;
            }

            if (_fds[i].revents & POLLIN)
            {
                if (_fds[i].fd == _serverFd)
                    acceptNewClient();
                else
                    receiveData(_fds[i].fd);
            }
        }
        std::cout << "Poll returned : "
        << ret
        << std::endl;
    }
}
