/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julian <julian@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 14:00:05 by julian            #+#    #+#             */
/*   Updated: 2026/06/16 15:26:40 by julian           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

void launch_server(int port, const std::string& password)
{
    try
    {
        Server server(port, password);
        server.initServer();
        server.run();
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        exit(1);   
    }
}


int check_port(const char *port_str)
{
    char *end;
    long port = strtol(port_str, &end, 10);

    if (*end != '\0' || port < 1024 || port > 65535)
        return -1;
    return static_cast<int>(port);
}

int main(int ac, char **av)
{
    if (ac != 3)
        return 1;
    int port = check_port(av[1]);
    if (port == -1)
    {
        std::cerr << "Invalid port number: " << av[1] << std::endl;
        return 1;
    }  
    std::string password = av[2];

    launch_server(port, password);
    return 0;
}