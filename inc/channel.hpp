
#pragma once

#include <iostream>
#include <ostream>
#include <cstring>
#include <map>
#include <vector>
#include <algorithm>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "user.hpp"

class	User;

class	Channel
{


	public:

		Channel(void);
		Channel(std::string name);
		~Channel(void);

		std::string			getName(void) const;
		std::string			getTopic(void) const;
		std::vector<int>	getUsers(void) const;
		std::vector<int>	getOperators(void) const;
		void				setTopic(std::string topic);
		bool				isInChan(int fd) const;
		bool				isOperator(int fd) const;
		void				addUser(int user);
		void				delUser(int user);
		void				addOperator(int op);
		void				delOperator(int op);

	private:

    	std::string 		_name;
    	std::string 		_topic;
    	std::vector<int> 	_users;
    	std::vector<int>	_operators;

};


