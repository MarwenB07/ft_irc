#ifndef CHANNEL_HPP
#define CHANNEL_HPP

// include //

#include "../Server/Server.hpp"

// Channel //

# define SUCCESS_JOIN(name) ("You join successfuly the channel #" + name + "\r\n")
# define CREATE_CHANNEL(channel) (":channel " + channel + " successfuly created\r\n")

// ERROR //

# define CREATE_ERROR "This channel parameter is invalide.\r\n"
# define INCORRECT_KEY "Invalide key for this channel.\r\n"
# define NOT_INVITED   "You haven't the authorization to join this channel.\r\n"
# define UNKNOW_CHANNEL(name) ("The channel " + name + " doesn't exist.\r\n") 
# define ALREADY_IN_CHANNEL(name) ("You are already in the channel #" + name + "\r\n")

// class //

class User;

class Channel
{
	private:
		std::string _name;
		std::string _channel_creator;
		std::string _password;
		std::string _topic;
		std::string _channel_mode_list;
		std::string _topic_seter;
		std::map<std::string, std::string> _user_mode;
		std::vector<User *> _invited;
		std::vector<User *> _authorized;
		std::vector<User *> _operator_list;
		time_t _creationTime;
		time_t _topicCreationTime;
		bool _invitation;
		bool _actif_password;
		bool _topic_restric;
		bool _client_have_limits;
		int  _clients_limits;
		int  _sizeofClient;

	public:
		// constructor //
		Channel();
		Channel( Channel const & src );
		Channel(std::string ChannelName, User *user);

		// operator // 
		Channel & operator=( Channel const & rhs );

		// other function //

		void AddToChannel(User *user, std::string name);
		void SendMsgToChannel(std::string message, int socket);
		bool VerifVector(std::vector<User *> all_users, User *user);

		// geter // const

		std::string getChannelName(void) const;
		std::string getChannelPass(void) const;
		std::string getChannelTopic(void) const;
		std::string getChannelMode(void) const;
		std::string getChannelCreator(void) const;
		std::string getTopicSeter(void) const;
		std::map<std::string,std::string> getUserModeList(void) const;
		std::vector<User *> getChannelAuthorized(void) const;
		std::vector<User *> getChannelOperator(void) const;
		std::vector<User *> getInvitedList(void) const;
		bool getChannelInvitation(void) const;
		bool getChannelActifPass(void) const;
		bool getChannelTopicRestric(void) const;
		bool getChannelHaveClientsLimits(void) const;
		int  getChannelClientsLimits(void) const;
		int  getChannelSizeofClients(void) const;
		time_t getCreationTime(void) const;
		time_t getTopicCreationTime(void) const;

		// seter //

		
		void setTopicCreationTime(void);
		void setTopicSeter(std::string set);
		void setChannelName(std::string set);
		void setChannelMode(std::string set);
		void setChannelPass(std::string set);
		void setChannelTopic(std::string set);
		void setChannelClientsLimits(int set);
		void setChannelSizeofClients(int set);
		void setChannelInvitation(bool set);
		void setChannelActifPass(bool set);
		void setChannelHaveClientsLimits(bool set);
		void setChannelTopicRestric(bool set);
		void DelChannelMode(std::string set);
		void DelOperator(User *user);
		void AddChannelAuthorized(User *user);
		void AddChannelOperator(User *user);
		void AddInvitedList(User *user);

		// Topic //

		void SendTopic(User *user);

		// Kick //

		void KickUser(User *user, User *sender, std::string reason);

		// Part //

		void PartChannel(User *user);

		// Join //

		void DeleteInvited(User *user);

		// other //

		void EraseChannelUser(User *user);
		void DeleteChannel(Channel *channel);
		void ChannelDeleteAll(void);

		~Channel();
};

#endif