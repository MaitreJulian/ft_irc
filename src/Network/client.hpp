#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>

class Client
{
    private:

        int _fd;

        std::string _nickname;
        std::string _username;

        bool _authenticated;

        std::string _buffer;

    public:

        Client(int fd);

        int getFd() const;

        std::string& getBuffer();

        void setNickname(const std::string& nick);
        void setUsername(const std::string& user);

        bool isAuthenticated() const;
};

#endif