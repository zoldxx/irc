/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blerouss <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 17:03:19 by blerouss          #+#    #+#             */
/*   Updated: 2024/03/22 17:03:28 by blerouss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/irc.hpp"

int Server::kick(int i, char *msg)
{
	if (!strncmp(msg, "KICK #", 6))
	{
        (void)i;
        std::string str_msg = msg;
		std::string serv_msg;
		std::string chan = extract(msg, "#", " ");
		std::string user_to_kick = str_msg.substr(7 + chan.size(), std::string::npos);
		user_to_kick.erase(user_to_kick.size() - 4, 4);
        std::cout << "chan =" << chan << "|user to kick =" << user_to_kick << "|\n";
		// if (!is_operator(i, chan))
		// {
		// 	serv_msg = ":localhost 482 #" + chan + " :You're not channel operator\r\n";
		// 	status = send(this->users[i].socket_fd, serv_msg.c_str(), strlen(serv_msg.c_str()), 0);
		// 		if (status == -1)
		// 			std::cout << "[Server] Send error to client fd " << this->users[i].socket_fd << ": " << strerror(errno) << std::endl;
		// 	return (1);
		// }

		// serv_msg = ":" + this->users[i].nick + "!" + this->users[i].username + "@localhost TOPIC #" + chan + " :" + newtopic + "\r\n";
		// for (int j = 1; j < this->get_poll_count(); j++)
		// {	
		// 	if (this->users[j].is_is_chan(chan))
		// 	{
		// 		status = send(this->users[j].socket_fd, serv_msg.c_str(), strlen(serv_msg.c_str()), 0);
		// 		if (status == -1)
		// 			std::cout << "[Server] Send error to client fd " << this->users[j].socket_fd << ": " << strerror(errno) << std::endl;
		// 	}
		// }
		// this->channels[chan].topic = newtopic;
		return (1);
	}
	return (0);
}
