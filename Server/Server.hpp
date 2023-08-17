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
#include <sstream>
#include "../User/User.hpp"

// define //

# define NB_CLIENTS 10

# define HELP_MESSAGE "\nIn order :\n\n\
PASS <password>\n\
NICK <nickname>\n\
USER <username> 0 * :<realname> Exemple : USER guest 0 * :Bart Simpson\n\r\n"

// presentation //

# define IRC_WELCOME "--- [ Welcome to IRC Serve ] ---\n\r\n"
# define IRC_INFO_LIST "SOCKET    NICKNAME\r\n"

// good //

# define START	  "\033[1;35m[IRC SERVER]\033[0;0m : Waiting connexion ..."
# define WELCOME  "Welcome to ft_irc server\r\n"
# define GoodPass "Good Password\r\n"
# define GoodNick "Good Nickname\r\n"
# define GoodUser "Good Usernamen\n\r\n"

// bad //

# define ERR_PASS "Bad password.\r\n"

# define ERR_NICKLENGTH "Invalide length for your nickname. (to allow 3 - 9 character)\r\n"
# define ERR_NICK       "Invalide nickname\r\n"
# define ERR_NICKSAME   "This nickname already exist, please retry with another nickname.\r\n"

# define ERR_USER       "Invalide username.\r\n"

# define ERR_LENGTH     "Invalide length.\r\n"
# define ERR_USELESS    "Invalide message, you can use 'HELPER' to have many information.\r\n"

// CMD //

# define INVALIDE "Invalide commande\r\n" 

// PRIVMSG //

# define PRIVMSG_ERR "Invalide commande PRIVMSG\r\n" 
# define PRIVMSG_ERR_USER(name) (name + " doesn't exist\r\n")
# define PRIVMSG(nick, ClientUser, message) (":" + nick + " PRIVMSG " + ClientUser + " " + message)

// Colors //

# define GREY   "\033[1;30m"
# define RED    "\033[1;31m"
# define GREEN  "\033[1;32m"
# define YELLOW "\033[1;33m"
# define BLUE   "\033[1;36m"
# define PURPLE "\033[1;35m"
# define WHITE  "\033[1;37m"
# define END	   "\033[0;0m"

// class //

class User;

class Server
{
	private:
		int _port;
		time_t _initialTime;
		std::string _password;
		std::string _message;
		int _servSocket;
		int _clientSocket;
		int _nbClient;
		std::vector<std::string> _splited;
		std::map<int, User> _users;
		std::vector<int> _clients;
		int _bytesRead;

		// private function //

		int tryPassword(std::vector<std::string> str, int socket);
		int tryNick(std::vector<std::string> str, std::map<int, User> user, int socket);
		int tryUser(std::vector<std::string> str, int socket);
		std::vector<std::string> s_split(std::string str);
		
		// semi-usefull //

		void cleanBuffer(char *buffer, int len);
		std::string makeIdenticalChat(char *buffer, std::string name);
		std::string Print(std::string string) const;

		// ServerHelp //

		std::string s_itoa(int number);
		void WelcomeToIrc(int socket);

		// Commande //

		int FindCmd(std::vector<std::string> message);
		int FindSocket(std::string name);
		void PrivateMsg(std::map<int, User> users, std::vector<std::string> buffer, int socket);

		// Execute //

		void ExectuteIrcCmd(int cmd, int socket, std::vector<std::string> split);

		// Join //

		void CreateChannel(void);
		
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
		std::string getMessage(void) const;
		int getClientSocket(void) const;
		int getSocket(void) const;
		time_t getTime(void) const;

		// usefull function //
		int InitServer( void );
		int StartServer( void );

		~Server();
};

std::string visiblechar(char *buffer);

#endif
