#include "irc.hpp"

Server::Server(char *port_str, char *mdp)
{
    this->poll_fds = NULL;
    this->poll_size = 10;
    this->poll_count = 1;

    if (!valid_args(port_str, mdp))
    {
        std::cout << "invalid arg" << std::endl;
        throw std::exception();
    }
    this->mdp = mdp;
    this->port = atoi(port_str);
    this->set_serv_socket(create_server_socket(this->get_port()));
    if (this->get_serv_socket() == -1)
    {
        std::cout << "create serv socket" << std::endl;
        throw std::exception();
    }

    std::cout << "[Server] Listening on port " << PORT << std::endl;
    this->status = listen(this->get_serv_socket(), 100);
    if (status != 0)
    {
        std::cout << "[Server] Listen error: " << strerror(errno) << std::endl;
        throw std::exception();
    }
    if (!this->calloc_pollfd(this->poll_size))
    {
        std::cout << "probleme calloc" << std::endl;
        throw std::exception();
    }
    this->poll_fds[0].fd = this->server_socket;
    this->poll_fds[0].events = POLLIN;
}

// Server::Server(char *port_str, char *mdp)
// {
//     this->poll_fds = NULL;
//     this->poll_size = 10;
//     this->poll_count = 1;

//     if (!valid_args(port_str, mdp))
//         throw std::exception();
//     this->mdp = mdp;
//     this->port = atoi(port_str);
//     this->set_serv_socket(create_server_socket(this->get_port()));
//     if (this->get_serv_socket() == -1)
//         throw std::exception();

//     std::cout << "[Server] Listening on port " << PORT << std::endl;
//     this->status = listen(this->get_serv_socket(), 100);
//     if (status != 0)
//     {
//         std::cout << "[Server] Listen error: " << strerror(errno) << std::endl;
//         throw std::exception();
//     }
//     if (!this->calloc_pollfd(this->poll_size))
//         throw std::exception();
//     this->poll_fds[0].fd = this->server_socket;
//     this->poll_fds[0].events = POLLIN;
// }

// Server::Server(void)
// {
//     this->poll_fds = NULL;
//     this->poll_size = 10;
//     this->poll_count = 1;

//     this->set_serv_socket(create_server_socket());
//     if (this->get_serv_socket() == -1)
//         throw std::exception();

//     std::cout << "[Server] Listening on port " << PORT << std::endl;
//     this->status = listen(this->get_serv_socket(), 100);
//     if (status != 0)
//     {
//         std::cout << "[Server] Listen error: " << strerror(errno) << std::endl;
//         throw std::exception();
//     }
//     if (!this->calloc_pollfd(this->poll_size))
//         throw std::exception();
//     this->poll_fds[0].fd = this->server_socket;
//     this->poll_fds[0].events = POLLIN;
// }

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

void Server::set_serv_socket(int sock)
{
    this->server_socket = sock;
}

int Server::get_serv_socket(void)
{
    return (this->server_socket);
}

struct pollfd *Server::get_pollfd(void)
{
    return (this->poll_fds);
}

int Server::get_poll_count(void)
{
    return (this->poll_count);
}

int Server::get_port(void)
{
    return (this->port);
}

int Server::calloc_pollfd(int size)
{
    if (!this->poll_fds)
    {
        this->poll_fds = (struct pollfd *) calloc(size + 1, sizeof *this->poll_fds);
        if (!this->poll_fds)
            return (0);
        return (1);

    }
    return (1);
}

int Server::check_revent(int i)
{
    if ((poll_fds[i].revents & POLLIN) != 1)
        return (1);
    return (0);
}

int Server::check_new_connections(int i)
{
    if (this->poll_fds[i].fd == this->get_serv_socket())
        return (1);
    return (0);
}

void Server::accept_new_connection(int i)
{
    (void) i;
    int client_fd = accept(server_socket, NULL, NULL);
    if (client_fd == -1) 
    {
        std::cout << "[Server] Accept error: " << strerror(errno) << std::endl;
        throw std::exception();
    }
    this->add_to_poll_fds(client_fd);
    // this->users[i].socket_fd = client_fd;
    // std::cout << "user " << i << " a le fd " << client_fd << std::endl;
    //std::cout << "[Server] Accepted new connection on client socket " << client_fd << std::endl;
}


void Server::add_to_poll_fds(int new_fd)
{
    // S'il n'y a pas assez de place, il faut réallouer le tableau de poll_fds
    if (this->poll_count == this->poll_size) 
    {
        this->poll_size *= 2; // Double la taille
        this->poll_fds = (struct pollfd *) realloc(this->poll_fds, sizeof(*(this->poll_fds)) * (this->poll_size));
        if (!this->poll_fds)
        {
            std::cout << "not enough memory" << std::endl;
            throw std::exception();
        }
    }
    this->poll_fds[this->poll_count].fd = new_fd;
    this->poll_fds[this->poll_count].events = POLLIN;
    this->poll_count++;
}

void Server::del_from_poll_fds(int i)
{
    this->poll_fds[i] = this->poll_fds[this->poll_count - 1];
    this->poll_count -= 1;
}

// void Server::accept_new_connection(void)
// {
//     int client_fd;
//     std::string msg;
//     int status;

//     //std::cout << "accept avant" << std::endl;

//     client_fd = accept(server_socket, NULL, NULL);
//     if (client_fd == -1) 
//     {
//         std::cout << "[Server] Accept error: " << strerror(errno) << std::endl;
//         throw std::exception();
//     }
//     this->add_to_poll_fds(client_fd);
//     std::cout << "[Server] Accepted new connection on client socket " << client_fd << std::endl;

//     //msg = "Welcome to 42 serv! \n";
//     //msg = ":nonstop.ix.me.dal.net 001 aaa :Welcome to the DALnet IRC Network aaa!~dberreby@62.210.32.190\n";
//     msg = "001 Welcome to 42 serv !\n";
//     status = send(client_fd, msg.c_str(), strlen(msg.c_str()), 0);
//     if (status == -1)
//     std::cout << "[Server] Send error to client " << client_fd << strerror(errno) << std::endl;
// }

// /connect localhost 6667