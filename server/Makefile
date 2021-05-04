NAME =		webserv

CC =		clang++

FLAGS =		-Wall -Werror -Wextra -g -std=c++98

# HEADER = /.

SRCS =		main.cpp \
			ParseRequest.cpp

OBJ =		$(SRCS:.cpp=.o)

%.o: %.cpp
			$(CC) $(FLAGS) -c $< -o $@

all: $(NAME)
$(NAME): $(OBJ)
			$(CC) $(FLAGS) $(OBJ) -o $(NAME)
clean:
			rm -rf $(OBJ)
fclean:
			rm -rf $(OBJ) $(NAME)
re: fclean all

.PHONY: all clean fclean re