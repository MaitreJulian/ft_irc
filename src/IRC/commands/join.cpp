/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julian <julian@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/04 17:27:30 by fhanuise          #+#    #+#             */
/*   Updated: 2026/08/05 15:58:26 by julian           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Network/server.hpp"
#include "../../Network/client.hpp"
#include "../channel.hpp"
#include "../operator.hpp"


void Server::channel_joined(const std::string &channel_name, int fd, const std::string &key)
{
    Client *client = _clients[fd];
    std::map<std::string, Channel*>::iterator it = _channels.find(channel_name);
    Channel *channel;

    if (it == _channels.end())
    {
        channel = new Channel(channel_name);
        _channels[channel_name] = channel;
        channel->addUser(client);
        channel->addOperator(client);
        client->join(channel);
    }
    else
    {
        channel = it->second;

        if (channel->isInviteOnly() && !channel->isInvited(client))
        {
            sendNumericReply(fd, "473", channel_name + " :Cannot join channel (+i)");
            return;
        }

        if (!channel->getPassword().empty() && channel->getPassword() != key)
        {
            sendNumericReply(fd, "475", channel_name + " :Cannot join channel (+k)");
            return;
        }

        if (channel->getUserLimit() > 0 &&
            channel->getUserCount() >= static_cast<size_t>(channel->getUserLimit()))
        {
            sendNumericReply(fd, "471", channel_name + " :Cannot join channel (+l)");
            return;
        }

        channel->addUser(client);
        client->join(channel);
    }

    std::string joinMsg = ":" + client->getPrefix() + " JOIN :" + channel_name;
    broadcastToChannel(channel, joinMsg);

    if (!channel->getTopic().empty())
        sendNumericReply(fd, "332", channel_name + " :" + channel->getTopic());

    std::string names;
    std::set<Client*>::iterator uit = channel->getUserList().begin();
    std::set<Client*>::iterator uite = channel->getUserList().end();
    for (; uit != uite; ++uit)
    {
        if (channel->isOperator(*uit))
            names += "@";
        names += (*uit)->getNickname() + " ";
    }
    sendNumericReply(fd, "353", "= " + channel_name + " :" + names);
    sendNumericReply(fd, "366", channel_name + " :End of /NAMES list");
}