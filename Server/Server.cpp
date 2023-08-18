#include "Server.hpp"

/* constructor */

// costructor //

Server::Server(int port, std::string password) : _port(port), _password(password)
{
	_servSocket = -1;
	_clientSocket = -1;
	_nbClient = 0;
	_initialTime = time(NULL);
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
	{
		send(socket, Print(ERR_PASS).c_str(), Print(ERR_PASS).length(), 0);
		return (2);
	}
	return (1);
}

// check if my client send a good nickname //

int Server::tryNick(std::vector<std::string> str, std::map<int, User> user, int socket)
{
	std::string nickname;
	std::string none_first = "_0123456789";
	std::string unauthorised = " \t\f\v";
	
	if (str.size() != 2)
		return (-1);
	
	std::vector<std::string>::iterator sit = str.begin();
	
	std::map<int, User>::iterator it;
	
	if (*sit != "NICK")
		return (-1);
	
	++sit;
	nickname = *sit;
	
	// note : verifier les non-ascii ( #$@^&* etc ... ) //
	
	if (nickname.length() > 9 || nickname.length() < 3)
	{
		send(socket, Print(ERR_NICKLENGTH).c_str(), Print(ERR_NICKLENGTH).length(), 0);
		return (2);
	}

	for (int i = 0; none_first.c_str()[i]; i++)
	{
		if (nickname.find(none_first.c_str()[i]) < 1)
		{
			send(socket, Print(ERR_NICK).c_str(), Print(ERR_NICK).length(), 0);
			return (2);
		}
	}

	for (int i = 0; unauthorised.c_str()[i]; i++)
	{
		if (nickname.find(unauthorised.c_str()[i]) <= nickname.length())
		{
			send(socket, Print(ERR_NICK).c_str(), Print(ERR_NICK).length(), 0);
			return (2);
		}
	}

	for (it = user.begin(); it != user.end() ;++it)
	{
		if (it->second.getNickname() != "NULL_NICKNAME" && it->second.getNickname() == nickname && it->second.getClientSocket() != socket)
		{
			send(socket, Print(ERR_NICKSAME).c_str(), Print(ERR_NICKSAME).length(), 0);
			return (2);
		}
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
		{
			send(socket, Print(ERR_USER).c_str(), Print(ERR_USER).length(), 0);
			return (2);
		}
	}

	for (int i = 0; unauthorised.c_str()[i]; i++)
	{
		if (username.find(unauthorised.c_str()[i]) != std::string::npos)
		{
			send(socket, Print(ERR_USER).c_str(), Print(ERR_USER).length(), 0);
			return (2);
		}
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
			{
				send(socket, Print(ERR_USER).c_str(), Print(ERR_USER).length(), 0);
				return (2);
			}
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
			{
				send(socket, Print(ERR_USER).c_str(), Print(ERR_USER).length(), 0);
				return (2);
			}
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

	char buffer[512];

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

            if (cfd > maxSocket)
                maxSocket = cfd;

			_users.insert(std::make_pair(cfd, User(cfd)));

            std::string welcomeMessage = WELCOME;
            send(cfd, welcomeMessage.c_str(), welcomeMessage.length(), 0);
            _clients.push_back(cfd);
        }
		
        for (std::vector<int>::iterator it = _clients.begin(); it != _clients.end(); ++it)
		{
    		int clientSocket = *it;
			std::map<int, User>::iterator itUser = _users.find(clientSocket);
            if (FD_ISSET(clientSocket, &currentfd)) 
			{
                _bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
				message = buffer;
				_message = message;
				_splited = s_split(message);
				if (itUser->second.getPasswordStatus() == false)
					returner = tryPassword(_splited, clientSocket);
				else if (itUser->second.getNickStatus() == false)
					returner = tryNick(_splited, _users, clientSocket);
				else
					returner = tryUser(_splited, clientSocket);
                if (_bytesRead <= 0) 
				{
					if (_bytesRead < 0)
                		std::cerr << "client[" << clientSocket <<"] was deconnected ... 😞"<< std::endl;
                    close(clientSocket);
                    FD_CLR(clientSocket, &readfds);
                    _clients.erase(std::remove(_clients.begin(), _clients.end(), clientSocket), _clients.end());
					break;
                }
				else if (_bytesRead > 0 && (!itUser->second.getPasswordStatus() || !itUser->second.getNickStatus() || !itUser->second.getUserStatus()))
				{
					if (_bytesRead > 512)
					{
						send(clientSocket, Print(ERR_LENGTH).c_str(), Print(ERR_LENGTH).length(), 0);
						continue;
					}
					else if (returner > 0 && itUser->second.getPasswordStatus() == false)
					{
						if (returner == 1)
						{
							itUser->second.MakeTrue("PASS");
							send(clientSocket, Print(GoodPass).c_str(), Print(GoodPass).length(), 0);
						}
					}
					else if (returner > 0 && itUser->second.getPasswordStatus() == true && itUser->second.getNickStatus() == false)
					{
						if (returner == 1)
						{
							itUser->second.MakeTrue("NICK");
							itUser->second.CompleteUser(message.c_str() + 5, itUser->second.getUsername());
							send(clientSocket, Print(GoodNick).c_str(), Print(GoodNick).length(), 0);
						}
					}
					else if (returner > 0 && itUser->second.getUserStatus() == false && itUser->second.getPasswordStatus() == true && itUser->second.getNickStatus() == true)
					{
						if (returner == 1)
						{
							std::vector<std::string>::iterator it = _splited.begin();
							++it;
							itUser->second.MakeTrue("USER");
							itUser->second.CompleteUser(itUser->second.getNickname(), *it);
							send(clientSocket, Print(GoodUser).c_str(), Print(GoodUser).length(), 0);
							WelcomeToIrc(clientSocket);
						}
					}
					else if (message == "HELPER\r\n" || message == "HELPER\n")
						send(clientSocket, Print(HELP_MESSAGE).c_str(), Print(HELP_MESSAGE).length(), 0);
					else
						send(clientSocket, Print(ERR_USELESS).c_str(), Print(ERR_USELESS).length(), 0);
					returner = 0;
				}
				else 
				{
                    buffer[_bytesRead] = '\0';
					if (_bytesRead > 512)
					{
						send(clientSocket, Print(ERR_LENGTH).c_str(), Print(ERR_LENGTH).length(), 0);
						continue;
					}
					else if (message == "\r\n" || message == "\n")
						send(clientSocket, Print(INVALIDE).c_str(), Print(INVALIDE).length(), 0);
					else
						ExectuteIrcCmd(FindCmd(_splited), clientSocket, _splited, _channel);
                }
				cleanBuffer(buffer, 512);
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

/* destructor */

// destructor //

Server::~Server()
{
	
}