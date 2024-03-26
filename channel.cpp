#include "inc/channel.hpp"

Channel::Channel()
{
}

Channel::Channel(std::string name) : _name(name), _topic("")
{
}

Channel::~Channel(void)
{
}

// std::string			Channel::getName(void) const
// {
//     return (_name);
// }

// std::string			Channel::getTopic(void) const
// {
//     return (_topic);
// }

// std::vector<int>    Channel::getUsers(void) const
// {
//     return(_users);
// }

// std::vector<int>    Channel::getOperators(void) const
// {
//     return(_operators);
// }

std::string			&Channel::getName(void)
{
    return (_name);
}

std::string			&Channel::getTopic(void)
{
    return (_topic);
}


std::vector<int>    &Channel::getUsers(void)
{
    return(_users);
}

std::vector<int>    &Channel::getOperators(void)
{
    return(_operators);
}

void				Channel::setTopic(std::string topic)
{
    _topic = topic;
}

bool 			Channel::isInChan(int fd) const
{
    if (std::find(this->_users.begin(), this->_users.end(), fd) != _users.end())
        return (true);
    
    return (false);
}

bool 			Channel::isOperator(int fd) const
{
    if (std::find(this->_operators.begin(), this->_operators.end(), fd) != _operators.end())
        return (true);
    
    return (false);
}

void			Channel::addUser(int user)
{
	_users.push_back(user);
}

void	Channel::delUser(int user)
{
	std::vector<int>::iterator pos = std::find(_users.begin(), _users.end(), user);
	if (pos != _users.end())
		_users.erase(pos);
}

void			Channel::addOperator(int op)
{
	_operators.push_back(op);
}

void	Channel::delOperator(int op)
{
	std::vector<int>::iterator pos = std::find(_operators.begin(), _operators.end(), op);
	if (pos != _operators.end())
		_operators.erase(pos);
}