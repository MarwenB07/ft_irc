#ifndef USER_HPP
#define USER_HPP

// include //

#include "../Server/Server.hpp"

// class //

class User
{
	private:
		std::string _nickname;
		std::string _username;
		int 		_clientSocket;
		bool		_status;
		
	public:
		// constructor //

		User(std::string nickname, std::string username, int clientSocket);

		// geter //

		std::string getNickname(void) const;
		std::string getUsername(void) const;
		int 		getClientSocket(void) const;
		bool		getStatus(void) const;

		~User();
};

#endif