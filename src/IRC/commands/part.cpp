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

        // Diffuse à tout le monde AVANT de retirer, pour que l'émetteur voie aussi la confirmation
        broadcastToChannel(channel, partMsg);

        channel->removeUser(client);
        client->leave(channel);
    }
}