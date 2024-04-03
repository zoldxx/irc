#include "inc/server.hpp"

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
    for (std::vector<std::string>::iterator it = this->_users[i].getChannels().begin(); it != this->_users[i].getChannels().end(); it++)
    {
		for (std::vector<int>::iterator it_user = this->_channels[(*it)].getUsers().begin() ; it_user != this->_channels[(*it)].getUsers().end() ; it_user++)
		{
			if ((*it_user) == i)
			{
				this->_channels[(*it)].getUsers().erase((it_user));
				break ;
			}
		}
		for (std::vector<int>::iterator it_operators = this->_channels[(*it)].getOperators().begin() ; it_operators != this->_channels[*it].getOperators().end() ; it_operators++)
		{
			if ((*it_operators) == i)
			{
				this->_channels[(*it)].getOperators().erase((it_operators));
				break ;
			}
		}
    }
    std::map<int, User>::iterator	it_users = this->_users.find(i);
	close(_users[i].getFd());
	del_from_poll_fds(_users[i].getFd());
    this->_users[i].getChannels().clear();
    this->_users.erase(it_users);
    return (i);
}

void		Server::handleMessage(int i)
{
	char			buffer[512];
	int				fd = _poll_fds[i].fd;

	memset(&buffer, '\0', sizeof(buffer));
	if (recv(fd, buffer, sizeof(buffer), 0) <= 0)
	{
		del_user(fd);
        close(this->_poll_fds[fd].fd);
        this->del_from_poll_fds(fd);
	}
	else
	{
		std::string					line = _users.find(fd)->second.getBuffer() + buffer;
		std::string					cmd;
		std::string					tmp;
    	std::string::size_type		end;
    	std::string::size_type		space;
		
		// std::cout <<  "line =" << line << std::endl;
		_users.find(fd)->second.setBuffer("");
		while ((end = line.find("\r\n", 0)) != std::string::npos)
		{
			cmd = line.substr(0, end);
			line.erase(0, end + 2);
			if ((space = cmd.find(" ", 0)) != std::string::npos)
			{
				tmp = cmd.substr(0, space);
				// std::cout <<  "tmp =" << tmp << std::endl;
				cmd.erase(0, space + 1);
				if (_command.find(tmp) != _command.end())
					(this->*_command[tmp])(_users.find(fd)->second, cmd);
			}
		}
		if (line.begin() != line.end())
			_users.find(fd)->second.setBuffer(line);
	}
}
