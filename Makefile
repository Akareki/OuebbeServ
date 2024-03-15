CC=c++
CFLAGS=-Wall -Wextra -Werror -std=c++98
NAME=webserv
HEADER=webserv.hpp VirtualServer.hpp TcpListener.hpp
SRCS= ./main.cpp ./VirtualServer.cpp ./TcpListener.cpp ./utils.cpp
OBJS= $(SRCS:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

%.o: %.cpp $(HEADER)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: clean fclean all re