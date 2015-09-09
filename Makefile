# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: adebray <adebray@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2015/02/11 01:49:54 by amaurer           #+#    #+#              #
#    Updated: 2015/09/07 18:55:52 by adebray          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRC_DIR		=	src/
INC_DIR		=	include
BIN_DIR		=	bin/

NAME		=	wolf3d
BIN_NAME	=	$(BIN_DIR)$(NAME)
SRC_FILES	=	$(shell ls src | grep '.c')

export CC		=	clang
export CFLAGS	=	-Wall -Werror -Wextra -pedantic -g3
LDFLAGS			=	-Lminilibx_macos -lmlx -framework OpenGL -framework AppKit

SRC			=	$(addprefix $(SRC_DIR), $(SRC_FILES))
OBJ			=	$(SRC:.c=.o)

all: $(BIN_NAME)
	@echo $(SRC)"\n"$(OBJ)
	@echo "\033[32m•\033[0m $(BIN_DIR)$(NAME) is ready."

$(BIN_NAME): $(OBJ)
	@mkdir -p $(BIN_DIR)
	$(CC) $(LDFLAGS) -o $(BIN_NAME) $^

clean:
	rm -rf $(OBJ_DIR)

fclean:
	rm -rf $(OBJ_DIR)
	rm -rf $(BIN_NAME)

re: fclean all

.PHONY: all re fclean clean $(BIN_NAME)
