# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: pvivian <pvivian@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/05/05 16:47:32 by pvivian           #+#    #+#              #
#    Updated: 2021/05/05 19:04:11 by pvivian          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME =		webserv

CC =		clang++

FLAGS =		-Wall -Werror -Wextra -g -std=c++98 -MMD  -I ./sources
# -fsanitize=address

MAIN = $(SRCS_DIR)main.cpp

SRCS =		main.cpp \
			Server.cpp \
			Session.cpp \
			ParseRequest.cpp \
			ParseConfig.cpp

SRCS_DIR = ./sources/

OBJS =	$(SRCS:.cpp=.o)
OBJS_DIR = ./objects/
OBJ = $(addprefix $(OBJS_DIR), $(OBJS))
DEP = $(OBJ:.o=.d)

LOGFILE = log

all: $(NAME)

$(NAME): $(OBJ) $(MAIN)
	$(CC) $(FLAGS) $(OBJ) -o $(NAME)

-include $(DEP)

$(OBJS_DIR)%.o: $(SRCS_DIR)%.cpp
	test -d $(OBJS_DIR) || mkdir $(OBJS_DIR)
	$(CC) $(FLAGS) -c $< -o $@

clean:
	rm -rf $(OBJS_DIR)

fclean:
	rm -rf $(OBJS_DIR) $(NAME) $(LOGFILE)
	
re: fclean all

.PHONY: all clean fclean re
