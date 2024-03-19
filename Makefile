# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: aoizel <marvin@42.fr>                      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/02/06 14:50:12 by aoizel            #+#    #+#              #
#    Updated: 2024/03/19 13:54:30 by aoizel           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


NAME			=	webserv

RAW_SOURCES		=	main.cpp VirtualServer.cpp Location.cpp WebServ.cpp Socket.cpp HTTPMessage.cpp

SOURCES_DIR		=	sources/

SOURCES			=	$(addprefix $(SOURCES_DIR), $(RAW_SOURCES))

INCLUDES_DIR	=	includes/

CPP_FLAGS		=	-Wall -Wextra -Werror -std=c++98

CPP				=	c++ $(CPP_FLAGS)

OBJECTS_DIR		=	.objs/

OBJECTS			=	$(addprefix $(OBJECTS_DIR),$(RAW_SOURCES:.cpp=.o))

DEPENDENCIES	=	Makefile

all:				$(OBJECTS_DIR)
						make $(NAME)

$(NAME):			$(OBJECTS_DIR) $(OBJECTS)
						$(CPP) $(OBJECTS) -o $@

$(OBJECTS_DIR):
					mkdir -p .objs/

$(OBJECTS_DIR)%.o:	$(SOURCES_DIR)%.cpp $(DEPENDENCIES)
						$(CPP) -c $< -I$(INCLUDES_DIR) -o $@

$(OBJECTS_DIR)%.d: %.cpp | $(OBJECTS_DIR)
						$(CPP) $< -MT -I$(INCLUDES_DIR) $(OBJECTS_DIR)$(<:.cpp=.o) -MM -MP -MF $@

clean:
						rm -rf $(OBJECTS_DIR)

fclean:				clean
						rm -rf $(NAME)

re:					fclean
						make all

.PHONY:				all clean fclean re

-include $(OBJECTS:.o=.d)
