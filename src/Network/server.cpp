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

Client* Server::getClientbyFD(int fd)
{
    return _clients[fd];
}
Client* Server::getClientbyNick(std::string nickname)
{

    for (std::map<int, Client*>::iterator it = _clients.begin();it != _clients.end();++it)
    {
        Client* client = it->second;

        if (client->getNickname() == nickname)
            return client;
    }
    return NULL;
}

void Server::execute_IRC_command(std::string command, int fd)
{
    std::vector<std::string> tokens = split_command(command);
    if (token.empty())
        return;
    std::string cmd = tokens[0];
        if (cmd == "JOIN")
        handleJoin(fd, tokens);
    else if (cmd == "KICK")
        handleKick(fd, tokens);
    else if (cmd == "INVITE")
        handleInvite(fd, tokens);
    else if (cmd == "TOPIC")
        handleTopic(fd, tokens);
    else if (cmd == "MODE")
        handleMode(fd, tokens);
    else
    {
        std::string err = "ERROR :UNKNOWN COMMAND\r\n";
        send(fd, err.c_str(), err.size(), 0);
    }
}

