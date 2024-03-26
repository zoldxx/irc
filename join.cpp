#include "inc/server.hpp"

void Server::fill_join_msg(std::string &serv_msg, std::string &channel_name, int i)
{
    std::vector<int>::iterator it;
    serv_msg += ":" + _users[i].getNick() + "!~" + _users[i].getUsername() + " JOIN :#" + channel_name + "\r\n";
    if (_channels[channel_name].getTopic().size() == 0)
        serv_msg += ":localhost 331 " + _users[i].getNick() + " #" + channel_name + " :No topic is set\r\n";
    else
        serv_msg += ":localhost 332 " + _users[i].getNick() + " #" + channel_name + " " + _channels[channel_name].getTopic() + "\r\n";
    serv_msg += ":localhost 353 " + _users[i].getNick() + " = #" + channel_name + " :";
    if (_channels.find(channel_name) != _channels.end())
    {
        for (it = _channels[channel_name].getOperators().begin(); it != _channels[channel_name].getOperators().end(); it++)
            serv_msg += "@" + _users[*it].getNick() + " ";
        for (it = _channels[channel_name].getUsers().begin(); it != _channels[channel_name].getUsers().end(); it++)
        {
            serv_msg += _users[*it].getNick();
            //if (std::next(it) != channels[channel_name].normal_users.end())
                serv_msg += " ";
        }
    }
    serv_msg += "\r\n:localhost 366 " + _users[i].getNick() + " #" + channel_name + " :End of /NAMES list\r\n";  
}

bool Server::join(User client, std::string cmd)
{
    int status;
    std::string serv_msg;
    std::string channel_name = cmd.substr(1, std::string::npos);
    channel_name.erase(channel_name.size() - 2, 2);
    
    if (_channels[channel_name].getUsers().size() == 0)
    {
        serv_msg = ":localhost 403 #" + channel_name + " :No such channel\r\n";
        _channels[channel_name].getOperators().push_back(client.getFd());
    }
    _channels[channel_name].getUsers().push_back(client.getFd());
    client.getChannels().push_back(channel_name);
    fill_join_msg(serv_msg, channel_name, client.getFd());
    std::string msg_to_send = ":" + client.getNick() + "!~" + client.getUsername() + " JOIN :#" + channel_name + "\r\n";

    for (int j = 1; j < this->get_poll_count(); j++)
    {
        if (_channels[channel_name].isInChan(_users[j].getFd()) && client.getFd() != _users[j].getFd())
        {
            status = send(_users[j].getFd(), msg_to_send.c_str(), strlen(msg_to_send.c_str()), 0);
            if (status == -1)
                std::cout << "[Server] Send error to client fd " << _users[j].getFd() << ": " << strerror(errno) << std::endl;
        }
        else if (_channels[channel_name].isInChan(_users[j].getFd()) && client.getFd() == _users[j].getFd())
        {
            status = send(_users[j].getFd(), serv_msg.c_str(), strlen(serv_msg.c_str()), 0);
            if (status == -1)
                std::cout << "[Server] Send error to client fd " << _users[j].getFd() << ": " << strerror(errno) << std::endl;
        }
    }
    return (0);
}


// bool Server::join(User client, std::string msg)
// {
//     std::string serv_msg;
//     std::string str_msg = msg;
//     std::string channel_name = str_msg.substr(6, std::string::npos);
//     channel_name.erase(channel_name.size() - 2, 2);
    
//     if (this->channels[channel_name].normal_users.size() == 0 && this->channels[channel_name].operators.size() == 0)
//     {
//         serv_msg = ":localhost 403 #" + channel_name + " :No such channel\r\n";
//         this->channels[channel_name].operators[client.nick] = client;
//         client.channel_operators.push_back(channel_name);
//     }
//     else
//     {
//         this->channels[channel_name].normal_users[client.nick] = client;
//         client.channel_normal.push_back(channel_name);
//     }
//     fill_join_msg(serv_msg, channel_name, i);
//     std::string msg_to_send = ":" + client.nick + "!~" + client.username + " JOIN :#" + channel_name + "\r\n";

//     for (int j = 1; j < this->get_poll_count(); j++)
//     {
//         if (this->users[j].is_is_chan(channel_name) && i != j)
//         {
//             status = send(this->users[j].socket_fd, msg_to_send.c_str(), strlen(msg_to_send.c_str()), 0);
//             if (status == -1)
//                 std::cout << "[Server] Send error to client fd " << this->users[j].socket_fd << ": " << strerror(errno) << std::endl;
//         }
//         else if (this->users[j].is_is_chan(channel_name) && i == j)
//         {
//             status = send(this->users[j].socket_fd, serv_msg.c_str(), strlen(serv_msg.c_str()), 0);
//             if (status == -1)
//                 std::cout << "[Server] Send error to client fd " << this->users[j].socket_fd << ": " << strerror(errno) << std::endl;
//         }
//     }
//     return (0);
// }
