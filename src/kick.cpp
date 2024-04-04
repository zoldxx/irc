#include "../inc/server.hpp"

void Server::kick_from_a_chan(std::string &chan, int fd_to_kick)
{
    std::vector<int>::iterator it = std::find(_channels[chan].getUsers().begin(), _channels[chan].getUsers().end(), fd_to_kick);
    if (it != _channels[chan].getUsers().end())
        _channels[chan].getUsers().erase(it);

    it = std::find(_channels[chan].getOperators().begin(), _channels[chan].getOperators().end(), fd_to_kick);
        if (it != _channels[chan].getOperators().end())
        _channels[chan].getOperators().erase(it);
    
    std::vector<std::string>::iterator ite = std::find(_users[fd_to_kick].getChannels().begin(),_users[fd_to_kick].getChannels().end(), chan);
    if (ite != _users[fd_to_kick].getChannels().end())
        _users[fd_to_kick].getChannels().erase(ite);
}

bool Server::kick(User &client, std::string cmd)
{
    std::string serv_msg;
    std::string chan = cmd.substr(1, cmd.find(" ", 1) - 1);
    std::string user_to_kick = cmd.substr(2 + chan.size(), std::string::npos);
    user_to_kick.erase(user_to_kick.size() - 2, 2);

    int fd_to_kick = 0;
    for (std::map<int, User>::iterator it = _users.begin(); it != _users.end(); it++)
    {
        if (it->second.getNick() == user_to_kick)
        {
            fd_to_kick = it->second.getFd();
            break ;
        }
    }
    if (fd_to_kick == 0)
    {
        serv_msg = ":localhost 441 " + client.getNick() + " " + user_to_kick + " #" + chan + " :They aren't on that channel\r\n";
        if (send(client.getFd(), serv_msg.c_str(), serv_msg.size(), 0) < 1)
                return (false);
        return (1);
    }
    
    if (!_channels[chan].isOperator(client.getFd()))
	{
		serv_msg = ":localhost 482 #" + chan + " :You're not channel operator\r\n";
		if (send(client.getFd(), serv_msg.c_str(), strlen(serv_msg.c_str()), 0) < 1)
            return (false);
        return (1);
	}
    
    serv_msg = ":" + client.getNick() + "!" + client.getUsername() + "@localhost KICK #" + chan + " " + user_to_kick + " :\r\n";
    for (std::vector<int>::iterator ite = _channels[chan].getUsers().begin(); ite != _channels[chan].getUsers().end(); ite++)
    {
            if (send(*ite, serv_msg.c_str(), serv_msg.size(), 0) < 1)
                return (false);
    }
    for (std::vector<int>::iterator ite = _channels[chan].getOperators().begin(); ite != _channels[chan].getOperators().end(); ite++)
    {
            if (send(*ite, serv_msg.c_str(), serv_msg.size(), 0) < 1)
                return (false);
    }
    kick_from_a_chan(chan, fd_to_kick);
    return (1);
}

// void Server::print_chan(int i)
// {
//     std::cout << "normal chan = ";
//     for (std::vector<std::string>::iterator it = this->users[i].channel_normal.begin(); it != this->users[i].channel_normal.end(); it++)
//         std::cout << *it << std::endl;
//     std::cout << "operator chan = ";
//     for (std::vector<std::string>::iterator it = this->users[i].channel_operators.begin(); it != this->users[i].channel_operators.end(); it++)
//         std::cout << *it << std::endl;
// }


// void Server::kick_from_a_chan(std::string &user_to_kick, std::string &chan)
// {
//     int i;
//     std::map<std::string, User>::iterator it_user = this->channels[chan].normal_users.find(user_to_kick);
//     if (it_user != this->channels[chan].normal_users.end())
//     {
//         i = it_user->second.socket_fd;
//         std::cout << "i = " << i << std::endl;
//         this->channels[chan].normal_users.erase(it_user);
//     }
//     else
//     {
//         it_user = this->channels[chan].operators.find(user_to_kick);
//         i = it_user->second.socket_fd;
//         std::cout << "i = " << i << std::endl;
//         if (it_user != this->channels[chan].operators.end())
//             this->channels[chan].operators.erase(it_user);
//     }
//     print_chan(i);
//     std::vector<std::string>::iterator it = std::find(this->users[i].channel_normal.begin(), this->users[i].channel_normal.end(), chan);
//     std::cout << "test\n";
//     if (it != this->users[i].channel_normal.end())
//     {
//         this->users[i].channel_normal.erase(it);
//         std::cout << "test2\n";
//     }
//     else
//     {
//         std::cout << "test3\n";
//         it = std::find(this->users[i].channel_operators.begin(), this->users[i].channel_operators.end(), chan);
//         this->users[i].channel_operators.erase(it);
//         std::cout << "test4\n";
//     }
// }

// // /connect localhost 6667 aaa

// int Server::kick(int i, char *msg)
// {
// 	if (!strncmp(msg, "KICK #", 6))
// 	{
//         (void)i;
//         std::string str_msg = msg;
// 		std::string serv_msg;
// 		std::string chan = str_msg.substr(6, str_msg.find(" ", 6) - 6);
// 		std::string user_to_kick = str_msg.substr(7 + chan.size(), std::string::npos);
// 		user_to_kick.erase(user_to_kick.size() - 4, 4);
//         //std::cout << "chan =" << chan << "|user to kick =" << user_to_kick << "|\n";
// 		if (!is_operator(i, chan))
// 		{
// 			serv_msg = ":localhost 482 #" + chan + " :You're not channel operator\r\n";
// 			status = send(this->users[i].socket_fd, serv_msg.c_str(), strlen(serv_msg.c_str()), 0);
// 				if (status == -1)
// 					std::cout << "[Server] Send error to client fd " << this->users[i].socket_fd << ": " << strerror(errno) << std::endl;
// 			return (1);
// 		}
// 		serv_msg = ":" + this->users[i].nick + "!" + this->users[i].username + "@localhost KICK #" + chan + " " + user_to_kick + " :\r\n";
// 		for (int j = 1; j < this->get_poll_count(); j++)
// 		{	
// 			if (this->users[j].is_is_chan(chan))
// 			{
// 				status = send(this->users[j].socket_fd, serv_msg.c_str(), strlen(serv_msg.c_str()), 0);
// 				if (status == -1)
// 					std::cout << "[Server] Send error to client fd " << this->users[j].socket_fd << ": " << strerror(errno) << std::endl;
// 			}
// 		}
//         kick_from_a_chan(user_to_kick, chan);
// 		return (1);
// 	}
// 	return (0);
// }
