#include "inc/server.hpp"

// int Server::first_message(int i, char *msg)
// {
//     std::string str_msg = msg;
//     std::string serv_msg;
//     std::string pswd;
//     std::string nick;

// 	this->users[i].socket_fd = this->poll_fds[i].fd;
	
	
// 	nick = str_msg.substr(20 + this->mdp.size(), str_msg.find("\r\n", 20 + this->mdp.size()) - this->mdp.size() - 20);
// 	if (!is_valid_nick(nick))
// 	{
// 	 serv_msg = ":localhost 432 " + nick + " :Erroneous nickname\r\n";
// 	 status = send(this->users[i].socket_fd, serv_msg.c_str(), strlen(serv_msg.c_str()), 0);
// 		if (status == -1)
// 			std::cout << "bbb[Server] Send error to client fd " << this->users[i].socket_fd << ": " << strerror(errno) << std::endl;
// 	 return (1);
// 	}
	
// 	this->users[i].nick = str_msg.substr(20 + this->mdp.size(), str_msg.find("\r\n", 20 + this->mdp.size()) - this->mdp.size() - 20);
// 	this->users[i].username = str_msg.substr(27 + this->users[i].nick.size() + this->mdp.size(),
// 	            str_msg.find(" ", 27 + this->users[i].nick.size() +this->mdp.size()) - this->users[i].nick.size() - this->mdp.size() - 27);
// 	serv_msg = "001 " + this->users[i].nick + " Welcome to 42 serv " + this->users[i].nick + "!~" + this->users[i].username + "@127.0.0.1\n";
// 	status = send(this->poll_fds[i].fd, serv_msg.c_str(), strlen(serv_msg.c_str()), 0);
// 	return (1);
// }
