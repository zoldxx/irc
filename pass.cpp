#include "inc/server.hpp"

bool	Server::pass(User & client, std::string cmd)
{
	std::string		msg;
	if (cmd.find(" ", 0) == std::string::npos)
	{	
		if (cmd == this->_mdp)
		{
			msg = "Password accepted.\r\n";
			if (send(client.getFd(), msg.c_str(), msg.size(), 0) < 1)
				return (false);
			if (client.getStatus() == 1) 
				client.setStatus(2); 
			return (true);
		}
	}
	msg = "Wrong password\r\n";
	send(client.getFd(), msg.c_str(), msg.size(), 0);
    return (false);
}
