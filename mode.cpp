#include "inc/server.hpp"

bool	Server::mode_send_message(int fd, std::string msg)
{
	if (send(fd, msg.c_str(), msg.size(), 0) < 1)
	{
	    close(fd);
	    del_from_poll_fds(fd);
		return (false);
	}
	return (true);
}

bool	Server::mode_check_chan(std::vector<std::string> & param, User & client)
{
	std::string					msg;

	if (param[0] == client.getNick())
		return (false);
	if (param[0].size() < 2 || param[0][0] != '#' || _channels.find(param[0].substr(1, param[0].size() - 1)) != _channels.end())
	{
		mode_send_message(client.getFd(), ":localhost 403 " + client.getNick() + " " + param[0] + " :No such channel");
		return (false);
	}
	param[0].erase(0, 1);
	return (true);
}

bool	Server::mode_print_info(User & client, std::string name, Channel &chan)
{
	std::string					msg;

	msg = ":localhost 324 " + client.getNick() + " " + name + " +";
	if (chan.getMTopic() == true)
		msg += "t";
	if (chan.getInvit() == true)
		msg += "i";
	if (chan.getMaxUser() != -1)
		msg += "l";
	if (chan.getPassword() != "")
		msg += "k";
	if (chan.getMaxUser() != -1)
	{
		std::stringstream ss;
    	ss << chan.getMaxUser();
    	std::string str = ss.str();
		msg += " " + str;
	}
	if (chan.getPassword() != "")
		msg += " " + chan.getPassword();
	if (mode_send_message(client.getFd(), msg) == false)
		return (false);
	return (true);
}

bool	Server::mode_check_option(std::string option, User & client, std::string chan)
{
	int				j = 0;
	int				i = 0;
	int				o = 0;
	int				t = 0;
	int				l = 0;
	int				k = 0;

	while (j < static_cast<int>(option.size()))
	{
		if (option[j] == 'i')
			i++;
		else if (option[j] == 'o')
			o++;
		else if (option[j] == 't')
			t++;
		else if (option[j] == 'l')
			l++;
		else if (option[j] == 'k')
			k++;
		j++;
	}
	if (i > 1 || o > 1 || t > 1 || l > 1 || k > 1)
	{
		if (mode_send_message(client.getFd(), ":localhost " + client.getNick() + " " + chan + " :Syntax error in the option") == false)
			return (false);
	}
	return (true);
}

bool	Server::mode(User &client, std::string cmd)
{
	std::vector<std::string>			param;		
	Channel								*chan;
	std::string							option("itklo");		
	std::string							msg;
	std::string							res;		
    std::string::size_type				end;
	std::vector<std::string>::iterator	it;
	std::map<int, User>::iterator 		iter = _users.begin();
	bool								type = true;
	
	while ((end = cmd.find(" ", 0)) != std::string::npos)
	{
		param.push_back(cmd.substr(0, end));
		cmd.erase(0, end + 1);
	}
	param.push_back(cmd);
	msg = ":" + client.getUsername() + "!~" + client.getNick() + "@127.0.0.1.ip MODE #" + param[0] + " ";
	if (mode_check_chan(param, client) == false)
		return (false);	
	else
		chan = &_channels[param[0]];
	if (param.size() == 1)
		return (mode_print_info(client, param[0], *chan));
	param.erase(param.begin());
	if (mode_check_option(param[1], client, param[0]) == false)
		return (false);
	for (std::string::iterator ite = param[1].begin(); ite != param[1].end() ; ite++)
	{
		if (*ite == '+')
			type = true;
		else if (*ite == '-')
			type = false;
		else if ((end = option.find(*ite)) != std::string::npos)
		{
			switch (end)
			{
				case 0:
					if (type == true)
						chan->setInvit(true);
					else
						chan->setInvit(false);
					break;
				case 1:
					if (type == true)
						chan->setMTopic(true);
					else
						chan->setMTopic(false);
					break;
				case 2:
					if (type == true)
					{
						chan->setPassword(param[2]);
						param.erase(param.begin() + 2);
					}
					else
						chan->setPassword("");
					break;
				case 3:
					if (type == true)
					{
						std::istringstream iss(param[2].c_str());
						int result;
						if ((iss >> result))
						{
							if (result > static_cast<int>(chan->getUsers().size() + chan->getOperators().size()))
								chan->setMaxUser(result);
							else
							{
								if (mode_send_message(client.getFd(), ":localost " + client.getNick() + " " + param[2] + " :there are already too many users in the channel to set this limit") == false)
									return (false);
							}
						}
						param.erase(param.begin() + 2);
						break;
					}
					else
						chan->setMaxUser(-1);
					break;
				case 4:
					while (iter != _users.end())
					{
						if (iter->second.getNick() == param[2])
						{
							if (chan->isInChan(iter->second.getFd()) == false)
							{
								if (mode_send_message(client.getFd(), ":localost 441 " + client.getNick() + " " + param[2] + " :They aren't on that channel") == false)
									return (false);
							}
							break;
						}
						iter++;
					}
					if (iter == _users.end() || chan->isInChan(iter->second.getFd()) == false)
						break;
					if (type == true)
					{
						if (chan->isOperator(iter->second.getFd()) == false)
						{
							chan->addOperator(iter->second.getFd());
							chan->delUser(iter->second.getFd());
						}
					}
					else
					{
						if (chan->isOperator(iter->second.getFd()) == true)
						{
							chan->addUser(iter->second.getFd());
							chan->delOperator(iter->second.getFd());
						}
					}
					break;
					
			}
		}
		else
		{
			if (mode_send_message(client.getFd(), ":localost 472 " + client.getNick() + " " + *ite + " :is unknown mode char to me"))
				return (false);
		}
	}
    return (true);
}
