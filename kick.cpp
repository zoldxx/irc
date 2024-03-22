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

void Server::print_chan(int i)
{
    std::cout << "normal chan = ";
    for (std::vector<std::string>::iterator it = this->users[i].channel_normal.begin(); it != this->users[i].channel_normal.end(); it++)
        std::cout << *it << std::endl;
    std::cout << "operator chan = ";
    for (std::vector<std::string>::iterator it = this->users[i].channel_operators.begin(); it != this->users[i].channel_operators.end(); it++)
        std::cout << *it << std::endl;
}


void Server::kick_from_a_chan(std::string &user_to_kick, std::string &chan)
{
    int i;
    std::map<std::string, User>::iterator it_user = this->channels[chan].normal_users.find(user_to_kick);
    if (it_user != this->channels[chan].normal_users.end())
    {
        i = it_user->second.socket_fd;
        std::cout << "i = " << i << std::endl;
        this->channels[chan].normal_users.erase(it_user);
    }
    else
    {
        it_user = this->channels[chan].operators.find(user_to_kick);
        i = it_user->second.socket_fd;
        std::cout << "i = " << i << std::endl;
        if (it_user != this->channels[chan].operators.end())
            this->channels[chan].operators.erase(it_user);
    }
    print_chan(i);
    std::vector<std::string>::iterator it = std::find(this->users[i].channel_normal.begin(), this->users[i].channel_normal.end(), chan);
    std::cout << "test\n";
    if (it != this->users[i].channel_normal.end())
    {
        this->users[i].channel_normal.erase(it);
        std::cout << "test2\n";
    }
    else
    {
        std::cout << "test3\n";
        it = std::find(this->users[i].channel_operators.begin(), this->users[i].channel_operators.end(), chan);
        this->users[i].channel_operators.erase(it);
        std::cout << "test4\n";
    }
}

// /connect localhost 6667 aaa

int Server::kick(int i, char *msg)
{
	if (!strncmp(msg, "KICK #", 6))
	{
        (void)i;
        std::string str_msg = msg;
		std::string serv_msg;
		std::string chan = str_msg.substr(6, str_msg.find(" ", 6) - 6);
		std::string user_to_kick = str_msg.substr(7 + chan.size(), std::string::npos);
		user_to_kick.erase(user_to_kick.size() - 4, 4);
        //std::cout << "chan =" << chan << "|user to kick =" << user_to_kick << "|\n";
		if (!is_operator(i, chan))
		{
			serv_msg = ":localhost 482 #" + chan + " :You're not channel operator\r\n";
			status = send(this->users[i].socket_fd, serv_msg.c_str(), strlen(serv_msg.c_str()), 0);
				if (status == -1)
					std::cout << "[Server] Send error to client fd " << this->users[i].socket_fd << ": " << strerror(errno) << std::endl;
			return (1);
		}
		serv_msg = ":" + this->users[i].nick + "!" + this->users[i].username + "@localhost KICK #" + chan + " " + user_to_kick + " :\r\n";
		for (int j = 1; j < this->get_poll_count(); j++)
		{	
			if (this->users[j].is_is_chan(chan))
			{
				status = send(this->users[j].socket_fd, serv_msg.c_str(), strlen(serv_msg.c_str()), 0);
				if (status == -1)
					std::cout << "[Server] Send error to client fd " << this->users[j].socket_fd << ": " << strerror(errno) << std::endl;
			}
		}
        kick_from_a_chan(user_to_kick, chan);
		return (1);
	}
	return (0);
}