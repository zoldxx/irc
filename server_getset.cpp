/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getter_setter.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blerouss <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 15:25:28 by blerouss          #+#    #+#             */
/*   Updated: 2024/03/22 17:01:23 by blerouss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/server.hpp"

void			Server::set_serv_socket(int sock)
{
    _server_socket = sock;
}

int				Server::get_serv_socket(void)
{
    return (_server_socket);
}

struct pollfd 	*Server::get_pollfd(void)
{
    return (_poll_fds);
}

int				Server::get_poll_count(void)
{
    return (_poll_count);
}

int 			Server::get_port(void)
{
    return (_port);
}
