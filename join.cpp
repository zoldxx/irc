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
            serv_msg += _users[*it].getNick() + " ";
    }
    serv_msg += "\r\n:localhost 366 " + client.getNick() + " #" + channel_name + " :End of /NAMES list\r\n";  
}

bool Server::send_join_msg(User &client, std::string &channel_name, std::string &serv_msg, std::string &msg_to_send)
{
    for (std::vector<int>::iterator ite = _channels[channel_name].getUsers().begin(); ite != _channels[channel_name].getUsers().end(); ite++)
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
    for (std::vector<int>::iterator ite = _channels[channel_name].getOperators().begin(); ite != _channels[channel_name].getOperators().end(); ite++)
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
    return (true);
}

//room full = >> :atw.hu.eu.dal.net 471 gdgergg #rommtest :Cannot join channel (+l)

bool Server::join(User &client, std::string cmd)
{
    std::string serv_msg;
    std::string channel_name = cmd.substr(1, std::string::npos);

    if (_channels[channel_name]._m_maxUser != -1 && (_channels[channel_name].getOperators().size() + _channels[channel_name].getUsers().size())  >= _channels[channel_name]._m_maxUser)
    {
        serv_msg = ":localhost 471 " + client.getNick() + " #" + channel_name + " :Cannot join channel (+l)\r\n";
        if (send(client.getFd(), serv_msg.c_str(), serv_msg.size(), 0) < 1)
            return (false);
        return (false)
    }
    //if (_channels[channel_name]._m_invit == true && cl)

    if (_channels[channel_name]._m_invit == 1 && _channels[channel_name]._m_maxUser == 8
        && _channels[channel_name]._m_topic == 1 && _channels[channel_name]._m_password == "yo")
        return (1);
    
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
    //std::cout << "channel name =" << channel_name << "|\nserv message =" << serv_msg;
    std::string msg_to_send = ":" + client.getNick() + "!~" + client.getUsername() + " JOIN :#" + channel_name + "\r\n";
    if (send_join_msg(client, channel_name, serv_msg, msg_to_send) == false)
        return (false);
    return (0);
}

// bool Server::join(User &client, std::string cmd)
// {
//     std::string serv_msg;
//     std::string channel_name = cmd.substr(1, std::string::npos);
    
//     if (_channels[channel_name].getUsers().size() == 0 && _channels[channel_name].getOperators().size() == 0)
//     {
//         serv_msg = ":localhost 403 #" + channel_name + " :No such channel\r\n";
//         _channels[channel_name].addOperator(client.getFd());
//         //_channels[channel_name].getOperators().push_back(client.getFd());
//     }
//     else
//         _channels[channel_name].addUser(client.getFd());
//     client.addChannel(channel_name);
//     fill_join_msg(serv_msg, channel_name, client);
//     //std::cout << "channel name =" << channel_name << "|\nserv message =" << serv_msg;
//     std::string msg_to_send = ":" + client.getNick() + "!~" + client.getUsername() + " JOIN :#" + channel_name + "\r\n";

//     for (std::vector<int>::iterator ite = _channels[channel_name].getUsers().begin(); ite != _channels[channel_name].getUsers().end(); ite++)
//     {
//         if (*ite == client.getFd())
//         {
//             if (send(*ite, serv_msg.c_str(), serv_msg.size(), 0) < 1)
//             return (false);
//         }
//         else
//         {
//             if (send(*ite, msg_to_send.c_str(), msg_to_send.size(), 0) < 1)
//                 return (false);
//         }
//     }
//     for (std::vector<int>::iterator ite = _channels[channel_name].getOperators().begin(); ite != _channels[channel_name].getOperators().end(); ite++)
//     {
//         if (*ite == client.getFd())
//         {
//             if (send(*ite, serv_msg.c_str(), serv_msg.size(), 0) < 1)
//             return (false);
//         }
//         else
//         {
//             if (send(*ite, msg_to_send.c_str(), msg_to_send.size(), 0) < 1)
//                 return (false);
//         }
//     }
//     return (0);
// }

