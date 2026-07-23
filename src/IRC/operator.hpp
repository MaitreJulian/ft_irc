#ifndef OPERATOR_HPP
#define OPERATOR_HPP

#include "channel.hpp"

class Operator
{
    private : 
        Client client;

    public:
        void createChannel(Channel *channel);
};


#endif