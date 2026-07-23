#include "../../Network/server.hpp"
#include "../../Network/client.hpp"
#include "../channel.hpp"
#include "../operator.hpp"


void Client::join(Channel *channel)
{
    _channels.insert(channel);
}

void Server::channel_joined(const std::string &channel_name, int fd)
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
        channel->addUser(client);
        client->join(channel);
    }

    // 1. Confirmation du JOIN, envoyée à tout le monde dans le channel (client inclus)
    std::string joinMsg = ":" + client->getPrefix() + " JOIN :" + channel_name;
    broadcastToChannel(channel, joinMsg);

    // 2. Topic si défini (RPL_TOPIC = 332)
    if (!channel->getTopic().empty())
        sendNumericReply(fd, "332", channel_name + " :" + channel->getTopic());

    // 3. Liste des utilisateurs du channel (RPL_NAMREPLY = 353 / RPL_ENDOFNAMES = 366)
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