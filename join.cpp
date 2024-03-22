/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blerouss <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 17:02:44 by blerouss          #+#    #+#             */
/*   Updated: 2024/03/22 17:02:50 by blerouss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/irc.hpp"

void Server::fill_join_msg(std::string &serv_msg, std::string &channel_name, int i)
{
    serv_msg += ":" + this->users[i].nick + "!~" + this->users[i].username + " JOIN :#" + channel_name + "\r\n";
    if (this->channels[channel_name].topic.size() == 0)
        serv_msg += ":localhost 331 " + this->users[i].nick + " #" + channel_name + " :No topic is set\r\n";
    else
        serv_msg += ":localhost 332 " + this->users[i].nick + " #" + channel_name + " " + this->channels[channel_name].topic + "\r\n";
    serv_msg += ":localhost 353 " + this->users[i].nick + " = #" + channel_name + " :";
    for (std::map<std::string, User>::iterator it = channels[channel_name].operators.begin(); it != channels[channel_name].operators.end(); it++)
        serv_msg += "@" + it->second.nick + " ";
    for (std::map<std::string, User>::iterator it = channels[channel_name].normal_users.begin(); it != channels[channel_name].normal_users.end(); it++)
    {
        serv_msg += it->second.nick;
        //if (std::next(it) != channels[channel_name].normal_users.end())
            serv_msg += " ";
    }
    serv_msg += "\r\n:localhost 366 " + this->users[i].nick + " #" + channel_name + " :End of /NAMES list\r\n";  
}

int Server::join_cmd(int i, char *msg)
{
    if (!strncmp(msg, "JOIN #", 6))
    {
        std::string serv_msg;
        std::string str_msg = msg;
        std::string channel_name = str_msg.substr(6, std::string::npos);
        channel_name.erase(channel_name.size() - 2, 2);
        
        if (this->channels[channel_name].normal_users.size() == 0 && this->channels[channel_name].operators.size() == 0)
        {
            serv_msg = ":localhost 403 #" + channel_name + " :No such channel\r\n";
            this->channels[channel_name].operators[this->users[i].nick] = this->users[i];
            this->users[i].channel_operators.push_back(channel_name);
        }
        else
        {
            this->channels[channel_name].normal_users[this->users[i].nick] = this->users[i];
            this->users[i].channel_normal.push_back(channel_name);
        }
        fill_join_msg(serv_msg, channel_name, i);
        std::string msg_to_send = ":" + this->users[i].nick + "!~" + this->users[i].username + " JOIN :#" + channel_name + "\r\n";

        for (int j = 1; j < this->get_poll_count(); j++)
        {
            if (this->users[j].is_is_chan(channel_name) && i != j)
            {
                status = send(this->users[j].socket_fd, msg_to_send.c_str(), strlen(msg_to_send.c_str()), 0);
                if (status == -1)
                    std::cout << "[Server] Send error to client fd " << this->users[j].socket_fd << ": " << strerror(errno) << std::endl;
            }
            else if (this->users[j].is_is_chan(channel_name) && i == j)
            {
                status = send(this->users[j].socket_fd, serv_msg.c_str(), strlen(serv_msg.c_str()), 0);
                if (status == -1)
                    std::cout << "[Server] Send error to client fd " << this->users[j].socket_fd << ": " << strerror(errno) << std::endl;
            }
        }
        return (1);
    }
    return (0);
}

// void Server::fill_join_msg(std::string &serv_msg, std::string &channel_name, int i)
// {
//     serv_msg += ":" + this->users[i].nick + "!~" + this->users[i].username + " JOIN :#" + channel_name + "\r\n";
//     if (this->channels[channel_name].topic.size() == 0)
//         serv_msg += ":localhost 331 " + this->users[i].nick + " #" + channel_name + " :No topic is set\r\n";
//     else
//         serv_msg += ":localhost 332 " + this->users[i].nick + " #" + channel_name + " " + this->channels[channel_name].topic + "\r\n";
//     serv_msg += ":localhost 353 " + this->users[i].nick + " = #" + channel_name + " :";
//     for (std::vector<User>::iterator it = channels[channel_name].operators.begin(); it != channels[channel_name].operators.end(); it++)
//         serv_msg += "@" + it->nick + " ";
//     for (std::vector<User>::iterator it = channels[channel_name].normal_users.begin(); it != channels[channel_name].normal_users.end(); it++)
//     {
//         serv_msg += it->nick;
//         if (it + 1 != channels[channel_name].operators.end())
//             serv_msg += " ";
//     }
//     serv_msg += "\r\n:localhost 366 " + this->users[i].nick + " #" + channel_name + " :End of /NAMES list\r\n";  
// }

// int Server::join_cmd(int i, char *msg)
// {
//     if (!strncmp(msg, "JOIN #", 6))
//     {
//         std::string serv_msg;
//         std::string str_msg = msg;
//         std::string channel_name = str_msg.substr(6, std::string::npos);
//         channel_name.erase(channel_name.size() - 2, 2);
        
//         if (this->channels[channel_name].all_users.size() == 0)
//         {
//             serv_msg = ":localhost 403 #" + channel_name + " :No such channel\r\n";
//             this->channels[channel_name].operators.push_back(this->users[i]);
//             this->users[i].channel_operators.push_back(channel_name);
//         }
//         else
//         {
//             this->channels[channel_name].normal_users.push_back(this->users[i]);
//             this->users[i].channel_normal.push_back(channel_name);
//         }
//         this->channels[channel_name].all_users.push_back(this->users[i]);

//         fill_join_msg(serv_msg, channel_name, i);
//         std::string msg_to_send = ":" + this->users[i].nick + "!~" + this->users[i].username + " JOIN :#" + channel_name + "\r\n";

//         for (int j = 1; j < this->get_poll_count(); j++)
//         {
//             if (this->users[j].is_is_chan(channel_name) && i != j)
//             {
//                 status = send(this->users[j].socket_fd, msg_to_send.c_str(), strlen(msg_to_send.c_str()), 0);
//                 if (status == -1)
//                     std::cout << "[Server] Send error to client fd " << this->users[j].socket_fd << ": " << strerror(errno) << std::endl;
//             }
//             else if (this->users[j].is_is_chan(channel_name) && i == j)
//             {
//                 status = send(this->users[j].socket_fd, serv_msg.c_str(), strlen(serv_msg.c_str()), 0);
//                 if (status == -1)
//                     std::cout << "[Server] Send error to client fd " << this->users[j].socket_fd << ": " << strerror(errno) << std::endl;
//             }
//         }
//         return (1);
//     }
//     return (0);
// }

// void Server::fill_join_msg(std::string &serv_msg, std::string &channel_name, int i)
// {
//     serv_msg += ":" + this->users[i].nick + "!~" + this->users[i].username + " JOIN :#" + channel_name + "\r\n";
//     if (this->channels[channel_name].topic.size() == 0)
//         serv_msg += ":localhost 331 " + this->users[i].nick + " #" + channel_name + " :No topic is set\r\n";
//     else
//         serv_msg += ":localhost 332 " + this->users[i].nick + " #" + channel_name + this->channels[channel_name].topic + "\r\n";
//     serv_msg += ":localhost 353 " + this->users[i].nick + " = #" + channel_name + " :";
//     for (std::vector<std::string>::iterator it = channels[channel_name].operators.begin(); it != channels[channel_name].operators.end(); it++)
//     {
//         serv_msg += "@" + *it;
//         if (it + 1 != channels[channel_name].operators.end())
//             serv_msg += " ";
//     }
//     for (std::vector<std::string>::iterator it = channels[channel_name].normal_users.begin(); it != channels[channel_name].normal_users.end(); it++)
//     {
//         serv_msg += *it;
//         if (it + 1 != channels[channel_name].operators.end())
//             serv_msg += " ";
//     }
//     serv_msg += "\r\n:localhost 366 " + this->users[i].nick + " #" + channel_name + " :End of /NAMES list\r\n";  
// }

// int Server::join_cmd(int i, char *msg)
// {
//     if (!strncmp(msg, "JOIN #", 6))
//     {
//         std::string serv_msg;
//         std::string str_msg = msg;
//         std::string channel_name = str_msg.substr(6, std::string::npos);
//         channel_name.erase(channel_name.size() - 2, 2);
        
//         if (this->channels[channel_name].all_users.size() == 0)
//         {
//             serv_msg = ":localhost 403 #" + channel_name + " :No such channel\r\n";
//             this->channels[channel_name].operators.push_back(this->users[i].nick);
//             this->users[i].channel_operators.push_back(channel_name);
//         }
//         else
//             this->users[i].channel_normal.push_back(channel_name);
//         this->channels[channel_name].all_users.push_back(this->users[i].nick);

//         fill_join_msg(serv_msg, channel_name, i);
//         std::string msg_to_send = ":" + this->users[i].nick + "!~" + this->users[i].username + " JOIN :#" + channel_name + "\r\n";

//         for (int j = 1; j < this->get_poll_count(); j++)
//         {
//             if (this->users[j].is_is_chan(channel_name) && i != j)
//             {
//                 status = send(this->users[j].socket_fd, msg_to_send.c_str(), strlen(serv_msg.c_str()), 0);
//                 if (status == -1)
//                     std::cout << "[Server] Send error to client fd " << this->users[j].socket_fd << ": " << strerror(errno) << std::endl;
//             }
//             else if (this->users[j].is_is_chan(channel_name) && i == j)
//             {
//                 status = send(this->users[j].socket_fd, serv_msg.c_str(), strlen(serv_msg.c_str()), 0);
//                 if (status == -1)
//                     std::cout << "[Server] Send error to client fd " << this->users[j].socket_fd << ": " << strerror(errno) << std::endl;
//             }
//         }
//         return (1);
//     }
//     return (0);
// }

// int Server::join_cmd(int i, char *msg)
// {
//     if (!strncmp(msg, "JOIN #", 6))
//     {
//         std::string serv_msg;
//         std::string str_msg = msg;
//         std::string channel_name = str_msg.substr(6, std::string::npos);
//         channel_name.erase(channel_name.size() - 2, 2);
//         if (this->channels[channel_name].all_users.size() == 0)
//         {
//             serv_msg = ":localhost 403 #" + channel_name + " :No such channel\r\n";
//             this->channels[channel_name].operators.push_back(this->users[i]);
//             this->users[i].channel_operators.push_back(channel_name);
//         }
//         else
//             this->users[i].channel_normal.push_back(channel_name);
//         this->channels[channel_name].all_users.push_back(this->users[i]);
//         fill_join_msg(serv_msg, channel_name, i);
//         for (int j = 1; j < this->get_poll_count(); j++)
//         {
//             if (this->users[j].is_is_chan(channel_name))
//             {
//                 status = send(this->users[j].socket_fd, serv_msg.c_str(), strlen(serv_msg.c_str()), 0);
//                 if (status == -1)
//                     std::cout << "[Server] Send error to client fd " << this->users[j].socket_fd << ": " << strerror(errno) << std::endl;
//             }
//         }
//         return (1);
//     }
//     return (0);
// }


// int Server::join_cmd(int i, char *msg)
// {
//     if (!strncmp(msg, "JOIN #", 6))
//     {
//         std::string serv_msg;
//         std::string str_msg = msg;
//         std::string channel_name = str_msg.substr(6, std::string::npos);
//         channel_name.erase(channel_name.size() - 2, 2);
//         if (this->channels[channel_name].all_users.size() == 0)
//         {
//             serv_msg = ":localhost 403 #" + channel_name + " :No such channel\r\n";
//             this->channels[channel_name].operators.push_back(this->users[i]);
//             this->users[i].channel_operators.push_back(channel_name);
//         }
//         else
//             this->users[i].channel_normal.push_back(channel_name);
//         this->channels[channel_name].all_users.push_back(this->users[i]);
        
//         serv_msg += ":" + this->users[i].nick + "!~" + this->users[i].username + " JOIN :#" + channel_name + "\r\n";
//         for (int j = 1; j < this->get_poll_count(); j++)
//         {
//             if (this->users[j].is_is_chan(channel_name))
//             {
//                 // std::cout << "message de base :" << msg << "|" << std::endl;
//                 // std::cout << "on send :" <<  serv_msg << "|" << std::endl;
//                 // std::cout << "sender (celui qui viens de join) = " << i << "son fd = " << this->users[i].socket_fd << std::endl;
//                 // std::cout << "celui a qui on va envoyer le message = " << j << "son fd = " << this->users[j].socket_fd << std::endl;
//                 status = send(this->users[j].socket_fd, serv_msg.c_str(), strlen(serv_msg.c_str()), 0);
//                 if (status == -1)
//                     std::cout << "[Server] Send error to client fd " << this->users[j].socket_fd << ": " << strerror(errno) << std::endl;
//             }
//         }
//         // for (std::map<int, User>::iterator it = this->users.begin(); it != this->users.end(); it++)
//         // {
//         //     if (it->first != i && it->second.is_is_chan(channel_name))
//         //     {
//         //         std::cout << "fd = " << it->second.socket_fd << std::endl;
//         //         status = send(it->second.socket_fd, serv_msg.c_str(), strlen(serv_msg.c_str()), 0);
//         //         if (status == -1)
//         //             std::cout << "[Server] Send error to client fd " << it->second.socket_fd << ": " << strerror(errno) << std::endl;
//         //     }
//         // }
//         return (1);
//     }
//     return (0);
// }

// int Server::join_cmd(int i, char *msg)
// {
//     if (!strncmp(msg, "JOIN #", 6))
//     {
//         std::string str_msg = msg;
//         std::string channel_name = str_msg.substr(6, str_msg.find("\r\n"));
//         if (this->channels[channel_name].all_users.size() == 0)
//         {
//             this->channels[channel_name].operators.push_back(this->users[i]);
//             this->users[i].channel_operators.push_back(channel_name);
//         }
//         else
//             this->users[i].channel_normal.push_back(channel_name);
//         this->channels[channel_name].all_users.push_back(this->users[i]);
        
//         std::string serv_msg = ":" + this->users[i].nick + "!~" + this->users[i].username + " JOIN : #" + channel_name;
//         for (std::map<int, User>::iterator it = this->users.begin(); it != this->users.end(); it++)
//         {
//             if (it->first != i && it->second.is_is_chan(channel_name))
//             {
//                 std::cout << "fd = " << it->second.socket_fd << std::endl;
//                 status = send(it->second.socket_fd, serv_msg.c_str(), strlen(serv_msg.c_str()), 0);
//                 if (status == -1)
//                     std::cout << "[Server] Send error to client fd " << it->second.socket_fd << ": " << strerror(errno) << std::endl;
//             }
//         }
//         return (1);
//     }
//     return (0);
// }

// int Server::join_cmd(int i, char *msg)
// {
//     if (!strncmp(msg, "JOIN #", 6))
//     {
//         std::string str_msg = msg;
//         std::string channel_name = str_msg.substr(6, str_msg.find("\r\n"));
//         if (this->channels[channel_name].all_users.size() == 0)
//         {
//             this->channels[channel_name].operators.push_back(this->users[i]);
//             this->users[i].channel_operators.push_back(this->channels[channel_name]);
//         }
//         else
//             this->users[i].channel_normal.push_back(this->channels[channel_name]);
//         this->channels[channel_name].all_users.push_back(this->users[i]);
        
//         std::string serv_msg = ":" + this->users[i].nick + "!~" + this->users[i].username + " JOIN : #" + channel_name;
//         for (int j = 0; j < this->poll_count; j++)
//         {
//             int dest_fd = this->poll_fds[j].fd;
//             if (dest_fd != server_socket && this->users[j].is_is_chan(channel_name))
//             {
//                 status = send(dest_fd, serv_msg.c_str(), strlen(serv_msg.c_str()), 0);
//                 if (status == -1)
//                     std::cout << "[Server] Send error to client fd " << dest_fd << ": " << strerror(errno) << std::endl;
//             }
//         }
//         return (1);
//     }
//     return (0);
// }

// int Server::join_cmd(int i, char *msg)
// {
//     if (!strncmp(msg, "JOIN #", 6))
//     {
//         std::string str_msg = msg;
//         std::string channel_name = str_msg.substr(6, str_msg.find("\r\n"));
//         if (this->channels[channel_name].all_users.size() == 0)
//         {
//             this->channels[channel_name].operators.push_back(this->users[i]);
//             this->users[i].channel_operators.push_back(this->channels[channel_name]);
//         }
//         else
//             this->users[i].channel_normal.push_back(this->channels[channel_name]);
//         this->channels[channel_name].all_users.push_back(this->users[i]);
//         std::string serv_msg = ":" + this->users[i].nick + "!~" + this->users[i].username + " JOIN : #" + channel_name;
//         int status = send(this->poll_fds[i].fd, serv_msg.c_str(), strlen(serv_msg.c_str()), 0);
//         if (status == -1)
//             std::cout << "[Server] Send error to client " << this->poll_fds[i].fd << strerror(errno) << std::endl;
//         return (1);
//     }
//     return (0);
// }
