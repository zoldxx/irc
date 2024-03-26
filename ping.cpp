#include "inc/server.hpp"

bool	Server::ping(User &client, std::string cmd)
{
    (void) cmd;
    int status;
    std::string serv_msg = "PONG localhost\r\n";
    status = send(client.getFd(), serv_msg.c_str(), strlen(serv_msg.c_str()), 0);
    if (status == -1)
        std::cout << "[Server] Send error to client " << client.getFd() << strerror(errno) << std::endl;
    return (1);
}


// int Server::ping(int i, char *msg)
// {
//      if (!strncmp(msg, "PING ", 5))
//      {
//         std::string serv_msg = "PONG localhost\r\n";
//         status = send(this->poll_fds[i].fd, serv_msg.c_str(), strlen(serv_msg.c_str()), 0);
//         if (status == -1)
//             std::cout << "[Server] Send error to client " << this->poll_fds[i].fd << strerror(errno) << std::endl;
//         return (1);
//      }
//      return (0);
// }

