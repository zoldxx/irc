NAME	=	ircserv

SRC		=	main.cpp						\
			server.cpp						\
			user.cpp						\
			channel.cpp						\
			server_getset.cpp				\
			server_poll.cpp					\
			server_exception.cpp			\
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

OBJ_DIR = ./obj

OBJ = $(addprefix $(OBJ_DIR)/, $(SRC:.cpp=.o))


CXX = c++

CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -g3

$(OBJ_DIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@


TOTAL_FILES := $(words $(SRC))
CURRENT_FILE = 0
PERCENTAGE = 0

GREEN = \033[32m
RED = \033[31m
YELLOW = \033[33m
BOLD = \e[1m
UNBOLD = \e[0m
RESET = \033[0m

define update_progress
	$(eval CURRENT_FILE=$(shell echo $$(($(CURRENT_FILE)+1))))
	$(eval PERCENTAGE=$(shell echo $$(($(CURRENT_FILE)*100/$(TOTAL_FILES)))))
	@printf "$(GREEN)\rCompiling [%-20s] %d%% $(RESET)" "####################" "$(PERCENTAGE)"
endef

###########################################################

all : $(NAME)

$(NAME) : $(SRC) $(OBJ) $(INC)
	$(CXX) $(CXXFLAGS) $(OBJ) -I $(INC) -o $(NAME)
	@echo "\n $(BOLD)$(NAME)$(UNBOLD) is built  🪄$(RESET)"

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(OBJ_DIR)
	$(call update_progress)
	@$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

clean:
	@rm -rf $(OBJ_DIR)
	@rm -rf $(DEPS)
	@echo "$(YELLOW)cleaning  🧽$(RESET)"

fclean: clean
	@rm -rf $(NAME)
	@echo "$(RED)full clean  🧹$(RESET)"

re: fclean all

-include $(DEPS)

.PHONY : all clean fclean re
