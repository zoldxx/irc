/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blerouss <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 14:35:41 by blerouss          #+#    #+#             */
/*   Updated: 2024/03/22 17:01:23 by blerouss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/irc.hpp"

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
