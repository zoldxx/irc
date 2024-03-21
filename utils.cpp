#include "irc.hpp"

int valid_args(char *port, char *mdp)
{
    if (!port || !mdp)
        return (0);
    std::string port_str = port;
    if (port_str.size() != 4)
        return (0);
    for (int i = 0; i < 4; i++)
    {
        if (!(port[i] >= '0' && port[i] <= '9'))
            return (0);
    }
    std::string mdp_str = mdp;
    if (mdp_str.size() < 1)
        return (0);
    return (1);
}

int create_server_socket(int port) 
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
    if (socket_fd == -1) {
        fprintf(stderr, "[Server] Socket error: %s\n", strerror(errno));
        return (-1);
    }
    printf("[Server] Created server socket fd: %d\n", socket_fd);

    // Liaison de la socket à l'adresse et au port
    status = bind(socket_fd, (struct sockaddr *)&sa, sizeof sa);
    if (status != 0) {
        fprintf(stderr, "[Server] Bind error: %s\n", strerror(errno));
        return (-1);
    }
    printf("[Server] Bound socket to localhost port %d\n", PORT);

    return (socket_fd);
}