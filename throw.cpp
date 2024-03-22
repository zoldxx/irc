/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   throw.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blerouss <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 14:23:32 by blerouss          #+#    #+#             */
/*   Updated: 2024/03/22 17:11:00 by blerouss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/irc.hpp"
    
const char  *Server::init_failed::what(void) const throw()
{   
    return ("The server initialization failed.");
}
        
const char  *invalid_args::what(void) const throw()
{   
    return ("The program needs, in this order :\n    _ a port number\n    _ a password\n");
}
        
const char  *Server::select_failed::what(void) const throw()
{       
    return ("A call to select failed.");
}
        
const char  *Server::closing_server::what(void) const throw()
{   
    return ("\nThe server's closing.");
}   
              
const char  *Server::fatal_error::what(void) const throw()
{   
    return ("A fatal error occurred while running the server.");
}   
