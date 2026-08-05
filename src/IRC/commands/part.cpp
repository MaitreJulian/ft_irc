/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julian <julian@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/04 17:27:47 by fhanuise          #+#    #+#             */
/*   Updated: 2026/08/05 16:08:02 by julian           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Network/server.hpp"
#include "../../Network/client.hpp"
#include "../channel.hpp"
#include <sstream>

static std::vector<std::string> splitByComma(const std::string &s)
{
    std::vector<std::string> result;
    std::istringstream iss(s);
    std::string token;

    while (std::getline(iss, token, ','))
        if (!token.empty())
            result.push_back(token);
    return result;
}

void Server::handlePart(std::vector<std::string> s_command, int fd)
{
    Client *client = getClientbyFD(fd);

    if (s_command.size() < 2)
    {
        sendNumericReply(fd, "461", "PART :Not enough parameters");
        return;
    }

    std::string reason = (s_command.size() >= 3) ? s_command[2] : client->getNickname();
    std::vector<std::string> channel_names = splitByComma(s_command[1]);

    for (size_t i = 0; i < channel_names.size(); i++)
    {
        std::string channel_name = channel_names[i];
        std::map<std::string, Channel*>::iterator it = _channels.find(channel_name);

        if (it == _channels.end())
        {
            sendNumericReply(fd, "403", channel_name + " :No such channel");
            continue;
        }

        Channel *channel = it->second;

        if (!channel->hasUser(client))
        {
            sendNumericReply(fd, "442", channel_name + " :You're not on that channel");
            continue;
        }

        std::string partMsg = ":" + client->getPrefix() + " PART " + channel_name + " :" + reason;
        broadcastToChannel(channel, partMsg);
        channel->removeUser(client);
        client->leave(channel);
        if (channel->getOperators().empty())
        {
            if (!channel->getUsers().empty())
            {
                std::set<Client*>::iterator uit = channel->getUsers().begin();
                std::set<Client*>::iterator uite = channel->getUserList().end();
                channel->addOperator(*uit);
                std::string names;
                for (; uit != uite; ++uit)
                {
                    if (channel->isOperator(*uit))
                        names += "@";
                    names += (*uit)->getNickname() + " ";
                }
                sendNumericReply(fd, "353", "= " + channel_name + " :" + names);
                sendNumericReply(fd, "366", channel_name + " :End of /NAMES list");
            }
        }
    }
}