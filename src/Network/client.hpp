#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include <iostream>
#include <set>

class Channel;

class Client
{
    private:

        int _fd;

        std::string _nickname;
        std::string _username;
        std::string _realname;
        bool _passwordOK;
        bool has_nick;
        bool has_user;

        bool is_operator;
        
        std::string _buffer;

        std::set<Channel*> _channels;

    public:

        Client(int fd);

        int getFd() const;

        std::string& getBuffer();
        
        std::string getNickname();
        std::string getUsername();

        void setNickname(const std::string& nick);
        void setUsername(const std::string& user);
        void setpass();
        

        bool isAuthenticated() const;

        void join(Channel *channel);
};

#endif