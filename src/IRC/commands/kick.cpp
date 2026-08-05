/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julian <julian@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/04 17:27:34 by fhanuise          #+#    #+#             */
/*   Updated: 2026/08/05 15:05:09 by julian           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Network/server.hpp"
#include "../../Network/client.hpp"
#include "../channel.hpp"

void Server::handleKick(std::vector<std::string> s_command, int fd)
{
    Client *client = getClientbyFD(fd);

    if (s_command.size() < 3)
    {
        sendNumericReply(fd, "461", "KICK :Not enough parameters");
        return;
    }

    std::string channel_name = s_command[1];
    std::string target_nick  = s_command[2];
    std::string reason = (s_command.size() >= 4) ? s_command[3] : target_nick;

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

    if (!channel->isOperator(client))
    {
        sendNumericReply(fd, "482", channel_name + " :You're not channel operator");
        return;
    }

    Client *target = getClientbyNick(target_nick);
    if (!target || !channel->hasUser(target))
    {
        sendNumericReply(fd, "441", target_nick + " " + channel_name + " :They aren't on that channel");
        return;
    }

    std::string kickMsg = ":" + client->getPrefix() + " KICK " + channel_name
                         + " " + target_nick + " :" + reason;

    
    broadcastToChannel(channel, kickMsg);

    channel->removeUser(target);
    target->leave(channel);
}