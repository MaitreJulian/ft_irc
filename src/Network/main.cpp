/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fhanuise <fhanuise@student.42belgium.be    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 14:00:05 by julian            #+#    #+#             */
/*   Updated: 2026/08/04 17:02:17 by fhanuise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"
#include <csignal>
#include <cerrno>
#include <cstring>

sig_atomic_t g_signal = 0;

void handleSigint(int sig)
{
    (void)sig;
    g_signal = 1;
}


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
    struct sigaction sa;
    std::memset(&sa, 0, sizeof(sa));   // évite les champs non initialisés
    sa.sa_handler = handleSigint;
    sigemptyset(&sa.sa_mask);          // aucun signal bloqué pendant l'exécution du handler
    sa.sa_flags = 0;                   // pas de SA_RESTART : on veut que poll() soit interrompu

    if (sigaction(SIGINT, &sa, NULL) == -1)
    {
        std::cerr << "sigaction failed" << std::endl;
        return 1;
    }
    if (sigaction(SIGTERM, &sa, NULL) == -1)
    {
        std::cerr << "sigaction failed" << std::endl;
        return 1;
    }
    launch_server(port, password);
    return 0;
}