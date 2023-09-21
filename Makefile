# ---- NAME ---- #

NAME = ircserv

# ---- COMPLILATION ---- #

CPP = c++

CXXFLAGS = -Wall -Wextra -Werror -std=c++98

# ---- SRC / OBJS ---- #

SRC =	ft_irc.cpp \
		Server/Server.cpp \
		Server/ServHelp.cpp \
		Server/ServerTrash.cpp \
		Server/AllCheck.cpp \
		Commande/Execute.cpp \
		Commande/Join.cpp \
		Commande/Topic.cpp \
		Commande/Part.cpp \
		Commande/Mode.cpp \
		Commande/Invite.cpp \
		Commande/PrivateMsg.cpp \
		Commande/Kick.cpp \
		Commande/Find.cpp \
		Commande/Quit.cpp \
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