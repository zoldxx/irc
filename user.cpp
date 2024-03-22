/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blerouss <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 16:52:11 by blerouss          #+#    #+#             */
/*   Updated: 2024/03/22 17:01:23 by blerouss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/irc.hpp"

std::string		User::getBuffer(void)
{
	return _buffer;
}

void			User::setBuffer(std::string buffer)
{
	_buffer = buffer;
}
