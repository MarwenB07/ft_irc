#ifndef SERVER_HPP
#define SERVER_HPP

// include //

#include <iostream>
#include <cstdlib>
#include <string>
#include <cstring>
#include <stdlib.h>
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

// presentation //

# define IRC_INFO_LIST "SOCKET    NICKNAME\r\n"

// bad //

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
		int _oldest;

		bool _bot;

		time_t _initialTime;

		std::string _old;
		std::string _host;
		std::string _version;
		std::string _password;
		std::string _Pass;
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

		std::string givePass(std::vector<std::string> str);
		int tryPassword(std::vector<std::string> str, int socket);
		int tryNick(std::vector<std::string> str, std::map<int, User *> user, int socket);
		int tryUser(std::vector<std::string> str, int socket, User *user);
		std::vector<std::string> s_split(std::string str);
		std::vector<std::string> newSplit(std::string s, std::string c);
		
		// semi-usefull //

		void cleanBuffer(char *buffer, int len);
		void eraseUserInMap(int socket);
		void eraseChanInMap(std::string chan);

		// ServerHelp //

		std::string s_itoa(int number);
		void WelcomeToIrc(int socket, User *user);
		std::string correctChar(std::string line, char c);
		void Ctrl_D_Join(char *buffer, std::string oldMess, std::string newMess);
		std::string createListOfMember(std::vector<User *> all_user, Channel *channel);
		void ChannelEraserInfo(User *user);
		void DeleteChannel(Channel *channel);

		// Commande //

		int FindCmd(std::vector<std::string> message);
		int FindSocket(std::string name);

		// PRIVMSG //

		std::vector<std::string> catch_nickname(std::vector<std::string> buffer);
		void PrivateMsg(std::map<int, User *> users, std::vector<std::string> buffer, int socket, std::map<std::string, Channel *> channelist);

		// TOPIC //

		void Topic(User *user, std::map<std::string, Channel *> channel, std::string line);

		// NAMES //

		void Names(User *user, std::map<std::string, Channel *> channel, std::string line);

		// Execute //

		void ExectuteIrcCmd(int socket, std::string message, std::map<std::string, Channel *> channel);

		// JOIN //

		void Join(int socket, std::vector<std::string> split, std::map<std::string, Channel *> channel);
		void JoinChannel(int socket, std::string nickname, std::string name, std::map<std::string, Channel *> channel, std::string pass);
		void CreateChannel(User *user, std::string name);
		void JoinZero(User *user);

		// KICK //

		void Kick(User *user, std::map<std::string, Channel *> channel, std::string line);

		// PART //

		void Part(User *user, std::map<std::string, Channel *> channel, std::string line);
		void PartOfChannel(User *user, std::map<std::string, Channel *> channel, std::string the_chan, std::string reason);

		// INVITE //

		void SendInvitedChannel(User *user, std::vector<Channel *> channel);
		void Invite(User *user, std::map<std::string, Channel *> channel, std::string line);

		// QUIT //

		void Quit(User *user, std::string line);

		// MODE //

		void Mode(User *user, std::map<std::string, Channel *> channel, std::string line);
		void Mode_i(Channel *channel, bool set);
		void Mode_t(Channel *channel, bool set);
		void Mode_k(Channel *channel, bool set, std::string setPass);
		void Mode_o(Channel *channel, std::string nick, bool set);
		void Mode_l(Channel *channel, int limits, bool set);
		void addMode(char mode, std::string key, Channel *channel);
		void delMode(char mode, std::string key, Channel *channel);
		char getModeType(char c);

		// Trash //

		int UserStep(int socket, int returner, User *user);
		int NickStep(std::string message, int returner, User *user);

		// Check //

		bool checkIsOperator(User *user, Channel *channel);
		bool AlreadyInChannel(User *user, Channel *channel);
		bool checkIsInvited(User *user, Channel *channel);
		bool checkNameOfChannel(std::string channel);
		bool checkIsCreator(User *user, Channel *channel);
		bool checkUserExist(std::string nickname);
		bool ChannelAlreadyExists(std::string channel, std::map<std::string, Channel *> channel_list, int c);
		bool checkChannelHaveActivePass(std::string name, std::map<std::string, Channel *> channel_list);

		// Add //

		void AddUserNickList(std::string set);
		void AddUserClassList(User *user);
		void AddChannelClassList(Channel *channel);
		void setClientConnected(int set);
		void UpNbClients(void);
		void DownNbClients(void);

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
		int getOldest(void) const;
		std::string getPass(void) const;
		std::string getOld(void) const;
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
