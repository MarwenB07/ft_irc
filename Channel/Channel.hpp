#ifndef CHANNEL_HPP
#define CHANNEL_HPP

// include //

#include "../Server/Server.hpp"

// Channel //

# define SUCCESS_JOIN(name) ("You join successfuly the channel #" + name + "\r\n")
# define JOIN_CHANNEL(name, channel) (":" + name + "JOIN #" + channel + "\r\n")
# define CREATE_CHANNEL(channel) (":channel #" + channel + " successfuly created\r\n")

// ERROR //

# define CREATE_ERROR "This channel parameter is invalide.\r\n"
# define INCORRECT_KEY "Invalide key for this channel.\r\n"
# define NOT_INVITED   "You haven't the authorization to join this channel.\r\n"
# define UNKNOW_CHANNEL(name) ("The channel #" + name + " doesn't exist.\r\n") 
# define ALREADY_IN_CHANNEL(name) ("You are already in the channel #." + name + "\r\n")

// class //

class Channel
{
	private:
		std::string _name;
		std::string _password;
		std::string _topic;
		std::vector<int> _invited;
		std::vector<int> _authorized;
		std::vector<int> _operator_list;
		bool _invitation;
		bool _actif_password;
		int  _clients_limits;
		int  _sizeofClient;
		
	public:
		// constructor //
		Channel(std::string ChannelName, int socket);

		// other function //

		void AddToChannel(int socket, std::string name);

		// geter //

		std::string getChannelName(void);
		std::string getChannelPass(void);
		std::string getChannelTopic(void);
		std::vector<int> getChannelAuthorized(void);
		std::vector<int> getChannelOperator(void);
		std::vector<int> getInvitedList(void);
		bool getChannelInvitation(void);
		bool getChannelActifPass(void);
		int  getChannelClientsLimits(void);
		int  getChannelSizeofClients(void);

		~Channel();
};

#endif