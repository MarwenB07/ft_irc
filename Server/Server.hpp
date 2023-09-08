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
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <poll.h>
#include <map>
#include <vector>
#include <thread>
#include <arpa/inet.h>
#include <sstream>
#include "../User/User.hpp"
#include "../Channel/Channel.hpp"
#include "../Irc_msg.hpp"

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
# define PRIVMSG(nick, ClientUser, message) (":" + nick + " PRIVMSG " + ClientUser + " " + message + "\r\n")
# define PRIVMSG_CHANNEL(nick, channel, message) (":" + nick + " PRIVMSG " + channel + " " + message + "\r\n")

// class //

class User;
class Channel;

class Server
{
	private:
		int _port;
		int _handler;
		int _nbClient;
		int _bytesRead;
		int _servSocket;
		int _clientSocket;
		int _actually_connected;
		int _max;

		time_t _initialTime;

		std::string _host;
		std::string _version;
		std::string _password;
		std::string _message;
		std::string _serverName;
		
		std::map<int, User *> _users;
		std::map<std::string, Channel *> _channel;
		std::vector<int> _clients;
		std::vector<std::string> _users_nick_list;
		std::vector<std::string> _splited;
		std::vector<User *> _users_class_list;
		std::vector<Channel *> _channel_class_list;

		// private function //

		int tryPassword(std::vector<std::string> str, int socket);
		int tryNick(std::vector<std::string> str, std::map<int, User *> user, int socket);
		int tryUser(std::vector<std::string> str, int socket);
		std::vector<std::string> s_split(std::string str);
		std::vector<std::string> newSplit(std::string s, std::string c);
		
		// semi-usefull //

		void cleanBuffer(char *buffer, int len);
		std::string makeIdenticalChat(char *buffer, std::string name);

		// ServerHelp //

		std::string s_itoa(int number);
		void WelcomeToIrc(int socket, User *user);

		// Commande //

		int FindCmd(std::vector<std::string> message);
		int FindSocket(std::string name);

		// PRIVMSG //

		std::vector<std::string> catch_nickname(std::vector<std::string> buffer);
		void PrivateMsg(std::map<int, User *> users, std::vector<std::string> buffer, int socket, std::map<std::string, Channel *> channelist);

		// TOPIC //

		void Topic(User *user, std::map<std::string, Channel *> channel, std::string line);

		// Execute //

		void ExectuteIrcCmd(int socket, std::string message, std::map<std::string, Channel *> channel);

		// JOIN //

		void Join(int socket, std::vector<std::string> split, std::map<std::string, Channel *> channel);
		void JoinChannel(int socket, std::string nickname, std::string name, std::map<std::string, Channel *> channel);
		void CreateChannel(User *user, std::string name);

		// KICK //

		void Kick(User *user, std::map<std::string, Channel *> channel, std::string line);

		// PART //

		void Part(User *user, std::map<std::string, Channel *> channel, std::string line);
		void PartOfChannel(User *user, std::map<std::string, Channel *> channel, std::string);

		// INVITE //

		void SendInvitedChannel(User *user, std::vector<Channel *> channel);
		void Invite(User *user, std::map<std::string, Channel *> channel, std::string line);

		// MODE //

		// Trash //

		void UserStep(int socket, int returner, User *user);
		void NickStep(int socket, std::string message, int returner, User *user);
		void PassStep(int socket, int returner, User *user);

		// Check //

		bool checkIsOperator(User *user, Channel *channel);
		bool AlreadyInChannel(User *user, Channel *channel);
		bool checkIsInvited(User *user, Channel *channel);
		bool checkNameOfChannel(std::string channel);
		bool checkIsCreator(User *user, Channel *channel);
		bool checkUserExist(std::string nickname);
		bool ChannelAlreadyExists(std::string channel, std::map<std::string, Channel *> channel_list, int c);

		// Add //

		void AddUserNickList(std::string set);
		void AddUserClassList(User *user);
		void AddUserClassList(Channel *channel);
		void setClientConnected(int set);
		void UpNbClients(void);

		// Find //

		User *FindUser(std::string nickname);
		Channel *FindChannel(std::string channelname);

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
		int getClientSocket(void) const;
		int getSocket(void) const;
		int getMaxClient(void) const;
		std::string getPassword(void) const;
		std::string getMessage(void) const;
		std::string getServerName(void) const;
		std::string getServerVersion(void) const;
		std::vector<User *> getUserClassList(void) const;
		std::vector<Channel *> getChannelClassList(void) const;
		time_t getTime(void) const;
		std::vector<std::string> getUserNickList(void) const;
		std::map<std::string, Channel *> getChannel(void) const;
		std::map<int, User *> getUser(void) const;

		void sends_msg(int socket, std::string message, std::vector<User *> all, int open);

		// usefull function //
		int InitServer( void );
		int StartServer( void );

		~Server();
};

#endif
