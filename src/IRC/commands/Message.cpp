#include "../../Network/server.hpp"
#include "../../Network/client.hpp"
#include "../channel.hpp"


void send_private_message(Client *receiver, std::string message)
{
    send(receiver->getFd(), message.c_str(), message.size(), 0);
}


#include "../../Network/server.hpp"
#include "../../Network/client.hpp"
#include "../channel.hpp"

void Server::send_message(std::vector<std::string> s_command, int fd)
{
    Client *sender = getClientbyFD(fd);
    std::string target = s_command[1];
    std::string text = s_command[2];

    if (target.empty())
    {
        sendNumericReply(fd, "411", ":No recipient given (PRIVMSG)");
        return;
    }
    if (text.empty())
    {
        sendNumericReply(fd, "412", ":No text to send");
        return;
    }

    std::string fullMessage = ":" + sender->getPrefix() + " PRIVMSG " + target + " :" + text;

    if (target[0] == '#')
    {
        std::map<std::string, Channel*>::iterator it = _channels.find(target);
        if (it == _channels.end())
        {
            sendNumericReply(fd, "403", target + " :No such channel");
            return;
        }
        Channel *channel = it->second;
        if (!channel->hasUser(sender))
        {
            sendNumericReply(fd, "404", target + " :Cannot send to channel");
            return;
        }

        broadcastToChannel(channel, fullMessage, fd);
    }
    else
    {
        Client *receiver = getClientbyNick(target);
        if (!receiver)
        {
            sendNumericReply(fd, "401", target + " :No such nick/channel");
            return;
        }

        sendReply(receiver->getFd(), fullMessage);
    }
}