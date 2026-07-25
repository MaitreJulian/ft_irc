#include "../../Network/server.hpp"
#include "../../Network/client.hpp"
#include "../channel.hpp"

void Server::handleNick(std::vector<std::string> s_command, int fd)
{
    Client *client = getClientbyFD(fd);

    if (s_command.size() < 2)
    {
        sendNumericReply(fd, "431", ":No nickname given");
        return;
    }

    std::string new_nick = s_command[1];

    if (NicknameExist(new_nick))
    {
        sendNumericReply(fd, "433", new_nick + " :Nickname is already in use");
        return;
    }

    std::string oldPrefix = client->getPrefix();
    std::string nickChangeMsg = ":" + oldPrefix + " NICK :" + new_nick;

    client->setNickname(new_nick);

    // Confirmation au client lui-même (il ne fait partie d'aucun channel dans certains cas)
    sendReply(fd, nickChangeMsg);

    // Broadcast à tous les channels où il est présent (sans le redoubler s'il est dans plusieurs)
    std::set<Channel*> &channels = client->getChannels();
    std::set<Client*> alreadyNotified;
    alreadyNotified.insert(client);

    for (std::set<Channel*>::iterator it = channels.begin(); it != channels.end(); ++it)
    {
        Channel *channel = *it;
        std::set<Client*>::iterator uit = channel->getUserList().begin();
        std::set<Client*>::iterator uite = channel->getUserList().end();

        for (; uit != uite; ++uit)
        {
            if (alreadyNotified.count(*uit) == 0)
            {
                sendReply((*uit)->getFd(), nickChangeMsg);
                alreadyNotified.insert(*uit);
            }
        }
    }
}