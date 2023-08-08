# ---- NAME ---- #

NAME = ircserv

# ---- COMPLILATION ---- #

CPP = c++

CXXFLAGS = #-Wall -Wextra -Werror -std=c++98

# ---- SRC / OBJS ---- #

SRC =	ft_irc.cpp \
		Server/Server.cpp \
		User/User.cpp
#
OBJS = $(SRC:.cpp=.o)

# ---- COMPILATION ---- #

all : $(NAME)

$(NAME) : $(OBJS)
	@$(CPP) $(CXXFLAGS) $(OBJS) -o $(NAME)

# ---- CLEAN ---- #

clean :
	rm -rf $(OBJS)

fclean : clean
	rm -rf $(NAME)

# ---- RE ---- #

re : fclean all