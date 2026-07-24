#include "../../Network/server.hpp"
#include "../../Network/client.hpp"
#include "../channel.hpp"

void Server::handleInvite(std::vector<std::string> s_command, int fd)
{
    Client *client = getClientbyFD(fd);

    if (s_command.size() < 3)
    {
        sendNumericReply(fd, "461", "INVITE :Not enough parameters");
        return;
    }

    std::string target_nick   = s_command[1];
    std::string channel_name  = s_command[2];

    std::map<std::string, Channel*>::iterator it = _channels.find(channel_name);
    if (it == _channels.end())
    {
        sendNumericReply(fd, "403", channel_name + " :No such channel");
        return;
    }
    Channel *channel = it->second;

    if (!channel->hasUser(client))
    {
        sendNumericReply(fd, "442", channel_name + " :You're not on that channel");
        return;
    }

    if (channel->isInviteOnly() && !channel->isOperator(client))
    {
        sendNumericReply(fd, "482", channel_name + " :You're not channel operator");
        return;
    }

    Client *target = getClientbyNick(target_nick);
    if (!target)
    {
        sendNumericReply(fd, "401", target_nick + " :No such nick/channel");
        return;
    }

    if (channel->hasUser(target))
    {
        sendNumericReply(fd, "443", target_nick + " " + channel_name + " :is already on channel");
        return;
    }

    channel->addInvited(target);

    sendNumericReply(fd, "341", target_nick + " " + channel_name);

    std::string inviteMsg = ":" + client->getPrefix() + " INVITE " + target_nick + " :" + channel_name;
    sendReply(target->getFd(), inviteMsg);
}