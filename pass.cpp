#include "inc/server.hpp"

// bool	pass(User client, std::string cmd)
// {
// 	if (cmd.find(" ", 0) == std::string::npos)
// 	{	
// 		if (cmd != this->mdp)
// 		{
// 		    serv_msg = "Wrong password\r\n";
// 		    status = send(this->poll_fds[i].fd, serv_msg.c_str(), strlen(serv_msg.c_str()), 0);
// 		    close(this->poll_fds[i].fd);
// 		    this->del_from_poll_fds(i);
// 		    return (1);
// 		}
// 	}
// 	serv_msg = "Password accepted.\r\n";
// 	status = send(this->poll_fds[i].fd, serv_msg.c_str(), strlen(serv_msg.c_str()), 0);
// 	if (status < 1)
// 	{
// 		    close(this->poll_fds[i].fd);
// 		    this->del_from_poll_fds(i);
// 		    return (1);
// 	}
// 	return 0;
// }
