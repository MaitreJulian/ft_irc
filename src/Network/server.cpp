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

std::map<std::string, Channel*> Server::getChannels()
{
    return _channels;
}

