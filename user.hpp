
#pragma once

#include <iostream>
#include <ostream>
#include <cstring>
#include <map>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>

class	Channel;

class	User
{


	public:

		User(void) {};
		User(int socket);
		~User(void);

    	int 			is_is_chan(std::string &chan);
		std::string		getBuffer(void);
		void			setBuffer(std::string buffer);

		std::string			getName(void);
		int					getSocket(void);

    	std::string 				nick;
    	std::string 				username;
    	std::string 				_buffer;
    	int 						socket_fd;
    	std::vector<std::string> 	channel_normal;
    	std::vector<std::string>	channel_operators;

	private:


};
