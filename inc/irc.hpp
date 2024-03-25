/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dberreby <dberreby@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 14:48:17 by blerouss          #+#    #+#             */
/*   Updated: 2024/03/22 17:56:04 by dberreby         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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


#define PORT 6667

void accept_new_connection(int server_socket, struct pollfd **poll_fds, int *poll_count, int *poll_size);
void read_data_from_socket(int i, struct pollfd **poll_fds, int *poll_count, int server_socket);
void add_to_poll_fds(struct pollfd *poll_fds[], int new_fd, int *poll_count, int *poll_size);
void del_from_poll_fds(struct pollfd **poll_fds, int i, int *poll_count);
int valid_args(std::string port, std::string mdp);

class	invalid_args :  public std::exception
{
	public:

		virtual const char * what() const throw();

};

class Channel;

class User
{
    public:

    	int 			is_is_chan(std::string &chan);
		std::string		getBuffer(void);
		void			setBuffer(std::string buffer);

    	std::string 				nick;
    	std::string 				username;
    	std::string 				_buffer;
    	int 						socket_fd;
    	std::vector<std::string> 	channel_normal;
    	std::vector<std::string>	channel_operators;
};

class Channel
{   
    public:

    	std::string 				name;
    	std::string 				topic;
    	std::map<std::string, User> normal_users;
    	std::map<std::string, User> operators;
};

class Server
{
    private:

    	int 													port;
    	int 													server_socket;
    	int 													poll_size;
    	int 													poll_count;
    	int 													status;
    	std::string 											mdp;
    	struct pollfd 											*poll_fds;
    	std::map<int, User> 									users;
    	std::map<std::string, Channel> 							channels;
		std::map<std::string, bool (*)(User&, std::string)>     _Command;
		static Server                                           *_ptrServer;

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
	    void			read_data_from_socket(int i);
	    void			del_from_poll_fds(int i);
		void      		initCommand(void);
		std::string 	extract(const std::string& chaine, std::string begin, std::string end);

    //commandes

		void	handleMessage(int fd);
    	int 	is_special_message(int i, char *msg);
    	int 	first_message(int i, char *msg);
    	int 	set_nick(int i, char *msg);
    	int 	join_cmd(int i, char *msg);
    	int 	mode(int i, char *msg);
    	int 	ping(int i, char *msg);
    	int 	privmsg(int i, char *msg);
    	int 	topic_cmd(int i, char *msg);
    	int 	kick(int i, char *msg);
		bool	capLs(User client, std::string cmd);
		bool	nick(User client, std::string cmd);
		bool	user(User client, std::string cmd);
		bool	whois(User client, std::string cmd);
		bool	pass(User client, std::string cmd);
		bool	ping(User client, std::string cmd);
		bool	quit(User client, std::string cmd);
		bool	join(User client, std::string cmd);
		bool	privmsg(User client, std::string cmd);
		bool	kick(User client, std::string cmd);
		bool	topic(User client, std::string cmd);
		bool	mode(User client, std::string cmd);
		bool	invite(User client, std::string cmd);
		bool	part(User client, std::string cmd);
		bool	oper(User client, std::string cmd);



    //utils

		static void	closeServer(int signal);
    	int			is_in_chan(int fd, std::string channel_name);
    	void		fill_join_msg(std::string &serv_msg, std::string &channel_name, int i);
    	int			is_valid_nick(std::string &nick);
    	int 		is_valid_str(std::string &str);
    	int 		del_user(int i);
    	int 		is_operator(int i, std::string &chan);
		void 		print_chan(int i);
		void 		kick_from_a_chan(std::string &user_to_kick, std::string &chan);



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
