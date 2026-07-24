#include "../../Network/server.hpp"
#include "../../Network/client.hpp"
#include "../channel.hpp"


void Server::sendReply(int fd, const std::string &message)
{
    std::string full = message + "\r\n";
    send(fd, full.c_str(), full.size(), 0);
}


void Server::sendNumericReply(int fd, const std::string &code, const std::string &params)
{
    Client *client = getClientbyFD(fd);
    std::string nick = (client && !client->getNickname().empty()) ? client->getNickname() : "*";

    std::string message = ":" + std::string(SERVER_NAME) + " " + code + " " + nick + " " + params;
    sendReply(fd, message);
}


void Server::broadcastToChannel(Channel *channel, const std::string &message, int excludeFd)
{
    std::set<Client*>::iterator it = channel->getUserList().begin();
    std::set<Client*>::iterator ite = channel->getUserList().end();

    for (; it != ite; ++it)
    {
        if ((*it)->getFd() != excludeFd)
            sendReply((*it)->getFd(), message);
    }
}