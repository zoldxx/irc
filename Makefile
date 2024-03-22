# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: blerouss <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/03/22 14:26:42 by blerouss          #+#    #+#              #
#    Updated: 2024/03/22 17:17:27 by blerouss         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	=	ircserv

SRC		=	main.cpp						\
			server.cpp						\
			user.cpp						\
			getter_setter.cpp				\
			handlePoll.cpp					\
			throw.cpp						\
			read_data.cpp					\
			utils.cpp						\
			join.cpp						\
			privmsg.cpp						\
			ping.cpp						\
			first_message.cpp				\
			mode.cpp						\
			nick.cpp						\
			topic.cpp						\
			kick.cpp						\

INC = 		inc								\

OBJ_DIR = obj

OBJ = $(addprefix $(OBJ_DIR)/, $(SRC:.cpp=.o))


CXX = c++

CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -g3

$(OBJ_DIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

###########################################################

all : $(NAME)

$(NAME) : $(SRC) $(OBJ) $(INC)
	$(CXX) $(CXXFLAGS) $(OBJ) -I $(INC) -o $(NAME) 

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY : all clean fclean re
