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

        std::string _topic;

    public:

        Channel(const std::string& name);

        void addUser(Client* client);
        void removeUser(Client* client);
};

#endif