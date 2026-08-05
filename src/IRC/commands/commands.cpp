/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julian <julian@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/04 17:27:18 by fhanuise          #+#    #+#             */
/*   Updated: 2026/08/05 15:11:06 by julian           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Network/server.hpp"
#include "../../Network/client.hpp"
#include "../channel.hpp"

std::vector<std::string> parseIRC(std::string command)
{
    std::vector<std::string> args;
    std::string current;

    for (size_t i = 0; i < command.size(); i++)
    {
        if (command[i] == ' ')
        {
            if (!current.empty())
            {
                args.push_back(current);
                current.clear();
            }
        }
        else if (command[i] == ':' && current.empty())
        {
            args.push_back(command.substr(i + 1));
            break;
        }
        else
        {
            current += command[i];
        }
    }

    if (!current.empty())
        args.push_back(current);

    return args;
}

void Server::execute_irc_command(std::string &buffer, size_t pos, int fd)
{
    std::string command = buffer.substr(0, pos);
    // std::cout << "ca c'est la commande "<< command << std::endl;
    buffer.erase(0, pos + 2);

    std::vector<std::string> s_command = parseIRC(command);
    std::cout <<"size of command ==" << s_command.size() << std::endl;

    if (s_command.empty())
        return;

    if (s_command[0] == "PING")
        handlePing(s_command, fd);
    else if (s_command.size() == 2 && s_command[0] == "JOIN")
        channel_joined(s_command[1], fd);
    else if (s_command.size() == 3 && s_command[0] == "JOIN")
        channel_joined(s_command[1], fd, s_command[2]);
    else if (s_command.size() == 3 && s_command[0] == "PRIVMSG")
        send_message(s_command, fd);
    else if (s_command[0] == "TOPIC")
        handleTopic(s_command, fd);
    else if (s_command[0] == "INVITE")
        handleInvite(s_command, fd);
    else if (s_command[0] == "MODE")
        handleMode(s_command, fd);
    else if (s_command[0] == "KICK")
        handleKick(s_command, fd);
    else if (s_command[0] == "PART")
        handlePart(s_command, fd);
    else if (s_command[0] == "NICK")
        handleNick(s_command, fd);

}
