/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blerouss <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 15:22:39 by blerouss          #+#    #+#             */
/*   Updated: 2024/03/22 17:01:23 by blerouss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/irc.hpp"

int User::is_is_chan(std::string &chan)
{

    if (std::find(this->channel_normal.begin(), this->channel_normal.end(), chan) != this->channel_normal.end() || 
            std::find(this->channel_operators.begin(), this->channel_operators.end(), chan) != this->channel_operators.end())
        return (1);
    
    return (0);
}

int Server::is_operator(int i, std::string &chan)
{
	    if (std::find(this->users[i].channel_operators.begin(), this->users[i].channel_operators.end(), chan) != this->users[i].channel_operators.end())
        return (1);  
    return (0);
}

int valid_args(std::string port, std::string mdp)
{
    if (port.size() < 3 || port.size() > 5)
        return (0);
    for (size_t i = 0; i < port.size(); i++)
    {
        if (!(port[i] >= '0' && port[i] <= '9'))
            return (0);
    }
    if (mdp.size() < 1)
        return (0);
    return (1);
}
