#include "../../Network/server.hpp"
#include "../../Network/client.hpp"
#include "../channel.hpp"


void send_private_message(Client *receiver, std::string message)
{
    send(receiver->getFd(), message.c_str(), message.size(), 0);
}


void Server::send_message(std::vector<std::string> s_command)
{
    std::string target = s_command[1];
    if (target[0] == '#')
    {
        if (_channels[target])
        {
            std::set<Client*>::iterator it = _channels[target]->getUserList().begin();
            std::set<Client*>::iterator it_end = _channels[target]->getUserList().end();

            while (it != it_end)
            {
                std::string message =  s_command[2];
                send((*it)->getFd(), message.c_str(), message.size(), 0);
                it++;
            }
        }
        else
            std::cerr << "Channel not found"<<std::endl;
    }
    else 
    {
        Client *receiver = getClientbyNick(s_command[1]);
        send_private_message(receiver, s_command[2]);
    }
}