#include "inc/server.hpp"
#include "inc/irc.hpp"

bool	Server::privmsg(User &client, std::string cmd)
{
    int status;
    std::string msg_str = extract(cmd, ":", "\0");
    std::string channel_name = extract(cmd, "#", ":");
    std::string msg_to_send = ":" + _users[client.getFd()].getNick() + "!~" + _users[client.getFd()].getUsername() + "@localhost PRIVMSG #" + channel_name + " :" + msg_str;
    for (int j = 1; j < this->get_poll_count(); j++)
    {
        if (_channels[channel_name].isInChan(_users[j].getFd()) && client.getFd() != _users[j].getFd())
        {
            status = send(_users[j].getFd(), msg_to_send.c_str(), strlen(msg_to_send.c_str()), 0);
            if (status == -1)
                std::cout << "[Server] Send error to client fd " << _users[j].getFd() << ": " << strerror(errno) << std::endl;
        }
    }
    return (1);
}

// int Server::privmsg(int i, char *msg)
// {
//      if (!strncmp(msg, "PRIVMSG ", 8))
//      {
//         std::string buf = msg;
//         std::string msg_str = extract(buf, ":", "\0");
//         std::string channel_name = extract(buf, "#", ":") ;
//         std::string msg_to_send = ":" + this->users[i].nick + "!~" + this->users[i].username + "@localhost PRIVMSG #" + channel_name + " :" + msg_str;
//         for (int j = 1; j < this->get_poll_count(); j++)
//         {
//             if (j != i && this->users[j].is_is_chan(channel_name))
//             {
//                 status = send(this->users[j].socket_fd, msg_to_send.c_str(), strlen(msg_to_send.c_str()), 0);
//                 if (status == -1)
//                     std::cout << "[Server] Send error to client fd " << this->users[j].socket_fd << ": " << strerror(errno) << std::endl;
//             }
//         }
//         return (1);
//      }
//      return (0);
// }

