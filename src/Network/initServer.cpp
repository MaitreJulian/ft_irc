/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initServer.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julian <julian@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 14:16:56 by julian            #+#    #+#             */
/*   Updated: 2026/06/16 15:24:26 by julian           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

void Server::createSocket()
{
    _serverFd = socket(AF_INET, SOCK_STREAM, 0);
    if (_serverFd < 0)
        throw std::runtime_error("socket failed");
    if (fcntl(_serverFd, F_SETFL, O_NONBLOCK) == -1)
    {
        throw std::runtime_error("fcntl failed");
    }
    std::cout << "Server socket created : "
    << _serverFd
    << std::endl;
}

void Server::bindSocket()
{
    sockaddr_in addr;
    
    std::memset(&addr, 0, sizeof(addr));
    
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(_port);
    
    if (bind(_serverFd, (sockaddr*)&addr, sizeof(addr)) < 0)
        throw std::runtime_error("bind failed");
    else
    {
        std::cout << "Server socket bound to port "
        << _port
        << std::endl;
    }
}
    
void Server::listenSocket()
{
    if (listen(_serverFd, SOMAXCONN) < 0)
    throw std::runtime_error("listen failed");
    else
    {
        std::cout << "Server is listening"
        << std::endl;
    }
}
void Server::initServer()
{
    createSocket();
    bindSocket();
    listenSocket();

    pollfd pfd;

    pfd.fd = _serverFd;
    pfd.events = POLLIN;
    pfd.revents = 0;

    _fds.push_back(pfd);
}