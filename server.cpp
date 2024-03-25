/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blerouss <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 14:48:59 by blerouss          #+#    #+#             */
/*   Updated: 2024/03/22 18:41:22 by blerouss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/irc.hpp"

Server* Server::_ptrServer = NULL;

Server::Server(char *port_str, char *mdp)
{
    this->poll_fds = NULL;
    this->poll_size = 10;
    this->poll_count = 1;

    this->mdp = mdp;
    this->port = atoi(port_str);
    this->set_serv_socket(create_server_socket(this->get_port()));
    if (server_socket == -1 || listen(this->get_serv_socket(), 20) == -1
    	|| !this->calloc_pollfd(this->poll_size))
	{
		closeServer(0);
        throw init_failed();
	}
    this->poll_fds[0].fd = this->server_socket;
    this->poll_fds[0].events = POLLIN;
    std::signal(SIGINT, closeServer);
    std::signal(SIGQUIT, closeServer);
	initCommand();
	_ptrServer = this;
}

Server::~Server(void)
{
    if (this->poll_fds)
    {
        int i = 0;
        while (i < poll_count)
        {
            close(this->poll_fds[i].fd);
            i++;
        }
        free(this->poll_fds);
    }
}

void		Server::closeServer(int signal)
{
	close(_ptrServer->server_socket);
	if (signal != 0)
		throw closing_server();
}

int Server::create_server_socket(int port) 
{
    struct sockaddr_in sa;
    int socket_fd;
    int status;

    memset(&sa, 0, sizeof sa);
    sa.sin_family = AF_INET; // IPv4
    sa.sin_addr.s_addr = htonl(INADDR_LOOPBACK); // 127.0.0.1, localhost
    sa.sin_port = htons(port);

    // Création de la socket
    socket_fd = socket(sa.sin_family, SOCK_STREAM, 0);
    if (socket_fd == -1)
        return (-1);

    // Liaison de la socket à l'adresse et au port
    status = bind(socket_fd, (struct sockaddr *)&sa, sizeof sa);
    if (status != 0)
        return (-1);

    return (socket_fd);
}

void      		Server::initCommand(void)
{
	//_Command["CAP"]     = &capLs;
	//_Command["NICK"]    = &nick;
	//_Command["USER"]    = &user;
	//_Command["WHOIS"]   = &whois;
	//_Command["PASS"]    = &pass;
	//_Command["PING"]    = &ping;
	//_Command["QUIT"]    = &quit;
	//_Command["JOIN"]    = &join;
	//_Command["PRIVMSG"] = &privmsg;
	//_Command["KICK"]    = &kick;
	//_Command["TOPIC"]   = &topic;
	//_Command["MODE"]    = &mode;
	//_Command["INVITE"]  = &invite;
	//_Command["PART"]    = &part;
	//_Command["OPER"]    = &oper;
}

std::string 	Server::extract(const std::string& chaine, std::string begin, std::string end) 
{
    std::size_t debut = chaine.find(begin);
    std::size_t fin = chaine.find(end, debut + 1);
    if (debut != std::string::npos && fin != std::string::npos) 
        return chaine.substr(debut + 1, fin - debut - 1);
    else 
        return "";
}

void	Server::loop(void)
{
	std::cout << "---- SERVER ----\n" << std::endl;
	while (1) 
	{
	    int status = poll(poll_fds, poll_count, 9000);
	    if (status == -1)
	        return;
	    else if (status == 0)
	    {
	        std::cout << "[Server] Waiting..." << std::endl;
	        continue;
	    }
	    for (int i = 0; i < poll_count; i++) 
	    {
    		if ((poll_fds[i].revents & POLLIN) != 1)
	            continue ;
    		if (this->poll_fds[i].fd == server_socket)
			{
    			int client_fd = accept(server_socket, NULL, NULL);
    			if (client_fd != -1) 
    				this->add_to_poll_fds(client_fd);
			}
	        else 
	            handleMessage(i);
	    }
	}
}
