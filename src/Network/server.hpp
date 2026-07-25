#ifndef SERVER_HPP
#define SERVER_HPP

#define SERVER_NAME "ircserv"

#include <vector>
#include <map>
#include <set>

#include <cstring>
#include <sstream>
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

#include <cerrno>
#include "../IRC/channel.hpp"

class Client;

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
        std::map<std::string, Channel*> getChannels();
        void initServer();
        void run();

        Client *getClientbyFD(int fd);
        Client* getClientbyNick(std::string username);

    private:


        //INIT Server
        void createSocket();
        void bindSocket();
        void listenSocket();

        //Server run
        void acceptNewClient();
        void receiveData(int fd);
        void removeClient(int fd);
        void leaveAllChannels(Client *client);
        void processClientbuffer(int fd);

        //Init Client
        int Authentificate(std::string &buffer, size_t pos, int fd);
        bool NicknameExist(const std::string &nickname);
        bool UsernameExist(const std::string& nickname);
        void handlePing(std::vector<std::string> s_command, int fd);

        void execute_irc_command(std::string &buffer, size_t pos, int fd);

        //Commands
        void channel_joined(const std::string &channel_name, int fd, const std::string &key = "");
        void send_message(std::vector<std::string> s_command, int fd);

        //Reply

    // Reply
        void sendReply(int fd, const std::string &message);
        void sendNumericReply(int fd, const std::string &code, const std::string &params);
        void broadcastToChannel(Channel *channel, const std::string &message, int excludeFd = -1);

        // Write robustness
        void flushClient(int fd);
        void enableWritePoll(int fd);
        void disableWritePoll(int fd);

        //Operators commands
        void handleTopic(std::vector<std::string> s_command, int fd);
        void handleInvite(std::vector<std::string> s_command, int fd);
        void handleMode(std::vector<std::string> s_command, int fd);
        void handleKick(std::vector<std::string> s_command, int fd);
        void handlePart(std::vector<std::string> s_command, int fd);
        void handleNick(std::vector<std::string> s_command, int fd);
        void handleUser(std::vector<std::string>s_command, int fd);
    };      

void send_instructions(int fd);

#endif