#include "irc.hpp"

// << TOPIC #aaa :fff
// >> :ggg!uuuuser@localhost TOPIC #aaa :fff

//>> :tip!uuuuser@localhost TOPIC #ttt :tooop

//>> :localhost 482 #ttt :You're not channel operator

int Server::topic_cmd(int i, char *msg)
{
	if (!strncmp(msg, "TOPIC #", 7))
	{
		std::string serv_msg;
		std::string chan = extract(msg, "#", ":");
		std::string newtopic = extract(msg, ":", "\0");
		newtopic.erase(newtopic.size() - 2, 2);
		if (!is_operator(i, chan))
		{
			serv_msg = ":localhost 482 #" + chan + " :You're not channel operator\r\n";
			status = send(this->users[i].socket_fd, serv_msg.c_str(), strlen(serv_msg.c_str()), 0);
				if (status == -1)
					std::cout << "[Server] Send error to client fd " << this->users[i].socket_fd << ": " << strerror(errno) << std::endl;
			return (1);
		}
		//std::cout << "chan =" << chan << "|" << "newtopic =" << newtopic << "|\n";
		// if (!is_valid_str(newtopic))
		// {

		// }
		serv_msg = ":" + this->users[i].nick + "!" + this->users[i].username + "@localhost TOPIC #" + chan + " :" + newtopic + "\r\n";
		for (int j = 1; j < this->get_poll_count(); j++)
		{	
			if (this->users[j].is_is_chan(chan))
			{
				status = send(this->users[j].socket_fd, serv_msg.c_str(), strlen(serv_msg.c_str()), 0);
				if (status == -1)
					std::cout << "[Server] Send error to client fd " << this->users[j].socket_fd << ": " << strerror(errno) << std::endl;
			}
		}
		this->channels[chan].topic = newtopic;
		return (1);
	}
	return (0);
}
