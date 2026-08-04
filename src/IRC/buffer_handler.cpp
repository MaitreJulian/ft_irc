#include "../Network/server.hpp"
#include "../Network/client.hpp"
#include "../IRC/channel.hpp"


void Server::leaveAllChannels(Client *client)
{
    std::set<Channel*> channels = client->getChannels();

    for (std::set<Channel*>::iterator it = channels.begin(); it != channels.end(); ++it)
    {
        Channel *channel = *it;

        std::string quitMsg = ":" + client->getPrefix() + " QUIT :Client disconnected";
        broadcastToChannel(channel, quitMsg, client->getFd());

        channel->removeUser(client);
    }
}

void print_buffer(std::string buffer)
{
    std::cout << "Buffer = [";

    for (size_t i = 0; i < buffer.size(); i++)
    {
        if (buffer[i] == '\r')
            std::cout << "\\r";
        else if (buffer[i] == '\n')
            std::cout << "\\n";
        else
            std::cout << buffer[i];
    }
std::cout << "]" << std::endl;
}
void Server::processClientbuffer(int fd)
{
    std::string& buffer = _clients[fd]->getBuffer();
    print_buffer(buffer);
    size_t pos;
    while ((pos = buffer.find("\r\n")) != std::string::npos)
    {


        if (!_clients[fd]->isAuthenticated())
        {
            // std::cout <<  "va dans autentificate" <<std::endl;
            if(Authentificate(buffer, pos, fd))
                continue;
            else
                std::cout << "Not totally authenticated" << std::endl;
        }
        else
            execute_irc_command(buffer, pos, fd);

    }
    std::cout << pos << std::endl;
}

void Server::receiveData(int fd)
{
    char buffer[512];

    std::cout << "Je suis dans reveive data" << std::endl;
        int bytes = recv(fd, buffer, sizeof(buffer), 0);

    if (bytes > 0)
    {
        _clients[fd]->getBuffer().append(buffer, bytes);
        processClientbuffer(fd);    
    }
    else if (bytes == 0)
        removeClient(fd);
    else
        if (errno != EAGAIN && errno != EWOULDBLOCK )
        removeClient(fd);
}

