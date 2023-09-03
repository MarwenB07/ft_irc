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
		std::string _host;
		int 		_clientSocket;
		bool		_passwordStatus;
		bool		_nickStatus;
		bool		_userStatus;
		
	public:
		// constructor //
		User( void );
		User(int clientSocket, std::string host);
        User( User const & src );

		// operator //
		User & operator=( User const & rhs );
		
		// other //
		void CompleteUser(std::string nickname, std::string username);
		void MakeTrue(std::string statusName);

		// geter //  ç
		std::string getNickname(void) const;
		std::string getUsername(void) const;
		std::string	getUserHost(void) const;
		int 		getClientSocket(void) const;
		bool 		getNickStatus(void) const;
		bool 		getPasswordStatus(void) const;
		bool 		getUserStatus(void) const;
		

		~User();
};

#endif