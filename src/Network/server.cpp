/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fhanuise <fhanuise@student.42belgium.be    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/04 17:28:36 by fhanuise          #+#    #+#             */
/*   Updated: 2026/08/04 17:28:37 by fhanuise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "server.hpp"
#include "client.hpp"

Server::Server(int port, const std::string& password)
{
    _port = port;
    _password = password;
    _serverFd = -1;
}

Server::~Server()
{
    for (size_t i = 0; i < _fds.size(); i++)
    {
        close(_fds[i].fd);
    }

    for (std::map<int, Client*>::iterator it = _clients.begin();
         it != _clients.end();
         ++it)
    {
        delete it->second;
    }
}

std::map<std::string, Channel*> Server::getChannels()
{
    return _channels;
}

Client* Server::getClientbyFD(int fd)
{
    std::map<int, Client*>::iterator it = _clients.find(fd);
    if (it == _clients.end())
        return NULL;
    return it->second;
}
Client* Server::getClientbyNick(std::string nickname)
{

    for (std::map<int, Client*>::iterator it = _clients.begin();it != _clients.end();++it)
    {
        Client* client = it->second;

        if (client->getNickname() == nickname)
            return client;
    }

    return NULL;
}