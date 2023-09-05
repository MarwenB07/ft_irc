# ---- NAME ---- #

NAME = ircserv

# ---- COMPLILATION ---- #

CPP = @c++

CXXFLAGS = -Wall -Wextra -Werror -std=c++98

# ---- SRC / OBJS ---- #

SRC =	ft_irc.cpp \
		Server/Server.cpp \
		Server/ServHelp.cpp \
		Server/ServerTrash.cpp \
		Commande/Execute.cpp \
		Commande/Join.cpp \
		Commande/PrivateMsg.cpp \
		Commande/Find.cpp \
		User/User.cpp \
		Channel/Channel.cpp
#
OBJS = $(SRC:.cpp=.o)

# ---- COMPILATION ---- #

all : $(NAME)

$(NAME) : $(OBJS)
	@$(CPP) $(CXXFLAGS) $(OBJS) -o $(NAME) \

# ---- CLEAN ---- #

clean :
	rm -rf $(OBJS)

fclean : clean
	rm -rf $(NAME)

# ---- RE ---- #

re : fclean all