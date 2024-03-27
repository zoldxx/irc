#pragma once

#include <errno.h>
#include <netdb.h>
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/poll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <csignal>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <map>
#include <cstdlib>
#include <algorithm>
#include <cctype>
#include "user.hpp"
#include "channel.hpp"

class Channel;

class User;

class Server
{
    private:

    	int 														_port;
    	int 														_server_socket;
    	int 														_poll_size;
    	int 														_poll_count;
    	int 														_status;
    	std::string 												_mdp;
    	struct pollfd 												*_poll_fds;
    	std::map<int, User> 										_users;
    	std::map<std::string, Channel> 								_channels;
		std::map<std::string, bool (Server::*)(User&, std::string)>	_command;
		static Server                                          		 *_ptrServer;

    public:

    //gestion du serveur, de la connexion et de la transmission de donnees
	    Server(char *port, char *mdp);
	    ~Server();
	    struct pollfd	*get_pollfd(void);
		void			loop(void);
	    void			set_serv_socket(int sock);
		int 			create_server_socket(int port);
	    int				get_serv_socket(void);
	    int				get_port(void);
	    int				calloc_pollfd(int size);
	    int				get_poll_count(void);
	    void			accept_new_connection(int i);
	    void			add_to_poll_fds(int new_fd);
	    void			del_from_poll_fds(int i);
	    void			read_data_from_socket(int i);
		void      		initCommand(void);
		std::string 	extract(const std::string& chaine, std::string begin, std::string end);

    //commandes

		void	handleMessage(int fd);
		bool	capLs(User & client, std::string cmd);
		bool	pass(User & client, std::string cmd);
		bool	nick(User & client, std::string cmd);
		bool	user(User & client, std::string cmd);
		bool	ping(User & client, std::string cmd);
		bool	join(User & client, std::string cmd);
		bool	part(User & client, std::string cmd);
		bool	privmsg(User & client, std::string cmd);
		bool	topic(User & client, std::string cmd);
		bool	mode(User & client, std::string cmd);
		bool	oper(User & client, std::string cmd);
		bool	whois(User & client, std::string cmd);
		bool	kick(User & client, std::string cmd);
		bool	quit(User & client, std::string cmd);
		bool	invite(User & client, std::string cmd);



    //utils

		static void	closeServer(int signal);
    	int			is_in_chan(int fd, std::string channel_name);
    	void		fill_join_msg(std::string &serv_msg, std::string &channel_name, User &Client);
		bool		is_valid_nick(std::string nick, std::string &msg);
    	int 		is_valid_str(std::string &str);
    	int 		del_user(int i);
    	int 		is_operator(int i, std::string &chan);
		void 		print_chan(int i);
		void 		kick_from_a_chan(std::string &chan, int fd_to_kick);

	//exception

		class	init_failed :  public std::exception
		{
			public:

				virtual const char * what() const throw();

		};

		class	select_failed :  public std::exception
		{
			public:

				virtual const char * what() const throw();

		};

		class	fatal_error :  public std::exception
		{
			public:

				virtual const char * what() const throw();

		};

		class	closing_server :  public std::exception
		{
			public:

				virtual const char * what() const throw();

		};

};
