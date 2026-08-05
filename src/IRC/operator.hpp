/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operator.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fhanuise <fhanuise@student.42belgium.be    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/04 17:28:14 by fhanuise          #+#    #+#             */
/*   Updated: 2026/08/04 17:28:15 by fhanuise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


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