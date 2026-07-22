#include "../Network/server.hpp"
#include "../Network/client.hpp"

void ft_command_size(std::vector<std::string> command)
{
    if (command.size() == 0)
        return ;
    size_t size = command.size();
    size_t i = 0;
    while (i < size)
    {
        std::cout << "command size : "<< size <<"[" << command[i] << "]";
        i++;
    }
    std::cout<<std::endl;
}


bool Server::UsernameExist(const std::string& username)
{
    std::cout<<"Dans UsernameExist" <<std::endl;
    for (std::map<int, Client*>::iterator it = _clients.begin();
         it != _clients.end();
         ++it)
    {
        if (!it->second->getUsername().empty() &&
            it->second->getUsername() == username)
            return true;
    }
    return false;
}
bool Server::NicknameExist(const std::string& nickname)
{
    std::cout << "dnas NicknameExist" << std::endl;
    for (std::map<int, Client*>::iterator it = _clients.begin();
         it != _clients.end();
         ++it)
    {
        if (!it->second->getNickname().empty() &&
            it->second->getNickname() == nickname)
            return true;
    }
    return false;
}

std::vector<std::string> split_command(std::string command)
{
    std::istringstream iss(command);
    std::vector<std::string> tokens;
    std::string token;

    while (iss >> token)
        tokens.push_back(token);
    return tokens;
}

int Server::Authentificate(std::string &buffer,size_t pos, int fd)
{

    std::cout << "Dans Authentificate avec client fd : " << fd << std::endl;
    std::string command = buffer.substr(0, pos);
    std::cout << "ca c'est la commande "<< command << std::endl;
    buffer.erase(0, pos + 2);

    std::string instructions;
    std::vector<std::string> s_command = split_command(command);
    ft_command_size(s_command);
    
    
    if(s_command.size() == 2 && s_command[0] == "PASS")
    {
        if(_password == s_command[1])
            _clients[fd]-> setpass();
        else
        {
            std::cerr << "WRONG PASSWORD" << std::endl;
            instructions = "Password incorrect";
            send(fd, instructions.c_str(), instructions.size(), 0);
        }
        return 1;
    }
    else if(s_command.size() == 2 && s_command[0] == "NICK")
    {
        if (!NicknameExist(s_command[1]))
            _clients[fd]->setNickname(s_command[1]);
        else
        {
            instructions = "Nickname exist already or is empty";
            send(fd, instructions.c_str(), instructions.size(), 0);

        }
    }
    else if (s_command.size() == 5 && s_command[0] == "USER")
    {
        if (!UsernameExist(s_command[1]))
            _clients[fd]->setUsername(s_command[1]);
        else
        {
            instructions = "Username exist already or is empty";
            send(fd, instructions.c_str(), instructions.size(), 0);
        }
    }
    else 
    {
        std::cerr << "Invalid command from client" << std::endl;
        std::string instructions;
        instructions = "Set USER and NICK correctly\n'NICK (nickname)' and 'USER (user) 0 * :(real name)'\n";
        send(fd, instructions.c_str(), instructions.size(), 0);
        return 1;
    }
    if (_clients[fd]->isAuthenticated())
    {
        sendNumericReply(fd, "001", ":Welcome to ft_irc " + _clients[fd]->getNickname() + "!" + _clients[fd]->getUsername() + "@localhost\r\n");
        return 1;
    }
    return 0;
}