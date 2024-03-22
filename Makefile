NAME	:= ircserv

CC		:= c++
FLAGS	:= -Wall -Wextra -Werror -std=c++98 -g

SRC		:= 	main.cpp server.cpp read_data.cpp utils.cpp join.cpp privmsg.cpp ping.cpp first_message.cpp mode.cpp nick.cpp topic.cpp kick.cpp

INCLUDE	:=

OBJ		:=$(SRC:.cpp=.o)

###########################################################

all: $(NAME)

$(NAME): $(OBJ) ${INCLUDE} 
	$(CC) $(FLAGS) $(OBJ) -o $(NAME) 

%.o: %.cpp
	$(CC) $(FLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY : all clean fclean re