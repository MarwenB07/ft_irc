#include "Server.hpp"

/* constructor */

// costructor //

Server::Server(int port, std::string password) : _port(port), _password(password)
{
	_servSocket = -1;
	_clientSocket = -1;
	_actually_connected = 0;
	_nbClient = 0;
	_serverName = "ft_irc.42";
	_initialTime = time(nullptr);
	_version = "0.1";
	_max = NB_CLIENTS;
}

/* function */

// check if my client send the good password //

int Server::tryPassword(std::vector<std::string> str, int socket)
{
	if (str.size() != 2)
		return (-1);

	std::vector<std::string>::iterator it = str.begin();

	if (*it != "PASS")
		return (-1);
	++it;
	if (*it != getPassword())
		return (send_msg(socket, ERR_PASS), 2);
	return (1);
}

// check if my client send a good nickname //

int Server::tryNick(std::vector<std::string> str, std::map<int, User *> user, int socket)
{
	std::string nickname;
	std::string none_first = "_0123456789";
	std::string unauthorised = " \t\f\v";
	
	if (str.size() != 2)
		return (-1);
	
	std::vector<std::string>::iterator sit = str.begin();
	
	std::map<int, User *>::iterator it;
	
	if (*sit != "NICK")
		return (-1);
	
	++sit;
	nickname = *sit;
	
	// note : verifier les non-ascii ( #$@^&* etc ... ) //
	
	if (nickname.length() > 9 || nickname.length() < 3)
		return(send_msg(socket, ERR_NICKLENGTH), 2);
	for (int i = 0; none_first.c_str()[i]; i++)
	{
		if (nickname.find(none_first.c_str()[i]) < 1)
			return (send_msg(socket, ERR_NICK), 2);
	}
	for (int i = 0; unauthorised.c_str()[i]; i++)
	{
		if (nickname.find(unauthorised.c_str()[i]) <= nickname.length())
			return (send_msg(socket, ERR_NICK), 2);
	}
	for (it = user.begin(); it != user.end() ;++it)
	{
		if (it->second->getNickname() != "NULL_NICKNAME" && it->second->getNickname() == nickname && it->second->getClientSocket() != socket)
			return (send_msg(socket, ERR_NICKSAME), 2);
	}
	return (1);
}

// check if my client send a good username //

int Server::tryUser(std::vector<std::string> str, int socket)
{
	std::string none_first = "_0123456789";
	std::string unauthorised = " \t\r\f\v";
	std::string username;


	if (str.size() != 6)
		return (-1);

	std::vector<std::string>::iterator it = str.begin();

	if (*it != "USER")
		return (-1);
	++it;

	username = *it;
	for (int i = 0; none_first.c_str()[i]; i++)
	{
		if (username.find(none_first.c_str()[i]) < 1)
			return (send_msg(socket, ERR_USER), 2);
	}

	for (int i = 0; unauthorised.c_str()[i]; i++)
	{
		if (username.find(unauthorised.c_str()[i]) != std::string::npos)
			return (send_msg(socket, ERR_USER), 2);
	}

	++it;
	if (*it != "0")
		return (-1);

	++it;
	if (*it != "*")
		return (-1);

	++it;
	username = *it;
	
	for (unsigned int i = 0; i <= username.length(); i++)
	{
		for (unsigned int i = 'a'; i <= 'z'; i++)
		{
			if (username.find(i, 1) != std::string::npos || username.find(i - 32, 1) != std::string::npos)
				break;
			else if (i == 'z')
				return (send_msg(socket, ERR_USER), 2);
		}
	}

	++it;
	username = *it;

	for (unsigned int i = 0; i <= username.length(); i++)
	{
		for (int i = 'a'; i <= 'z'; i++)
		{
			if (username.find(i) != std::string::npos || username.find(i - 32) != std::string::npos)
				break;
			else if (i == 'z')
				return (send_msg(socket, ERR_USER), 2);
		}
	}
	return (1);
}

// init my server //

int Server::InitServer( void )
{
	struct sockaddr_in serverAddr;

	socklen_t serverAddrLen = sizeof(serverAddr);

	_servSocket = socket(AF_INET, SOCK_STREAM, 0);

	if (_servSocket < 0)
	{
		std::cerr << "Port Error" << std::endl;
        return (-1);
	}

	if (fcntl(_servSocket, F_SETFL, O_NONBLOCK) < 0)
	{
		std::cerr << "Fcntl Error" << std::endl;
		close(_servSocket);
        return (-1);
	}

	serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(getPort());
    serverAddr.sin_addr.s_addr = INADDR_ANY;

	// bind() = link an address with a socket //

	if (bind(getSocket(), (struct sockaddr*)&serverAddr, serverAddrLen) < 0)
	{
		std::cerr << "Bind Error" << std::endl;
		close(_servSocket);
        return (-1);
	}

	if (listen(getSocket(), serverAddr.sin_port) < 0)
	{
		std::cerr << "Listen Error" << std::endl;
		close(_servSocket);
        return (-1);
	}

	std::cout << START << std::endl; 

	return (0);
}

// execute my Server //

int Server::StartServer( void )
{
	fd_set readfds;

	FD_ZERO(&readfds);
    FD_SET(_servSocket, &readfds);

	struct sockaddr_in clientAddr;
	socklen_t clientAddrLen = sizeof(clientAddr);

	std::string message = WELCOME;
	std::string newChat;

	int cfd;
	int maxSocket = NB_CLIENTS;
	int returner = 0;

	char buffer[513];

	while (true) 
	{
		signal(SIGPIPE, SIG_IGN);

        fd_set currentfd = readfds;

        if (select(maxSocket + 1, &currentfd, NULL, NULL, NULL) < 0) 
		{
            std::cerr << "Error in select. 😞\n";
            return -1;
        }

        // Check for activity on the server socket (new client connection)
        if (FD_ISSET(_servSocket, &currentfd)) 
		{
            clientAddrLen = sizeof(clientAddr);
            cfd = accept(_servSocket, (struct sockaddr*)&clientAddr, &clientAddrLen);

            std::cout << "Connection established with client : socket[" << cfd << "]" << std::endl;

            FD_SET(cfd, &readfds);

			char clientIP[INET_ADDRSTRLEN];
    		inet_ntop(AF_INET, &(clientAddr.sin_addr), clientIP, INET_ADDRSTRLEN);

			_host = clientIP;

			cleanBuffer(clientIP, 512);

            if (cfd > maxSocket)
                maxSocket = cfd;

			_users.insert(std::make_pair(cfd, new User(cfd, clientIP)));

            std::string welcomeMessage = WELCOME;
            send(cfd, welcomeMessage.c_str(), welcomeMessage.length(), 0);
            _clients.push_back(cfd);
        }
		
        for (std::vector<int>::iterator it = _clients.begin(); it != _clients.end(); ++it)
		{
    		int clientSocket = *it;
			std::map<int, User *>::iterator itUser = _users.find(clientSocket);
            if (FD_ISSET(clientSocket, &currentfd)) 
			{
                _bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
				message = buffer;
				_message = message;
				_splited = s_split(message);
				//std::cout << visiblechar(buffer);
				if (itUser->second->getPasswordStatus() == false)
					returner = tryPassword(_splited, clientSocket);
				else if (itUser->second->getNickStatus() == false)
					returner = tryNick(_splited, _users, clientSocket);
				else
					returner = tryUser(_splited, clientSocket);
                if (_bytesRead <= 0) 
				{
					if (_bytesRead < 0)
                		std::cerr << "client[" << clientSocket <<"] was deconnected ... 😞"<< std::endl;
                    close(clientSocket);
					setClientConnected(1);
                    FD_CLR(clientSocket, &readfds);
					//_users.erase(std::remove(_users.begin(), _users.end(), clientSocket), _users.end());
					// *function efface l'user des channel* //
                    _clients.erase(std::remove(_clients.begin(), _clients.end(), clientSocket), _clients.end());
					break;
                }
				else if (_bytesRead > 0 && (!itUser->second->getPasswordStatus() || !itUser->second->getNickStatus() || !itUser->second->getUserStatus()))
				{
					if (_bytesRead > 512)
					{
						close(clientSocket);
						// supp le client user //
						continue;
					}
					else if (returner > 0 && itUser->second->getPasswordStatus() == false)
						PassStep(clientSocket, returner, itUser->second);
					else if (returner > 0 && itUser->second->getPasswordStatus() == true && itUser->second->getNickStatus() == false)
						NickStep(clientSocket, message, returner, itUser->second);
					else if (returner > 0 && itUser->second->getUserStatus() == false && itUser->second->getPasswordStatus() == true && itUser->second->getNickStatus() == true)
						UserStep(clientSocket, returner, itUser->second);
					else if (message == "HELPER\r\n" || message == "HELPER\n")
						send_msg(clientSocket, HELP_MESSAGE);
					else
						send_msg(clientSocket, ERR_USELESS);
					returner = 0;
				}
				else
				{
                    buffer[_bytesRead] = '\0';
					if (_bytesRead > 512)
					{
						send_msg(clientSocket, ERR_LENGTH);
						continue;
					}
					else if (message == "\r\n" || message == "\n")
						send_msg(clientSocket, INVALIDE);
					else
						ExectuteIrcCmd(clientSocket, message, getChannel());
                }
				cleanBuffer(buffer, 513);
				_splited.clear();
            }
        }
    }
	close(_servSocket);
	return (0);
}

/* geter */

// get for my port //

int Server::getPort(void) const
{
	return (_port);
}

// get max clients //

int Server::getMaxClient(void) const
{
	return (_max);
}

// get nb of client connected in my server //

int Server::getNbClient(void) const
{
	return (_nbClient);
}

// get for my password //

std::string Server::getPassword(void) const
{
	return (_password);
}

std::vector<Channel *> Server::getChannelClassList(void) const
{
	return (_channel_class_list);
}

std::string Server::getServerVersion(void) const
{
	return (_version);
}

std::string Server::getServerName(void) const
{
	return (_serverName);
}

// get my socket stat //

int Server::getSocket(void) const
{
	return (_servSocket);
}

// get the message //

std::string Server::getMessage(void) const
{
	return (_message);
}

// get the time in second of the start of my server //

time_t Server::getTime(void) const
{
	return (_initialTime);
}

// get my client socket stat //

int Server::getClientSocket(void) const
{
	return (_clientSocket);
}

std::vector<std::string> Server::getUserNickList(void) const
{
	return (_users_nick_list);
}

std::vector<User *> Server::getUserClassList(void) const
{
	return (_users_class_list);
}

// get my channel  //

std::map<std::string, Channel *> Server::getChannel(void) const
{
	return (_channel);
}

std::map<int, User *> Server::getUser(void) const
{
	return (_users);
}

void Server::UpNbClients(void)
{
	_nbClient += 1;
}

void Server::setClientConnected(int set)
{
	_actually_connected += set;
}

void Server::AddUserNickList(std::string set)
{
	_users_nick_list.push_back(set);
}

void Server::AddUserClassList(User *user)
{
	_users_class_list.push_back(user);
}

void Server::AddChannelClassList(Channel *channel)
{
	_channel_class_list.push_back(channel);
}

/* destructor */

// destructor //

Server::~Server()
{
	
}