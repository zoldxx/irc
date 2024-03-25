/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blerouss <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 14:35:41 by blerouss          #+#    #+#             */
/*   Updated: 2024/03/25 13:23:52 by blerouss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/irc.hpp"

int valid_args(std::string port, std::string mdp)
{
    if (port.size() < 3 || port.size() > 5)
        return (0);
    for (size_t i = 0; i < port.size(); i++)
    {
        if (!(port[i] >= '0' && port[i] <= '9'))
            return (0);
    }
    if (mdp.size() < 1)
        return (0);
    return (1);
}

int main(int ac, char **av)
{
    try
    {
    	if (ac != 3 || !valid_args(av[1], av[2]))
			throw invalid_args();
        Server serv(av[1], av[2]);
		serv.loop();
    }
    catch (std::exception & e)
    {
		std::cout << e.what() << std::endl;
        return (1);
    }
    return (0);
}
