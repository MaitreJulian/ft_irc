#include "../../Network/server.hpp"
#include "../../Network/client.hpp"
#include "../channel.hpp"
#include <cstdlib>

void Server::handleMode(std::vector<std::string> s_command, int fd)
{
    Client *client = getClientbyFD(fd);

    if (s_command.size() < 2)
    {
        sendNumericReply(fd, "461", "MODE :Not enough parameters");
        return;
    }

    std::string channel_name = s_command[1];
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

    
    if (s_command.size() == 2)
    {
        std::string modes = "+";
        if (channel->isInviteOnly()) modes += "i";
        if (channel->isTopicRestricted()) modes += "t";
        if (!channel->getPassword().empty()) modes += "k";
        if (channel->getUserLimit() > 0) modes += "l";
        sendNumericReply(fd, "324", channel_name + " " + modes);
        return;
    }

    if (!channel->isOperator(client))
    {
        sendNumericReply(fd, "482", channel_name + " :You're not channel operator");
        return;
    }

    std::string modeString = s_command[2];
    size_t paramIndex = 3;
    bool adding = true;
    std::string appliedModes;
    std::string appliedParams;

    for (size_t i = 0; i < modeString.size(); i++)
    {
        char c = modeString[i];

        if (c == '+') { adding = true; continue; }
        if (c == '-') { adding = false; continue; }

        if (c == 'i')
        {
            channel->setInviteOnly(adding);
            appliedModes += adding ? "+i" : "-i";
        }
        else if (c == 't')
        {
            channel->setTopicRestricted(adding);
            appliedModes += adding ? "+t" : "-t";
        }
        else if (c == 'k')
        {
            if (adding)
            {
                if (paramIndex >= s_command.size())
                {
                    sendNumericReply(fd, "461", "MODE :Not enough parameters");
                    continue;
                }
                channel->setPassword(s_command[paramIndex]);
                appliedModes += "+k";
                appliedParams += " " + s_command[paramIndex];
                paramIndex++;
            }
            else
            {
                channel->setPassword("");
                appliedModes += "-k";
            }
        }
        else if (c == 'o')
        {
            if (paramIndex >= s_command.size())
            {
                sendNumericReply(fd, "461", "MODE :Not enough parameters");
                continue;
            }
            Client *target = getClientbyNick(s_command[paramIndex]);
            if (!target || !channel->hasUser(target))
            {
                sendNumericReply(fd, "441", s_command[paramIndex] + " " + channel_name + " :They aren't on that channel");
                paramIndex++;
                continue;
            }
            if (adding)
                channel->addOperator(target);
            else
                channel->removeOperator(target);
            appliedModes += adding ? "+o" : "-o";
            appliedParams += " " + s_command[paramIndex];
            paramIndex++;
        }
        else if (c == 'l')
        {
            if (adding)
            {
                if (paramIndex >= s_command.size())
                {
                    sendNumericReply(fd, "461", "MODE :Not enough parameters");
                    continue;
                }
                int limit = std::atoi(s_command[paramIndex].c_str());
                channel->setUserLimit(limit);
                appliedModes += "+l";
                appliedParams += " " + s_command[paramIndex];
                paramIndex++;
            }
            else
            {
                channel->setUserLimit(0);
                appliedModes += "-l";
            }
        }
        else
        {
            sendNumericReply(fd, "472", std::string(1, c) + " :is unknown mode char to me");
        }
    }

    if (!appliedModes.empty())
    {
        std::string msg = ":" + client->getPrefix() + " MODE " + channel_name + " " + appliedModes + appliedParams;
        broadcastToChannel(channel, msg);
    }
}