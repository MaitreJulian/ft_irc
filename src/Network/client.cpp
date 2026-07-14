/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julian <julian@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/15 13:39:33 by julian            #+#    #+#             */
/*   Updated: 2026/07/15 01:06:59 by julian           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.hpp"

Client::Client(int fd)
{
    _fd = fd;
    has_nick= false;
    has_user= false;
}

int Client::getFd() const
{
    return _fd;
}

std::string& Client::getBuffer()
{
    return _buffer;
}

std::string Client::getnick()
{
    return _nickname; 
}
void Client::setNickname(const std::string& nick)
{
    _nickname = nick;
    std::cout << "Le surnon est maintenant : " << _nickname << std::endl;
    has_nick = true;
}

void Client::setUsername(const std::string& user)
{
    _username = user;
    std::cout << "Le username est maintenant : " << _username << std::endl;
    has_user = true;
}

bool Client::isAuthenticated() const
{
    return has_nick && has_user;
}



