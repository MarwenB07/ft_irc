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

// define //

# define NB_CLIENTS 10

# define WELCOME "Welcome to ft_irc server sheeeeeesh\n"

# define HELP_MESSAGE "In order to login in the server\n\nPASS <password> : have an acces to the server\nNICK <nickname> \
(less than 9 character and 3 minimal only) : create your nickname \
(the other client can see your nickname)\nUSER <USERNAME> (less than \
20 character and 3 minimal only) : create your username \
(the other client can't see your username)\n"

// class //

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
