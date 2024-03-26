#include "inc/server.hpp"


// << MODE zoldxx +i
// >> :zoldxx MODE zoldxx :+iH

bool	Server::mode(User client, std::string cmd)
{
    int status;
    std::string serv_msg = ":" + client.getNick() + " MODE " + client.getNick() + " :+iH" + "\r\n";
    status = send(client.getFd(), serv_msg.c_str(), strlen(serv_msg.c_str()), 0);
    if (status == -1)
        std::cout << "[Server] Send error to client " << client.getFd() << strerror(errno) << std::endl;
    return (1);
}

// int Server::mode(int i, char *msg)
// {
//      if (!strncmp(msg, "MODE ", 5))
//      {
//         std::string serv_msg = ":" + this->users[i].nick + " MODE " + this->users[i].nick + " :+iH" + "\r\n";
//         status = send(this->poll_fds[i].fd, serv_msg.c_str(), strlen(serv_msg.c_str()), 0);
//         if (status == -1)
//             std::cout << "[Server] Send error to client " << this->poll_fds[i].fd << strerror(errno) << std::endl;
//         return (1);
//      }
//      return (0);
// }
