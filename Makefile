# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/02/20 16:23:00 by fsidler           #+#    #+#              #
#    Updated: 2019/02/27 16:20:32 by fsidler          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = computorV1

CLANG = clang++

FLAGS = -std=c++11 -Wall -Wextra -Werror -MMD

INC_DIR = includes
SRC_DIR = sources
OBJ_DIR = objs

SRC = Solver.cpp \
	  Parser.cpp \
	  main.cpp 

OBJ = $(addprefix $(OBJ_DIR)/, $(SRC:.cpp=.o))

DPD = $(addprefix $(OBJ_DIR)/, $(SRC:.cpp=.d))

Y = "\033[33m"
B = "\033[34m"
X = "\033[0m"
UP = "\033[A"
CUT = "\033[K"

opti:
	@make -j all

all:
	@make $(NAME)

$(NAME): $(OBJ)
	@$(CLANG) $(FLAGS) $^ -o $@
	@echo ${Y}[$(NAME)] compilation success${X}

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@echo ${B}compiling[$@]...${X}
	@mkdir -p $(OBJ_DIR)
	@$(CLANG) $(FLAGS) -I $(INC_DIR) -c $< -o $@
	@printf ${UP}${CUT}

clean:
	@rm -rf $(OBJ_DIR)

fclean: clean
	@rm -f $(NAME)

re:
	@make fclean
	@make

.PHONY: opti all clean fclean re

-include $(DPD)
