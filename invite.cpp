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
	if (nick.size() == 0 || channel.size() == 0)
		//Exception ou msg d'erreur ?
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
	std::map<std::string, Channel>::iterator it_channel;
	it_channel = _channels.find(channel);
	std::vector<int>::iterator it_oper;
	if (it_channel != _channels.end())
	{
		for (it_oper = it_channel->second.getOperators().begin() ; it_oper != it_channel->second.getOperators().end() ; it_oper++)
			if ((*it_oper) == client.getFd())
				break;
		if (it_oper == it_channel->second.getOperators().end())
			//msg probleme pas operateur
			;
	}
	else
		//exception no nick/channel;
		;
	std::map<int, User>::iterator it_guest;
	for (it_guest = _users.begin() ; it_guest != _users.end() ; it_guest++)
	{
		if (it_guest->second.getNick() == nick)
			break;
	}
	if (it_guest == _users.end())
		//send || :nonstop.ix.me.dal.net 401 finf #finf :No such nick/channel || to inviter;
		;
	std::vector<int>::iterator it_users;
	std::vector<int>::iterator it_operators;
	for (it_users = it_channel->second.getUsers().begin() ; it_users != it_channel->second.getUsers().end() ; it_users++)
	{
		if ((*it_users) == it_guest->second.getFd())
			break;
	}
	for (it_operators = it_channel->second.getOperators().begin() ; it_operators != it_channel->second.getOperators().end() ; it_operators++)
	{
		if ((*it_operators) == it_guest->second.getFd())
			break;
	}
	if (it_users == it_channel->second.getUsers().end() && it_operators == it_channel->second.getOperators().end())
	{
		it_channel->second.getWhitelist().push_back(it_guest->second.getFd());
		//>> :nonstop.ix.me.dal.net 341 finf finfin #finf
		//>> :serenity.fl.us.dal.net NOTICE @#finf :finf invited finfin into channel #finf
	}
	else
		//exception ou msg user already in channel
		;
	return true;
}
