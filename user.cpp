/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blerouss <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 16:52:11 by blerouss          #+#    #+#             */
/*   Updated: 2024/03/25 14:44:56 by blerouss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/irc.hpp"

User::User(int socket) : socket_fd(socket)
{
}

User::~User(void)
{
}

int					User::getSocket(void)
{
	return this->socket_fd;
}
std::string		User::getBuffer(void)
{
	return _buffer;
}

void			User::setBuffer(std::string buffer)
{
	_buffer = buffer;
}

int 			User::is_is_chan(std::string &chan)
{

    if (std::find(this->channel_normal.begin(), this->channel_normal.end(), chan) != this->channel_normal.end() || 
            std::find(this->channel_operators.begin(), this->channel_operators.end(), chan) != this->channel_operators.end())
        return (1);
    
    return (0);
}
