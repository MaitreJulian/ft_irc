#include "../Network/server.hpp"
#include "../Network/client.hpp"

void Server::set_client(int fd, const std::string& buffer)
{
    Client* client = _clients[fd];
    if (client)
    {
        if (buffer.find("NICK ") == 0)
        {
            std::string nick = buffer.substr(5);
            client->setNickname(nick);
        }
        else if (buffer.find("USER ") == 0)
        {
            std::string user = buffer.substr(5);
            client->setUsername(user);
        }
    }
}

void Server::receiveData(int fd)
{
    char buffer[512];
    int bytes = recv(fd, buffer, sizeof(buffer) - 1, 0);
    _clients[fd]->getBuffer() += buffer; 

    /*
        Faire en sorte que le buffer soit traité ligne par ligne, et que les commandes soient exécutées au fur et à mesure
    */
    if (bytes <= 0)
    {
        removeClient(fd);
        return;
    }

    buffer[bytes] = '\0';

    std::cout << "Client " << fd << " : " << buffer << std::endl;
    set_client(fd, buffer);
}
void send_instructions(int fd)
{
    std::string instructions;
    instructions = "Please set NICK and USER to get started.\n";
    send(fd, instructions.c_str(), instructions.size(), 0);
}

void send_welcome_message(int fd)
{
    std::string welcome_message;
    welcome_message = "Welcome to the IRC server!\n";
    send(fd, welcome_message.c_str(), welcome_message.size(), 0);
}