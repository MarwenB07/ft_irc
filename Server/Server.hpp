#ifndef SERVER_HPP
#define SERVER_HPP

// include //

#include <iostream>
#include <cstdlib>
#include <string>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <poll.h>
#include <map>
#include <vector>
#include <thread>
#include <arpa/inet.h>
#include "../User/User.hpp"

// define //

# define NB_CLIENTS 10

# define HELP_MESSAGE "\nIn order :\n\n\
\033[1;32mPASS\033[0;0m \033[1;31m<password>\033[0;0m\n\
\033[1;32mNICK\033[0;0m \033[1;31m<nickname>\033[0;0m\n\
\033[1;32mUSER\033[0;0m \033[1;31m<username> 0 * <realname>\033[0;0m\n\n"

// good //

# define START	  "\033[1;35m[IRC SERVER]\033[0;0m : Waiting connexion ..."
# define WELCOME  "\033[1;31mWelcome to ft_irc server\033[0;0m\n"
# define GoodPass "\033[1;32mGood Password\033[0;0m\n"
# define GoodNick "\033[1;32mGood Nickname\033[0;0m\n"
# define GoodUser "\033[1;32mGood Username\033[0;0m\n"

// bad //

# define ERR_NICKLENGTH "\033[1;31mInvalide length for your nickname. (to allow 3 - 9 character)\033[0;0m\n"
# define ERR_NICK       "\033[1;31mInvalide nickname.\033[0;0m\n"
# define ERR_NICKSAME   "\033[1;31mThis nickname already exist, please retry with another nickname.\033[0;0m\n"
# define ERR_LENGTH     "\033[1;31mInvalide length.\n"
# define ERR_USELESS    "\033[1;31mInvalide message, you can use 'HELP' to have many information.\033[0;0m\n"

// Colors //

#define GREY   "\033[1;30m"
#define RED    "\033[1;31m"
#define GREEN  "\033[1;32m"
#define YELLOW "\033[1;33m"
#define BLUE   "\033[1;36m"
#define PURPLE "\033[1;35m"
#define WHITE  "\033[1;37m"
#define END	   "\033[0;0m"

// class //

class User;

class Server
{
	private:
		int _port;
		time_t _initialTime;
		std::string _password;
		int _servSocket;
		int _clientSocket;
		int _nbClient;

		// private function //

		int tryPassword(std::string message);
		int tryNick(std::string message, std::map<int, User> user, int socket);
		int tryUser(std::string message);
		
		// semi-usefull //

		void cleanBuffer(char *buffer, int len);
		std::string makeIdenticalChat(char *buffer, std::string name);
		std::string Print(std::string string) const;
		
	public:
		// constructor //

		Server(int port, std::string password);

		// print //

		std::string getHelp(void) const;
		std::string getLengthError(void) const;
		std::string getUselessError(void) const;

		// geter //

		int getPort(void) const;
		int getNbClient(void) const;
		std::string getPassword(void) const;
		int getClientSocket(void) const;
		int getSocket(void) const;
		time_t getTime(void) const;

		// usefull function //
		int InitServer( void );
		int StartServer( void );

		~Server();
};

#endif
