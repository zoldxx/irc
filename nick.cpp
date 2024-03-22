/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blerouss <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 17:04:14 by blerouss          #+#    #+#             */
/*   Updated: 2024/03/22 17:04:19 by blerouss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/irc.hpp"

// << NICK zoldxxxx

int Server::is_valid_str(std::string &str)
{
	for (size_t i = 0; i < str.size(); ++i)
	{
		if (!std::isalnum(str[i]))
			return (0);
	}
	return (1);
}


int Server::is_valid_nick(std::string &nick)
{
	for (std::map<int, User>::iterator it = users.begin(); it != users.end(); ++it)
	{
		if (it->second.nick == nick)
			return (0);
	}
	if (!is_valid_str(nick))
		return (0);
	return (1);
}

int Server::set_nick(int i, char *msg)
{
	if (!strncmp(msg, "NICK ", 5))
	{
		std::string serv_msg;
		std::string str_msg = msg;
		std::string new_nick = str_msg.substr(5, std::string::npos);
		new_nick.erase(new_nick.size() - 2, 2);
		if (!is_valid_nick(new_nick))
		{
         serv_msg = ":localhost 432 " + new_nick + " :Erroneous nickname\r\n";
         status = send(this->users[i].socket_fd, serv_msg.c_str(), strlen(serv_msg.c_str()), 0);
			if (status == -1)
				std::cout << "[Server] Send error to client fd " << this->users[i].socket_fd << ": " << strerror(errno) << std::endl;
         return (1);
		}
		serv_msg = ":" + this->users[i].nick + "!~" + this->users[i].username + " NICK :" + new_nick + "\r\n";
		for (int j = 1; j < this->get_poll_count(); j++)
		{
			status = send(this->users[j].socket_fd, serv_msg.c_str(), strlen(serv_msg.c_str()), 0);
			if (status == -1)
				std::cout << "[Server] Send error to client fd " << this->users[j].socket_fd << ": " << strerror(errno) << std::endl;
		}
		this->users[i].nick = new_nick;
		return (1);
	}
	return (0);
}

// int Server::set_nick(int i, char *msg)
// {
//      if (!strncmp(msg, "NICK ", 5))
//      {
//         std::string str_msg = msg;
//         std::string new_nick = str_msg.substr(5, std::string::npos);
//         new_nick.erase(new_nick.size() - 2, 2);
//         std::string serv_msg = ":" + this->users[i].nick + "!~"
//	+ this->users[i].username + " NICK :" + new_nick + "\r\n";
//         for (int j = 1; j < this->get_poll_count(); j++)
//         {
//                 status = send(this->users[j].socket_fd, serv_msg.c_str(),
//		strlen(serv_msg.c_str()), 0);
//                 if (status == -1)
//                     std::cout << "[Server] Send error to client fd " << this->users[j].socket_fd << ": " << strerror(errno) << std::endl;
//         }
//         this->users[i].nick = new_nick;
//         return (1);
//      }
//      return (0);
// }

// int Server::set_nick(int i, char *msg)
// {
//      if (!strncmp(msg, "NICK ", 5))
//      {
//         std::string str_msg = msg;
//         std::string new_nick = str_msg.substr(5, std::string::npos);
//         new_nick.erase(new_nick.size() - 2, 2);
//         std::string serv_msg = ":" + this->users[i].nick + "!~"
//	+ this->users[i].username + " NICK :" + new_nick + "\r\n";
//         status = send(this->poll_fds[i].fd, serv_msg.c_str(),
//		strlen(serv_msg.c_str()), 0);
//         if (status == -1)
//             std::cout << "[Server] Send error to client " << this->poll_fds[i].fd << strerror(errno) << std::endl;
//         this->users[i].nick = new_nick;
//         return (1);
//      }
//      return (0);
// }
