#include "irc.hpp"


std::string extract(const std::string& chaine, std::string begin, std::string end) 
{
    std::size_t debut = chaine.find(begin);
    std::size_t fin = chaine.find(end, debut + 1) - 1 ;
    if (debut != std::string::npos && fin != std::string::npos) 
        return chaine.substr(debut + 1, fin - debut - 1);
    else 
        return "";
}

int Server::del_user(int i)
{
    for (std::vector<std::string>::iterator it = this->users[i].channel_normal.begin(); it != this->users[i].channel_normal.end(); it++)
    {
        std::map<std::string, User>::iterator it_user = this->channels[*it].normal_users.find(this->users[i].nick);
        this->channels[*it].normal_users.erase(it_user);
    }
    for (std::vector<std::string>::iterator it = this->users[i].channel_operators.begin(); it != this->users[i].channel_operators.end(); it++)
    {
        std::map<std::string, User>::iterator it_user = this->channels[*it].operators.find(this->users[i].nick);
        this->channels[*it].operators.erase(it_user);
    }
    std::map<int, User>::iterator it_user = this->users.find(i);
    this->users.erase(it_user);
    this->users[i].channel_normal.clear();
    this->users[i].channel_operators.clear();
    return (i);
}

void Server::read_data_from_socket(int i)
{
    char buffer[512];
    int bytes_read;

    memset(&buffer, '\0', sizeof buffer);
    bytes_read = recv(this->poll_fds[i].fd, buffer, BUFSIZ, 0);
    if (bytes_read <= 0) 
    {
        if (bytes_read == 0)
            std::cout << "[" << this->poll_fds[i].fd << "] " << "Client socket closed connection." << std::endl;
        else 
            std::cout << "[Server] Recv error: " << strerror(errno) << std::endl;
        this->del_user(i);
        close(this->poll_fds[i].fd);
        this->del_from_poll_fds(i);
    }
    else
    {
        if (this->is_special_message(i, buffer))
          return ;
    }
}


int Server::is_special_message(int i, char *msg)
{
    if (this->first_message(i, msg))
        return (1);
    else if (this->join_cmd(i, msg))
        return (1);
    else if (this->set_nick(i, msg))
        return (1);
    else if (this->mode(i, msg))
        return (1);
    else if (this->ping(i, msg))
        return (1);
    else if (this->privmsg(i, msg))
        return (1);
    else if (this->topic_cmd(i, msg))
        return (1);
    if (!strncmp(msg, "QUIT ", 5))
        return (1);
    return (0);
}




// void Server::read_data_from_socket(int i)
// {
//     char buffer[BUFSIZ];
//     std::string msg_to_send;
//     int bytes_read;
//     int status;
//     int dest_fd;
//     int sender_fd;

//     sender_fd = this->poll_fds[i].fd;
//     memset(&buffer, '\0', sizeof buffer);
//     bytes_read = recv(sender_fd, buffer, BUFSIZ, 0);
//     if (bytes_read <= 0) 
//     {
//         if (bytes_read == 0)
//             std::cout << "[" << sender_fd << "] " << "Client socket closed connection." << std::endl;
//         else 
//             std::cout << "[Server] Recv error: " << strerror(errno) << std::endl;
//         close(sender_fd);
//         this->del_from_poll_fds(i);
//     }
//     else
//     {
//         if (this->is_special_message(i, buffer))
//           return ;
//         std::string buf = buffer;
//         std::string msg = buf.substr(buf.find(":") - 2, std::string::npos);
//         std::string channel_name = buf.substr(buf.find("#", 0) + 1, buf.find(":", 2) - 1);
//         std::cout << "channel_name = " << channel_name << std::endl;
//         std::cout << "[" << sender_fd << "]" << " Got message: " << msg;
//         msg_to_send = ":" + this->users[i].nick + "!~" + this->users[i].username + "@localhost PRIVMSG " + channel_name + msg;
//         for (int j = 0; j < this->poll_count; j++)
//         {
//             dest_fd = this->poll_fds[j].fd;
//             if (dest_fd != server_socket && dest_fd != sender_fd)
//             {
//                 status = send(dest_fd, msg_to_send.c_str(), strlen(msg_to_send.c_str()), 0);
//                 if (status == -1)
//                     std::cout << "[Server] Send error to client fd " << dest_fd << ": " << strerror(errno) << std::endl;
//             }
//         }
//     }
// }

// >> :testeur!~dberreby@8e5-aefa-5668-68dd-4f69.210.62.ip PRIVMSG #asdfg :salut

// void Server::read_data_from_socket(int i)
// {
//     char buffer[BUFSIZ];
//     std::string msg_to_send;
//     int bytes_read;
//     int status;
//     int dest_fd;
//     int sender_fd;

//     sender_fd = this->poll_fds[i].fd;
//     memset(&buffer, '\0', sizeof buffer);
//     bytes_read = recv(sender_fd, buffer, BUFSIZ, 0);
//     if (bytes_read <= 0) 
//     {
//         if (bytes_read == 0)
//             std::cout << "[" << sender_fd << "] " << "Client socket closed connection." << std::endl;
//         else 
//             std::cout << "[Server] Recv error: " << strerror(errno) << std::endl;
//         close(sender_fd);
//         this->del_from_poll_fds(i);
//     }
//     else
//     {
//         if (this->is_special_message(i, buffer))
//           return ;
//         std::cout << "fd " << sender_fd << " Got message: " << buffer;
//         std::stringstream ss;
//         ss << sender_fd;
//         std::string str = ss.str();
//         msg_to_send = "[" + str + "] " + "says: " + buffer ;
//         for (int j = 0; j < this->poll_count; j++)
//         {
//             dest_fd = this->poll_fds[j].fd;
//             if (dest_fd != server_socket && dest_fd != sender_fd)
//             {
//                 status = send(dest_fd, msg_to_send.c_str(), strlen(msg_to_send.c_str()), 0);
//                 if (status == -1)
//                     std::cout << "[Server] Send error to client fd " << dest_fd << ": " << strerror(errno) << std::endl;
//             }
//         }
//     }
// }