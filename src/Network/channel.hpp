#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>
#include <set>

class Client;

class Channel
{
    private:

        std::string _name;

        std::set<Client*> _users;
        std::set<Client*> _operators;
        std::string       _password;
        std::string       _topic;
        int               _userLimit;
        bool              _inviteOnly;
        bool              _topicrestricted;

    public:

        Channel(const std::string& name);

        void    addUser(Client* client);
        const   std::string&  getName() const;
        const   std::string&  getTopic() const;
        const   std::string&  getPassword() const;
        bool    isInviteOnly() const;
        int     getUserLimit() const;
        size_t  getUserCount() const;
        void    addOperator(Client* client);
        void    removeOperator(Client* client);
        bool    isOperator(Client* client) const;
        bool    hasUser(Client* client) const ;
        void    removeUser(Client* client);
        void    setTopic(const std::string& topic);
        void    setPassword(const std::string& password);
        void    setInviteOnly(bool value);
        void    setTopicRestricted(bool value);
        void    setUserLimit(int limit);
};

#endif