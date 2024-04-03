/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handlePoll.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blerouss <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 15:24:32 by blerouss          #+#    #+#             */
/*   Updated: 2024/03/22 17:09:50 by blerouss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/server.hpp"

int Server::calloc_pollfd(int size)
{
    if (!this->_poll_fds)
    {
        this->_poll_fds = (struct pollfd *) calloc(size + 1, sizeof *this->_poll_fds);
        if (!this->_poll_fds)
            return (0);
    }
    return (1);
}

void Server::add_to_poll_fds(int new_fd)
{
    // S'il n'y a pas assez de place, il faut réallouer le tableau de _poll_fds
    if (this->_poll_count == this->_poll_size) 
    {
        this->_poll_size *= 2; // Double la taille
        this->_poll_fds = (struct pollfd *) realloc(this->_poll_fds, sizeof(*(this->_poll_fds)) * (this->_poll_size));
        if (!this->_poll_fds)
            throw fatal_error();
    }
    this->_poll_fds[this->_poll_count].fd = new_fd;
    this->_poll_fds[this->_poll_count].events = POLLIN;
    this->_poll_count++;
}

void Server::del_from_poll_fds(int i)
{
    for (int j = 0; j < _poll_count; j++)
	{
        if (_poll_fds[j].fd == i)
        {
            this->_poll_fds[j] = this->_poll_fds[this->_poll_count - 1];
            this->_poll_count -= 1;
        }
    }
}
