#include "irc.hpp"

int Server::first_message(int i, char *msg)
{
    std::string str_msg = msg;
    std::string serv_msg;
    std::string line_pswd;
    std::string pswd;
    std::string nick;

    if (!strncmp(str_msg.c_str(), "CAP LS\r\n", 8))
    {
        line_pswd = extract(str_msg, "\n", "\n");
        pswd = extract(line_pswd, " ", "\0");
        this->users[i].socket_fd = this->poll_fds[i].fd;

        if (pswd != this->mdp)
        {
            std::cout << "Connection fail: Wrong password\n";
            serv_msg = "Wrong password\r\n";
            status = send(this->poll_fds[i].fd, serv_msg.c_str(), strlen(serv_msg.c_str()), 0);
            if (status == -1)
                std::cout << "aaa[Server] Send error to client " << this->poll_fds[i].fd << strerror(errno) << std::endl;
            close(this->poll_fds[i].fd);
            this->del_from_poll_fds(i);
            return (1);
        }

        nick = str_msg.substr(20 + this->mdp.size(), str_msg.find("\r\n", 20 + this->mdp.size()) - this->mdp.size() - 20);
        if (!is_valid_nick(nick))
		{
         serv_msg = ":localhost 432 " + nick + " :Erroneous nickname\r\n";
         status = send(this->users[i].socket_fd, serv_msg.c_str(), strlen(serv_msg.c_str()), 0);
			if (status == -1)
				std::cout << "bbb[Server] Send error to client fd " << this->users[i].socket_fd << ": " << strerror(errno) << std::endl;
         return (1);
		}

        this->users[i].nick = str_msg.substr(20 + this->mdp.size(), str_msg.find("\r\n", 20 + this->mdp.size()) - this->mdp.size() - 20);
        this->users[i].username = str_msg.substr(27 + this->users[i].nick.size() + this->mdp.size(),
                    str_msg.find(" ", 27 + this->users[i].nick.size() +this->mdp.size()) - this->users[i].nick.size() - this->mdp.size() - 27);
        serv_msg = "Password accepted.\r\n001 " + this->users[i].nick + " Welcome to 42 serv " + this->users[i].nick + "!~" + this->users[i].username + "@127.0.0.1\n";
        status = send(this->poll_fds[i].fd, serv_msg.c_str(), strlen(serv_msg.c_str()), 0);
        if (status == -1)
            std::cout << "ccc[Server] Send error to client " << this->poll_fds[i].fd << strerror(errno) << std::endl;
        return (1);
    }
    return (0);
}

// int Server::first_message(int i, char *msg)
// {
//     std::string str_msg = msg;
//     std::string serv_msg;
//     std::string line_pswd;
//     std::string pswd;

//     if (!strncmp(str_msg.c_str(), "CAP LS\r\n", 8))
//     {
//         line_pswd = extract(str_msg, "\n", "\n");
//         pswd = extract(line_pswd, " ", "\0");
//         //std::cout << "serv mdp =" << this->mdp << " pswd =" << pswd << std::endl;
//         if (pswd != this->mdp)
//         {
//             std::cout << "Connection fail: Wrong password\n";
//             serv_msg = "Wrong password\r\n";
//             status = send(this->poll_fds[i].fd, serv_msg.c_str(), strlen(serv_msg.c_str()), 0);
//             if (status == -1)
//                 std::cout << "[Server] Send error to client " << this->poll_fds[i].fd << strerror(errno) << std::endl;
//             close(this->poll_fds[i].fd);
//             this->del_from_poll_fds(i);
//             return (1);
//         }
//         this->users[i].nick = str_msg.substr(20 + this->mdp.size(), str_msg.find("\r\n", 20 + this->mdp.size()) - this->mdp.size() - 20);
//         this->users[i].username = str_msg.substr(27 + this->users[i].nick.size() + this->mdp.size(),
//                     str_msg.find(" ", 27 + this->users[i].nick.size() +this->mdp.size()) - this->users[i].nick.size() - this->mdp.size() - 27);
//         this->users[i].is_ops = 0;
//         this->users[i].socket_fd = this->poll_fds[i].fd;
//         // std::cout << "nickname = " << this->users[i].nick << "|" << std::endl;
//         // std::cout << "username = " << this->users[i].username << "|" << std::endl;
//         serv_msg = "Password accepted.\r\n001 " + this->users[i].nick + " Welcome to 42 serv " + this->users[i].nick + "!~" + this->users[i].username + "@127.0.0.1\n";
//         status = send(this->poll_fds[i].fd, serv_msg.c_str(), strlen(serv_msg.c_str()), 0);
//         if (status == -1)
//             std::cout << "[Server] Send error to client " << this->poll_fds[i].fd << strerror(errno) << std::endl;
//         return (1);
//     }
//     return (0);
// }

// int Server::first_message(int i, char *msg)
// {
//     std::string str_msg = msg;
//     std::string serv_msg;
//     if (!strncmp(str_msg.c_str(), "CAP LS\r\n", 8))
//     {
//         this->users[i].nick = str_msg.substr(13, str_msg.find("\r\n", 14) - 13);
//         this->users[i].username = str_msg.substr(20 + this->users[i].nick.size(), str_msg.find(" ", 20 + this->users[i].nick.size()) - this->users[i].nick.size() - 20);
//         this->users[i].socket_fd = i;
//         this->users[i].is_ops = 0;
//         // std::cout << "nickname = " << this->users[i].nick << "|" << std::endl;
//         // std::cout << "username = " << this->users[i].username << "|" << std::endl;
//         serv_msg = "001 " + this->users[i].nick + " Welcome to 42 serv " + this->users[i].nick + "!~" + this->users[i].username + "@127.0.0.1\n";
//         status = send(this->poll_fds[i].fd, serv_msg.c_str(), strlen(serv_msg.c_str()), 0);
//         if (status == -1)
//             std::cout << "[Server] Send error to client " << this->poll_fds[i].fd << strerror(errno) << std::endl;
//         return (1);
//     }
//     return (0);
// }