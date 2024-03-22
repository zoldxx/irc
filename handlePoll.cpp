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

#include "inc/irc.hpp"

int Server::calloc_pollfd(int size)
{
    if (!this->poll_fds)
    {
        this->poll_fds = (struct pollfd *) calloc(size + 1, sizeof *this->poll_fds);
        if (!this->poll_fds)
            return (0);
    }
    return (1);
}

void Server::add_to_poll_fds(int new_fd)
{
    // S'il n'y a pas assez de place, il faut réallouer le tableau de poll_fds
    if (this->poll_count == this->poll_size) 
    {
        this->poll_size *= 2; // Double la taille
        this->poll_fds = (struct pollfd *) realloc(this->poll_fds, sizeof(*(this->poll_fds)) * (this->poll_size));
        if (!this->poll_fds)
            throw fatal_error();
    }
    this->poll_fds[this->poll_count].fd = new_fd;
    this->poll_fds[this->poll_count].events = POLLIN;
    this->poll_count++;
}

void Server::del_from_poll_fds(int i)
{
    this->poll_fds[i] = this->poll_fds[this->poll_count - 1];
    this->poll_count -= 1;
}
