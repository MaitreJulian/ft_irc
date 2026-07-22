#include "../../Network/server.hpp"
#include "../../Network/client.hpp"
#include "../channel.hpp"

std::vector<std::string> parseIRC(std::string command)
{
    std::vector<std::string> args;
    std::string current;

    for (size_t i = 0; i < command.size(); i++)
    {
        if (command[i] == ' ')
        {
            if (!current.empty())
            {
                args.push_back(current);
                current.clear();
            }
        }
        else if (command[i] == ':' && current.empty())
        {
            // tout ce qui suit devient un seul argument
            args.push_back(command.substr(i + 1));
            break;
        }
        else
        {
            current += command[i];
        }
    }

    if (!current.empty())
        args.push_back(current);

    return args;
}

int Server::execute_irc_command(std::string &buffer, size_t pos, int fd)
{
    std::string command = buffer.substr(0, pos);
    std::cout << "ca c'est la commande "<< command << std::endl;
    buffer.erase(0, pos + 2);
    
    std::vector<std::string> s_command = parseIRC(command);
    std::cout <<"size of command ==" << s_command.size() << std::endl;
    if (s_command.size() == 2 && s_command[0] == "JOIN")
    {
        std::cout<< "Dans join" << std::endl;
        channel_joined(s_command[1], fd);
    }
    else if (s_command.size() == 3 && s_command[0] == "PRIVMSG")
    {
        send_message(s_command, fd);
    }
    return 0;

}
