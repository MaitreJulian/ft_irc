#include "server.hpp"
#include "client.hpp"

void Server::acceptNewClient()
{
    std::string instructions;

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
    instructions = "Connection succeed\n";
    send(clientFd, instructions.c_str(), instructions.size(), 0);
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