#include "inc/server.hpp"
#include "inc/irc.hpp"

// << TOPIC #aaa :fff
// >> :ggg!uuuuser@localhost TOPIC #aaa :fff

//>> :tip!uuuuser@localhost TOPIC #ttt :tooop

//>> :localhost 482 #ttt :You're not channel operator

bool	Server::topic(User &client, std::string cmd)
{
	std::string serv_msg;
	std::string chan = extract(cmd, "#", ":");
	std::string newtopic = extract(cmd, ":", "\0");
	if (!_channels[chan].isOperator(client.getFd()))
	{
		serv_msg = ":localhost 482 #" + chan + " :You're not channel operator\r\n";
		if (send(client.getFd(), serv_msg.c_str(), serv_msg.size(), 0) < 1)
            return (false);
	}
	serv_msg = ":" + client.getNick() + "!" + client.getUsername() + "@localhost TOPIC #" + chan + " :" + newtopic + "\r\n";
	for (std::vector<int>::iterator it = _channels[chan].getUsers().begin(); it != _channels[chan].getUsers().end(); it++)
	{
		if (send(*it, serv_msg.c_str(), serv_msg.size(), 0) < 1)
            return (false);
	}
	for (std::vector<int>::iterator it = _channels[chan].getOperators().begin(); it != _channels[chan].getOperators().end(); it++)
	{
		if (send(*it, serv_msg.c_str(), serv_msg.size(), 0) < 1)
            return (false);
	}
	_channels[chan].setTopic(newtopic);
	return (1);
}

// int Server::topic_cmd(int i, char *msg)
// {
// 	if (!strncmp(msg, "TOPIC #", 7))
// 	{
// 		std::string serv_msg;
// 		std::string chan = extract(msg, "#", ":");
// 		std::string newtopic = extract(msg, ":", "\0");
// 		newtopic.erase(newtopic.size() - 2, 2);
// 		std::cout << "chan =" << chan << "|" << "newtopic =" << newtopic << "|\n";
// 		if (!is_operator(i, chan))
// 		{
// 			serv_msg = ":localhost 482 #" + chan + " :You're not channel operator\r\n";
// 			status = send(this->users[i].socket_fd, serv_msg.c_str(), strlen(serv_msg.c_str()), 0);
// 				if (status == -1)
// 					std::cout << "[Server] Send error to client fd " << this->users[i].socket_fd << ": " << strerror(errno) << std::endl;
// 			return (1);
// 		}
// 		// if (!is_valid_str(newtopic))
// 		// {

// 		// }
// 		serv_msg = ":" + this->users[i].nick + "!" + this->users[i].username + "@localhost TOPIC #" + chan + " :" + newtopic + "\r\n";
// 		for (int j = 1; j < this->get_poll_count(); j++)
// 		{	
// 			if (this->users[j].is_is_chan(chan))
// 			{
// 				status = send(this->users[j].socket_fd, serv_msg.c_str(), strlen(serv_msg.c_str()), 0);
// 				if (status == -1)
// 					std::cout << "[Server] Send error to client fd " << this->users[j].socket_fd << ": " << strerror(errno) << std::endl;
// 			}
// 		}
// 		this->channels[chan].topic = newtopic;
// 		return (1);
// 	}
// 	return (0);
// }
