#include "inc/server.hpp"

bool	Server::is_valid_nick(std::string nick, std::string &msg)
{
	if (nick.size() < 1)
	{
    	msg = ":" + nick + "!~" + "*@bdtServer 431 " + ": No nickname given\r\n";
		return (false);
	}
	for (std::map<int, User>::iterator it = _users.begin(); it != _users.end(); ++it)
	{
		if (it->second.getNick() == nick)
		{
    		msg = nick + " is already in use\r\n";
			return (false);
		}
	}
	for (size_t i = 0; i < nick.size(); ++i)
	{
		if (!std::isgraph(nick[i]))
		{
    		msg = ":" + nick + "!~" + "*@bdtServer 432 " + ": Erroneous nickname\r\n";
			return (false);
		}
	}
	return (true);
}

bool Server::nick(User & client, std::string cmd)
{
	std::string					msg("");
    std::string::size_type		space;
	std::string					nick_msg("");
	if ((space = cmd.find(" ", 0)) != std::string::npos)
		cmd = cmd.substr(0, space);
	if (!is_valid_nick(cmd, msg))
	{
    	if (send(client.getFd(), msg.c_str(), msg.size(), 0) < 1)
			del_user(client.getFd());
	   	return (false);
	}
	msg = ":" + client.getNick() + " NICK :" + cmd + "\r\n";
	std::vector<std::string> vec_chan = client.getChannels();
	if (client.getStatus() == 4)
	{
		for (std::vector<std::string>::iterator it = vec_chan.begin(); it != vec_chan.end(); it++)
		{
			std::vector<int> vec_user = _channels.find(*it)->second.getUsers();
			for (std::vector<int>::iterator ite = vec_user.begin(); ite != vec_user.end(); ite++)
			{
				if (send(*ite, msg.c_str(), msg.size(), 0) < 1)
				{
					del_user(*ite);
					return (false);
				}
			}
		}
	}
	client.setNick(cmd);
	if (client.getStatus() == 2)
		client.setStatus(3);
	return (true);
}
