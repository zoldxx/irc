#include "irc.hpp"

int main(int ac, char **av)
{
    if (ac != 3)
        return (std::cout << "wrong number of arguments\n", 1);
    std::cout << "---- SERVER ----\n" << std::endl;
    try
    {
        Server serv(av[1], av[2]);
        while (1) 
        {
            int status = poll(serv.get_pollfd(), serv.get_poll_count(), 9000);
            if (status == -1)
            {
                std::cout << "[Server] Poll error: " << strerror(errno) << std::endl;
                return (1);
            }
            else if (status == 0)
            {
                std::cout << "[Server] Waiting..." << std::endl;
                continue;
            }
            for (int i = 0; i < serv.get_poll_count(); i++) 
            {
                if (serv.check_revent(i))
                    continue ;
                if (serv.check_new_connections(i))
                    serv.accept_new_connection(i);
                else 
                    serv.read_data_from_socket(i);
            }
        }
    }
    catch (std::exception & e)
    {
        std::cout << "creation of server fail" << std::endl;
        return (1);
    }
    return (0);
}

// int main(void)
// {
//     std::cout << "---- SERVER ----\n" << std::endl;
//     try
//     {
//         Server serv;
//         while (1) 
//         {
//             int status = poll(serv.get_pollfd(), serv.get_poll_count(), -1);
//             if (status == -1)
//             {
//                 std::cout << "[Server] Poll error: " << strerror(errno) << std::endl;
//                 return (1);
//             }
//             else if (status == 0)
//             {
//                 std::cout << "[Server] Waiting..." << std::endl;
//                 continue;
//             }
//             for (int i = 0; i < serv.get_poll_count(); i++) 
//             {
//                 if (serv.check_revent(i))
//                     continue ;
//                 if (serv.check_new_connections(i))
//                     serv.accept_new_connection();
//                 else 
//                     serv.read_data_from_socket(i);
//             }
//         }
//     }
//     catch (std::exception & e)
//     {
//         std::cout << "creation of server fail" << std::endl;
//         return (1);
//     }
//     return (0);
// }


