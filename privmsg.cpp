/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blerouss <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 17:04:44 by blerouss          #+#    #+#             */
/*   Updated: 2024/03/22 17:04:51 by blerouss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/irc.hpp"

int Server::privmsg(int i, char *msg)
{
     if (!strncmp(msg, "PRIVMSG ", 8))
     {
        std::string buf = msg;
        std::string msg_str = extract(buf, ":", "\0");
        std::string channel_name = extract(buf, "#", ":") ;
        std::string msg_to_send = ":" + this->users[i].nick + "!~" + this->users[i].username + "@localhost PRIVMSG #" + channel_name + " :" + msg_str;
        for (int j = 1; j < this->get_poll_count(); j++)
        {
            if (j != i && this->users[j].is_is_chan(channel_name))
            {
                status = send(this->users[j].socket_fd, msg_to_send.c_str(), strlen(msg_to_send.c_str()), 0);
                if (status == -1)
                    std::cout << "[Server] Send error to client fd " << this->users[j].socket_fd << ": " << strerror(errno) << std::endl;
            }
        }
        return (1);
     }
     return (0);
}

// int Server::privmsg(int i, char *msg)
// {
//      if (!strncmp(msg, "PRIVMSG ", 8))
//      {
//         //int sender_fd = this->poll_fds[i].fd;
//         std::string buf = msg;
//         std::string msg_str = extract(buf, ":", "\0");
//         std::string channel_name = extract(buf, "#", ":") ;
//         std::cout << "channel name =" << channel_name << "|" << std::endl;
//         std::string msg_to_send = ":" + this->users[i].nick + "!~" + this->users[i].username + "@localhost PRIVMSG #" + channel_name + " :" + msg_str;
//         //std::cout << "message from [" << sender_fd << "] : " << msg_str;
//         for (int j = 1; j < this->get_poll_count(); j++)
//         {
//             if (j != i && this->users[j].is_is_chan(channel_name))
//             {
//                 std::cout << "message de base :" << msg << "|" << std::endl;
//                 std::cout << "on send :" <<  msg_to_send << "|" << std::endl;
//                 // std::cout << "sender (celui qui viens de join) = " << i << "son fd = " << this->users[i].socket_fd << std::endl;
//                 // std::cout << "celui a qui on va envoyer le message = " << j << "son fd = " << this->users[j].socket_fd << std::endl;
//                 status = send(this->users[j].socket_fd, msg_to_send.c_str(), strlen(msg_to_send.c_str()), 0);
//                 if (status == -1)
//                     std::cout << "[Server] Send error to client fd " << this->users[j].socket_fd << ": " << strerror(errno) << std::endl;
//             }
//         }
//         return (1);
//      }
//      return (0);
// }

// int Server::privmsg(int i, char *msg)
// {
//      if (!strncmp(msg, "PRIVMSG ", 8))
//      {
//         int sender_fd = this->poll_fds[i].fd;
//         std::string buf = msg;
//         std::string msg_str = extract(buf, ":", "\0");
//         std::string channel_name = extract(buf, "#", ":") ;
//         std::string msg_to_send = ":" + this->users[i].nick + "!~" + this->users[i].username + "@localhost PRIVMSG #" + channel_name + " :" + msg_str;
//         std::cout << "message from [" << sender_fd << "] : " << msg_str;
//         for (int j = 0; j < this->poll_count; j++)
//         {
//             int dest_fd = this->users[j].socket_fd;
//             if (dest_fd != server_socket && dest_fd != sender_fd && this->users[j].is_is_chan(channel_name))
//             {
//                 status = send(dest_fd, msg_to_send.c_str(), strlen(msg_to_send.c_str()), 0);
//                 if (status == -1)
//                     std::cout << "[Server] Send error to client fd " << dest_fd << ": " << strerror(errno) << std::endl;
//             }
//         }
//         return (1);
//      }
//      return (0);
// }

// int Server::privmsg(int i, char *msg)
// {
//      if (!strncmp(msg, "PRIVMSG ", 8))
//      {
//         int sender_fd = this->poll_fds[i].fd;
//         std::string buf = msg;
//         std::string msg = extract(buf, ":", "\0");
//         std::string channel_name = extract(buf, "#", ":") ;
//         std::string msg_to_send = ":" + this->users[i].nick + "!~" + this->users[i].username + "@localhost PRIVMSG #" + channel_name + " :" + msg;
//         std::cout << "message from [" << sender_fd << "] : " << msg;
//         for (int j = 0; j < this->poll_count; j++)
//         {
//             int dest_fd = this->poll_fds[j].fd;
//             if (dest_fd != server_socket && dest_fd != sender_fd && this->users[j].is_is_chan(channel_name))
//             {
//                 status = send(dest_fd, msg_to_send.c_str(), strlen(msg_to_send.c_str()), 0);
//                 if (status == -1)
//                     std::cout << "[Server] Send error to client fd " << dest_fd << ": " << strerror(errno) << std::endl;
//             }
//         }
//         return (1);
//      }
//      return (0);
// }

// int Server::privmsg(int i, char *msg)
// {
//      if (!strncmp(msg, "PRIVMSG ", 8))
//      {
//         int sender_fd = this->poll_fds[i].fd;
//         std::string buf = msg;
//         std::string msg = extract(buf, ":", "\0");
//         std::string channel_name = extract(buf, "#", ":") ;
//         std::string msg_to_send = ":" + this->users[i].nick + "!~" + this->users[i].username + "@localhost PRIVMSG #" + channel_name + " :" + msg;
//         std::cout << "message from [" << sender_fd << "] : " << msg;
//         for (int j = 0; j < this->poll_count; j++)
//         {
//             int dest_fd = this->poll_fds[j].fd;
//             if (dest_fd != server_socket && dest_fd != sender_fd)
//             {
//                 status = send(dest_fd, msg_to_send.c_str(), strlen(msg_to_send.c_str()), 0);
//                 if (status == -1)
//                     std::cout << "[Server] Send error to client fd " << dest_fd << ": " << strerror(errno) << std::endl;
//             }
//         }
//         return (1);
//      }
//      return (0);
// }
