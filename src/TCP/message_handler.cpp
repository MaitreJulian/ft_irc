#include "../Network/server.hpp"
#include "../Network/client.hpp"


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
        std::string command = buffer.substr(0, pos);
        std::cout << "ca c'est la commande "<< command << std::endl;
        buffer.erase(0, pos + 2);

        if (!_clients[fd]->isAuthenticated())
        {
            std::cout <<  "va dasn autentificate" <<std::endl;
            Authentificate(command,fd);
        }
        else
            std::cout << "n'est pas alle dans auten" << std::endl;
    }
    std::cout << pos << std::endl;
}

void Server::receiveData(int fd)
{
    char buffer[512];

    std::cout << "Je suis dans reveive data" << std::endl;
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
    processClientbuffer(fd);    
}
void send_instructions(int fd)
{
    std::string instructions;
    instructions = "Please set NICK and USER to get started.\n'NICK (nickname)' or 'USER (user)'\n";
    send(fd, instructions.c_str(), instructions.size(), 0);
}

void send_welcome_message(int fd)
{
    std::string welcome_message;
    welcome_message = "Welcome to the IRC server!\n";
    send(fd, welcome_message.c_str(), welcome_message.size(), 0);
}