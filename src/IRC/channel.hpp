/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fhanuise <hanuiseflorian@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/15 13:39:33 by fhanuise          #+#    #+#             */
/*   Updated: 2026/07/14 11:42:16 by fhanuise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>
#include <set>

class Client;

class Channel
{
    private:
        std::string         _name;
        std::string         _topic;
        std::string         _password;
        std::set<Client*>   _users;
        std::set<Client*>   _operators;
        bool                _inviteOnly;
        bool                _topicRestricted;
        int                 _userLimit;

    public:
        Channel(const std::string& name);
        const std::string&  getName() const;
        const std::string&  getTopic() const;
        const std::string&  getPassword() const;
        bool                isInviteOnly() const;
        int                 getUserLimit() const;
        size_t              getUserCount() const;
        bool                isOperator(Client* client) const;
        bool                hasUser(Client* client) const;
        void    addUser(Client* client);
        void    removeUser(Client* client);
        void    addOperator(Client* client);
        void    removeOperator(Client* client);
        void    setTopic(const std::string& topic);
        void    setPassword(const std::string& password);
        void    setInviteOnly(bool value);
        void    setTopicRestricted(bool value);
        void    setUserLimit(int limit);
};

#endif