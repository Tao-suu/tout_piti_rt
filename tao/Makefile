# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tbez--du <tbez--du@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/02/27 17:19:19 by tbez--du          #+#    #+#              #
#    Updated: 2026/03/02 18:37:36 by tbez--du         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRC		=	main.c
OBJ		=	$(SRC:.c=.o)

SRC_DIR	=	src/
OBJ_DIR	=	obj/

CC		=	clang
CFLAGS	=	-Wall -Werror -Wextra
CLINKS	=	-Lmlx/ -lmlx -lmlx_Linux -lX11 -lXext -lm
CINC	=	-Imlx/

CF		=	$(CC) $(CFLAGS) $(CINC)

NAME	=	rt_poc


all:			$(NAME)

$(NAME):		$(addprefix $(OBJ_DIR), $(OBJ))
	$(CF) $(addprefix $(OBJ_DIR), $(OBJ)) $(CLINKS) -o $(NAME)

$(OBJ_DIR)%.o:	$(SRC_DIR)%.c
	mkdir obj -p
	$(CF) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)

fclean:	clean
	rm -f $(NAME)

re:	fclean all

.PHONY: all clean fclean re
