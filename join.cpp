#include "inc/server.hpp"

void Server::fill_join_msg(std::string &serv_msg, std::string &channel_name, User &client)
{
    std::vector<int>::iterator it;
    serv_msg += ":" + client.getNick() + "!~" + client.getUsername() + " JOIN :#" + channel_name + "\r\n";
    if (_channels[channel_name].getTopic().size() == 0)
        serv_msg += ":localhost 331 " + client.getNick() + " #" + channel_name + " :No topic is set\r\n";
    else
        serv_msg += ":localhost 332 " + client.getNick() + " #" + channel_name + " " + _channels[channel_name].getTopic() + "\r\n";
    serv_msg += ":localhost 353 " + client.getNick() + " = #" + channel_name + " :";
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
    serv_msg += "\r\n:localhost 366 " + client.getNick() + " #" + channel_name + " :End of /NAMES list\r\n";  
}

bool Server::join(User &client, std::string cmd)
{
    // int status;
    std::string serv_msg;
    std::string channel_name = cmd.substr(1, std::string::npos);
    //channel_name.erase(channel_name.size() - 2, 2);
    
    // if (_channels[channel_name].getUsers().size() == 0)
      if (_channels[channel_name].getUsers().size() == 0 && _channels[channel_name].getOperators().size() == 0)
    {
        serv_msg = ":localhost 403 #" + channel_name + " :No such channel\r\n";
        _channels[channel_name].addOperator(client.getFd());
        //_channels[channel_name].getOperators().push_back(client.getFd());
    }
    else
        _channels[channel_name].addUser(client.getFd());
    client.addChannel(channel_name);
    fill_join_msg(serv_msg, channel_name, client);
    std::cout << "channel name =" << channel_name << "|\nserv message =" << serv_msg;
    std::string msg_to_send = ":" + client.getNick() + "!~" + client.getUsername() + " JOIN :#" + channel_name + "\r\n";

    std::vector<int> vec_user = _channels[channel_name].getUsers();
    for (std::vector<int>::iterator ite = vec_user.begin(); ite != vec_user.end(); ite++)
    {
        if (*ite == client.getFd())
        {
            if (send(*ite, serv_msg.c_str(), serv_msg.size(), 0) < 1)
            return (false);
        }
        else
        {
            if (send(*ite, msg_to_send.c_str(), msg_to_send.size(), 0) < 1)
                return (false);
        }
    }
    std::vector<int> vec_op = _channels[channel_name].getOperators();
    for (std::vector<int>::iterator ite = vec_op.begin(); ite != vec_op.end(); ite++)
    {
        if (*ite == client.getFd())
        {
            if (send(*ite, serv_msg.c_str(), serv_msg.size(), 0) < 1)
            return (false);
        }
        else
        {
            if (send(*ite, msg_to_send.c_str(), msg_to_send.size(), 0) < 1)
                return (false);
        }
    }
    return (0);
}

