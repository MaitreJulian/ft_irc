#include "server.hpp"
#include "client.hpp"
#include <csignal>

extern sig_atomic_t g_signal;   
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
    sendReply(clientFd, "Connection succeed");
}


void Server::removeClient(int fd)
{
    std::map<int, Client*>::iterator it = _clients.find(fd);

    if (it != _clients.end())
    {
        Client *client = it->second;

        leaveAllChannels(client);

        delete client;
        _clients.erase(it);
    }

    for (std::vector<pollfd>::iterator pit = _fds.begin();
         pit != _fds.end();
         ++pit)
    {
        if (pit->fd == fd)
        {
            _fds.erase(pit);
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
    while (!g_signal)
    {
        int ret = poll(&_fds[0], _fds.size(), -1);

        if (ret < 0)
        {
            if (errno == EINTR)
                break;      
        }

        for (size_t i = 0; i < _fds.size(); i++)
        {
            int fd = _fds[i].fd;
            short revents = _fds[i].revents;
            bool removed = false;

            if (revents & (POLLHUP | POLLERR))
            {
                removeClient(fd);
                removed = true;
            }
            else
            {
                if (revents & POLLOUT)
                    flushClient(fd);
                if (getClientbyFD(fd) == NULL && fd != _serverFd)
                    removed = true;
                if (!removed && (revents & POLLIN))
                {
                    if (fd == _serverFd)
                        acceptNewClient();
                    else
                        receiveData(fd);
                }
                if (!removed && getClientbyFD(fd) == NULL && fd != _serverFd)
                    removed = true;
            }
            if (removed)
                i--;
        }
    }
    for (std::map<int, Client*>::iterator it = _clients.begin();
         it != _clients.end(); ++it)
    {
        close(it->first);
        delete it->second;
    }
    _clients.clear();

    for (std::map<std::string, Channel*>::iterator it = _channels.begin();
         it != _channels.end(); ++it)
    {
        delete it->second;
    }
    _channels.clear();

    close(_serverFd);

    std::cout << "Server shutting down (signal received)" << std::endl;
}