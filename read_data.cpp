/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_data.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blerouss <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 17:00:33 by blerouss          #+#    #+#             */
/*   Updated: 2024/03/22 17:32:52 by blerouss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/irc.hpp"

std::string extract(const std::string& chaine, std::string begin, std::string end) 
{
    std::size_t debut = chaine.find(begin);
    std::size_t fin = chaine.find(end, debut + 1) - 1;
    if (debut != std::string::npos && fin != std::string::npos) 
        return chaine.substr(debut + 1, fin - debut - 1);
    else 
        return "";
}

int Server::del_user(int i)
{
    for (std::vector<std::string>::iterator it = this->users[i].channel_normal.begin(); it != this->users[i].channel_normal.end(); it++)
    {
        std::map<std::string, User>::iterator it_user = this->channels[*it].normal_users.find(this->users[i].nick);
        this->channels[*it].normal_users.erase(it_user);
    }
    for (std::vector<std::string>::iterator it = this->users[i].channel_operators.begin(); it != this->users[i].channel_operators.end(); it++)
    {
        std::map<std::string, User>::iterator it_user = this->channels[*it].operators.find(this->users[i].nick);
        this->channels[*it].operators.erase(it_user);
    }
    std::map<int, User>::iterator it_user = this->users.find(i);
    this->users.erase(it_user);
    this->users[i].channel_normal.clear();
    this->users[i].channel_operators.clear();
    return (i);
}

void		Server::handleMessage(int fd)
{
	char			buffer[512];

	memset(&buffer, '\0', sizeof(buffer));
	if (recv(fd, buffer, sizeof(buffer) - 1, MSG_DONTWAIT) <= 0)
	{
		del_user(fd);
        close(this->poll_fds[fd].fd);
        this->del_from_poll_fds(fd);
	}
	else
	{
		std::string					line = users.find(fd)->second.getBuffer() + buffer;
		std::string					cmd;
		std::string					tmp;
    	std::string::size_type		end;

		users.find(fd)->second.setBuffer("");
		while ((end = line.find("\r\n", 0)) != std::string::npos)
		{
			cmd = line.substr(0, end);
			line.erase(0, end + 2);
			if (cmd.find(" ", 0) != std::string::npos)
			{
				tmp = cmd.substr(0, cmd.find(" ", 0));
				cmd.erase(0, cmd.find(" ", 0) + 1);
				if (_Command.find(tmp) != _Command.end())
					_Command.find(tmp)->second(users.find(fd)->second, cmd);
			}
		}
		if (line.begin() != line.end())
			users.find(fd)->second.setBuffer(line);
	}
}
