#ifndef CHANNEL_HPP
#define CHANNEL_HPP

// include //

#include "../Server/Server.hpp"

// class //

class Channel
{
	private:
		std::string _name;
		std::string _password;
		std::string _Topic;
		std::vector<int> _authorized;
		std::vector<int> _operator_list;
		bool _invitation;
		bool _actif_password;
		int  _clients_limits;
		int  _sizeofClient;
		
	public:
		// constructor //
		Channel(std::string ChannelName);

		~Channel();
};op

#endif