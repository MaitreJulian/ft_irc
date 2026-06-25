#include "../Network/server.hpp"
#include "../Network/client.hpp"

// void Server::set_client(int fd, const std::string& buffer)
// {
// }
std::vector<std::string> split_command(std::string command)
{
    std::istringstream iss(command);
    std::vector<std::string> tokens;
    std::string token;

    while (iss >> token)
        tokens.push_back(token);
    return tokens;
}

int Server::Authentificate(std::string command, int fd)
{
    std::cout << "Dans Authentificate avec client fd : " << fd << std::endl;
    std::vector<std::string> s_command = split_command(command);
    if(s_command.size() == 2 && s_command[0] == "NICK")
    {
        _clients[fd]->setNickname(s_command[1]);
    }
    else if (s_command.size() == 5 && s_command[0] == "USER")
    {
        _clients[fd]->setUsername(s_command[1]);
    }
    else 
    {
        std::cerr << "Invalid command from client" << std::endl;
        std::string instructions;
        instructions = "Set USER and NICK correctly\n'NICK (nickname)' or 'USER (user) 0 * :(real name)'\n";
        send(fd, instructions.c_str(), instructions.size(), 0);
    }   
    return 1;
}