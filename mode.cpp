/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blerouss <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 17:03:50 by blerouss          #+#    #+#             */
/*   Updated: 2024/03/22 17:03:55 by blerouss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/irc.hpp"


// << MODE zoldxx +i
// >> :zoldxx MODE zoldxx :+iH

int Server::mode(int i, char *msg)
{
     if (!strncmp(msg, "MODE ", 5))
     {
        std::string serv_msg = ":" + this->users[i].nick + " MODE " + this->users[i].nick + " :+iH" + "\r\n";
        status = send(this->poll_fds[i].fd, serv_msg.c_str(), strlen(serv_msg.c_str()), 0);
        if (status == -1)
            std::cout << "[Server] Send error to client " << this->poll_fds[i].fd << strerror(errno) << std::endl;
        return (1);
     }
     return (0);
}
