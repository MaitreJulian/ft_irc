/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fhanuise <fhanuise@student.42belgium.be    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/04 17:27:57 by fhanuise          #+#    #+#             */
/*   Updated: 2026/08/04 17:27:58 by fhanuise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Network/server.hpp"
#include "../../Network/client.hpp"
#include "../channel.hpp"

void Server::handleTopic(std::vector<std::string> s_command, int fd)
{
    Client *client = getClientbyFD(fd);

    if (s_command.size() < 2)
    {
        sendNumericReply(fd, "461", "TOPIC :Not enough parameters");
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
        if (channel->getTopic().empty())
            sendNumericReply(fd, "331", channel_name + " :No topic is set");
        else
            sendNumericReply(fd, "332", channel_name + " :" + channel->getTopic());
        return;
    }

    if (channel->isTopicRestricted() && !channel->isOperator(client))
    {
        sendNumericReply(fd, "482", channel_name + " :You're not channel operator");
        return;
    }
    channel->setTopic(s_command[2]);

    std::string msg = ":" + client->getPrefix() + " TOPIC " + channel_name + " :" + s_command[2];
    broadcastToChannel(channel, msg);
}