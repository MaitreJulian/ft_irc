#include "../Network/server.hpp"
#include "../Network/client.hpp"

void Server::set_client(int fd, const std::string& buffer)
{
}
int check_format(std::string commmand)
{
    std::cout <<"Dans la fonction check_format" << std::endl;
    return 1;
}

void Server::processClientbuffer(Client *clients)
{
    std::string& buffer = clients->getBuffer();
    std::cout <<  buffer << std::endl;

    size_t pos;
    while ((pos = buffer.find("\r\n")) != std::string::npos)
    {
        std::string command = buffer.substr(0, pos);
        std::cout << command << std::endl;
        buffer.erase(0, pos + 2);

        if (!clients->isAuthenticated())
        {
            if(command.compare(0, 5, "NICK") == 0  || command.compare(0, 5, "USER") == 0)
            {
                if(!check_format(command)) // Pas encore fait check_format pour USER et NICK
                {
                   std::cerr << "Invalid command from client" << std::endl;
                }
                else
                    set_client(clients->getFd(), command); //Pas encore fait set_client
            }
            else
                std::cerr << "Set USER and NICK correctly";
        }
        // else
        //     execute_IRC_command()
    }
}

void Server::receiveData(int fd)
{
    char buffer[512];

    while (true)
    {
        int bytes = recv(fd, buffer, sizeof(buffer), 0);

        if (bytes > 0)
        {
            _clients[fd]->getBuffer().append(buffer, bytes);
        }
        else if (bytes == 0)
        {
            removeClient(fd);
            return;
        }
        else
        {
            if (errno == EAGAIN || errno == EWOULDBLOCK)
                break;

            removeClient(fd);
            return;
        }
    }
    processClientbuffer(_clients[fd]);
    
}
void send_instructions(int fd)
{
    std::string instructions;
    instructions = "Please set NICK and USER to get started.\n'NICK (nickname)' or 'USER (user)'";
    send(fd, instructions.c_str(), instructions.size(), 0);
}

void send_welcome_message(int fd)
{
    std::string welcome_message;
    welcome_message = "Welcome to the IRC server!\n";
    send(fd, welcome_message.c_str(), welcome_message.size(), 0);
}