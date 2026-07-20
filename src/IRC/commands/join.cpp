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

    std::map<std::string, Channel*>::iterator it = _channels.find(channel_name);

    if (it == _channels.end())
    {
        std::cout << "New channel : " << channel_name << "added to server" << std::endl;
        Channel *new_channel = new Channel(channel_name);
        _channels[channel_name] = new_channel;
        new_channel->addUser(_clients[fd]);
        new_channel->addOperator(_clients[fd]);
        _clients[fd]->join(new_channel);
    }
    else
    {
        std::cout << "Client : " << _clients[fd]->getNickname() << "added to" << channel_name << std::endl;
        it->second->addUser(_clients[fd]);
        _clients[fd]->join(it->second);
    }
}