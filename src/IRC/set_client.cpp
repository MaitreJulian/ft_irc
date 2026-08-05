/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_client.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fhanuise <fhanuise@student.42belgium.be    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/04 17:28:18 by fhanuise          #+#    #+#             */
/*   Updated: 2026/08/04 17:28:20 by fhanuise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../Network/server.hpp"
#include "../Network/client.hpp"

void Server::handlePing(std::vector<std::string> s_command, int fd)
{
    std::string token = (s_command.size() >= 2) ? s_command[1] : "";
    sendReply(fd, "PONG :" + token);
}

void ft_command_size(std::vector<std::string> command)
{
    if (command.size() == 0)
        return ;
    size_t size = command.size();
    size_t i = 0;
    while (i < size)
    {
        // std::cout << "command size : "<< size <<"[" << command[i] << "]";
        i++;
    }
    std::cout<<std::endl;
}


bool Server::UsernameExist(const std::string& username)
{
    // std::cout<<"Dans UsernameExist" <<std::endl;
    for (std::map<int, Client*>::iterator it = _clients.begin();
         it != _clients.end();
         ++it)
    {
        if (!it->second->getUsername().empty() &&
            it->second->getUsername() == username)
            return true;
    }
    return false;
}
bool Server::NicknameExist(const std::string& nickname)
{
    // std::cout << "dnas NicknameExist" << std::endl;
    for (std::map<int, Client*>::iterator it = _clients.begin();
         it != _clients.end();
         ++it)
    {
        if (!it->second->getNickname().empty() &&
            it->second->getNickname() == nickname)
            return true;
    }
    return false;
}

std::vector<std::string> split_command(std::string command)
{
    std::istringstream iss(command);
    std::vector<std::string> tokens;
    std::string token;

    while (iss >> token)
        tokens.push_back(token);
    return tokens;
}

int Server::Authentificate(std::string &buffer, size_t pos, int fd)
{
    // std::cout << "Dans Authentificate avec client fd : " << fd << std::endl;
    std::string command = buffer.substr(0, pos);
    // std::cout << "ca c'est la commande "<< command << std::endl;
    buffer.erase(0, pos + 2);

    std::string instructions;
    std::vector<std::string> s_command = split_command(command);
    ft_command_size(s_command);

    if (s_command.size() > 0 && s_command[0] == "CAP")
    {
        if (s_command.size() >= 2 && s_command[1] == "LS")
            sendReply(fd, "CAP * LS :");
    }
    else if (s_command.size() > 0 && s_command[0] == "PING")
    {
        handlePing(s_command, fd);
    }
    else if(s_command.size() == 2 && s_command[0] == "PASS")
    {
        if(_password == s_command[1])
            _clients[fd]->setpass();
        else
        {
            std::cerr << "WRONG PASSWORD" << std::endl;
            sendNumericReply(fd, "464", ":Password incorrect");
        }
    }
    else if(s_command.size() == 2 && s_command[0] == "NICK")
    {
        if (!NicknameExist(s_command[1]))
        {
            // std::cout << "dans command NICK" <<std::endl;
            _clients[fd]->setNickname(s_command[1]);
            
            sendReply(fd, "Nickname is now " + _clients[fd]->getNickname());
        }
        else
            sendNumericReply(fd, "433", s_command[1] + " :Nickname is already in use");
    }
    else if (s_command.size() == 5 && s_command[0] == "USER")
    {
        if (!UsernameExist(s_command[1]))
            _clients[fd]->setUsername(s_command[1]);
        else
            sendReply(fd, "Username exist already or is empty");
    }
    else
    {
        std::cerr << "Invalid command from client" << std::endl;
        sendReply(fd, "Set USER and NICK correctly\r\nNICK (nickname)\r\nUSER (user) 0 * :(real name)");
    }
    if (_clients[fd]->isAuthenticated())
    {
        sendNumericReply(fd, "001", ":Welcome to ft_irc " + _clients[fd]->getNickname() + "!" + _clients[fd]->getUsername() + "@localhost\r\n");
        return 1;
    }
    return 0;
}