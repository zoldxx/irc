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
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <map>
#include <cstdlib>
#include <algorithm>
#include <cctype>


#define PORT 6667

int create_server_socket(int port);
void accept_new_connection(int server_socket, struct pollfd **poll_fds, int *poll_count, int *poll_size);
void read_data_from_socket(int i, struct pollfd **poll_fds, int *poll_count, int server_socket);
void add_to_poll_fds(struct pollfd *poll_fds[], int new_fd, int *poll_count, int *poll_size);
void del_from_poll_fds(struct pollfd **poll_fds, int i, int *poll_count);
int valid_args(char *port, char *mdp);
std::string extract(const std::string& chaine, std::string begin, std::string end) ;

class Channel;

class User
{
    public:
    std::string nick;
    std::string username;
    int socket_fd;
    std::vector<std::string> channel_normal;
    std::vector<std::string> channel_operators;
    int is_is_chan(std::string &chan);
};

class Channel
{   
    public:
    std::string name;
    std::string topic;
    std::map<std::string, User> normal_users;
    std::map<std::string, User> operators;
};

class Server
{
    private:
    int port;
    int server_socket;
    int poll_size;
    int poll_count;
    int status;
    std::string mdp;
    struct pollfd *poll_fds;
    std::map<int, User> users;
    std::map<std::string, Channel> channels;

    public:
    //gestion du serveur, de la connexion et de la transmission de donnees
    Server(char *port, char *mdp);
    ~Server();
    void set_serv_socket(int sock);
    int get_serv_socket(void);
    int get_port(void);
    int calloc_pollfd(int size);
    struct pollfd *get_pollfd(void);
    int get_poll_count(void);
    int check_revent(int i);
    int check_new_connections(int i);
    void accept_new_connection(int i);
    void add_to_poll_fds(int new_fd);
    void read_data_from_socket(int i);
    void del_from_poll_fds(int i);

    //commandes
    int is_special_message(int i, char *msg);
    int first_message(int i, char *msg);
    int set_nick(int i, char *msg);
    int join_cmd(int i, char *msg);
    int mode(int i, char *msg);
    int ping(int i, char *msg);
    int privmsg(int i, char *msg);
    int topic_cmd(int i, char *msg);
    int kick(int i, char *msg);

    //utils
    int fd_is_in_chan(int fd, std::string channel_name);
    void fill_join_msg(std::string &serv_msg, std::string &channel_name, int i);
    int is_valid_nick(std::string &nick);
    int is_valid_str(std::string &str);
    int del_user(int i);
    int is_operator(int i, std::string &chan);
};

// class Channel
// {   
//     public:
//     std::string name;
//     std::string topic;
//     std::vector<User> all_users;
//     std::vector<User> normal_users;
//     std::vector<User> operators;
//     std::vector<User> kick_users;
// };

// class Channel
// {   
//     public:
//     std::string name;
//     std::string topic;
//     std::vector<std::string> all_users;
//     std::vector<std::string> normal_users;
//     std::vector<std::string> operators;
//     std::vector<std::string> kick_users;
// };

// class Channel
// {   
//     public:
//     std::string name;
//     std::string topic;
//     std::map<std::string, User> all_users;
//     std::map<std::string, User> normal_users;
//     std::map<std::string, User> operators;
//     std::map<std::string, User> kick_users;
// };

// class User
// {
//     public:
//     std::string nick;
//     std::string username;
//     int socket_fd;
//     int is_ops;
//     std::vector<Channel> channel_normal;
//     std::vector<Channel> channel_operators;
//     int is_is_chan(std::string chan);
// };