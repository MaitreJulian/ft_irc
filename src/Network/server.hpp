#ifndef SERVER_HPP
#define SERVER_HPP

#include <vector>
#include <map>

#include <cstring>
#include <cstdlib>
#include <string>
#include <stdexcept>
#include <iostream>

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#include <poll.h>
#include <arpa/inet.h>
#include <sys/socket.h>

class Client;
class Channel;

class Server
{
    private:

        int _serverFd;
        int _port;
        std::string _password;

        std::vector<pollfd> _fds;

        std::map<int, Client*> _clients;
        std::map<std::string, Channel*> _channels;

    public:

        Server(int port, const std::string& password);
        ~Server();
        void set_client(int fd, const std::string& buffer);
        void initServer();
        void run();

    private:

        void createSocket();
        void bindSocket();
        void listenSocket();

        void acceptNewClient();
        void receiveData(int fd);

        void removeClient(int fd);
};

void send_instructions(int fd);
void send_welcome_message(int fd);

#endif