/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blerouss <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 15:22:39 by blerouss          #+#    #+#             */
/*   Updated: 2024/03/25 13:26:43 by blerouss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/irc.hpp"

int Server::is_operator(int i, std::string &chan)
{
    if (std::find(this->users[i].channel_operators.begin(), this->users[i].channel_operators.end(), chan) != this->users[i].channel_operators.end())
        return (1);  
    return (0);
}
