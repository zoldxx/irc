	#include "inc/server.hpp"

bool	Server::invite(User & client, std::string cmd)
{
	(void)client;
	(void)cmd;
	std::string					nick("");
	std::string					channel("");

	if (cmd.find(" ", 0) != std::string::npos)
		nick = cmd.substr(0, cmd.find(" ", 0));
	cmd.erase(0, cmd.find(" ", 0) + 1);
	channel = cmd;
	if (channel[0] != '#')
	{
		if (nick[0] != '#')
		{
			std::cout << "Wrong parameters for invite (to change to good error msg)" << std::endl;
			return false;
		}
		else
			std::swap(nick, channel);
	}
	return true;
}
