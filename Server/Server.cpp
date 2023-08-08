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

int Server::tryPassword(std::string message)
{
	if (message.find("PASS") <= 512 && message.find(getPassword()) <= 512
		&& message.find(getPassword()) > message.find("PASS")
		&& message.length() == getPassword().length() + 6)
		return (1);
	return (-1);
}

// check if my client send a good nickname //

int Server::tryNick(std::string message, std::map<int, User> user, int socket)
{
	std::string nickname;
	std::string none_first = "_0123456789";
	std::string unauthorised = " \t\r\f\v";

	std::map<int, User>::iterator it;

	// note : verifier les non-ascii ( #$@^&* etc ... ) //

	if (message.find("NICK") == 0 && message.length() >= 8)
	{
		nickname = message.c_str() + 5;
		if (nickname.length() > 9)
		{
			send(socket, Print(ERR_NICKLENGTH).c_str(), Print(ERR_NICKLENGTH).length() + 1, 0);
			return (2);
		}	

		for (int i = 0; none_first.c_str()[i]; i++)
		{
			if (nickname.find(none_first.c_str()[i]) < 1)
			{
				send(socket, Print(ERR_NICK).c_str(), Print(ERR_NICK).length() + 1, 0);
				return (2);
			}
		}
		for (int i = 0; unauthorised.c_str()[i]; i++)
		{
			if (nickname.find(unauthorised.c_str()[i]) <= nickname.length())
			{
				send(socket, Print(ERR_NICK).c_str(), Print(ERR_NICK).length() + 1, 0);
				return (2);
			}
		}
		for (it = user.begin(); it != user.end() ;++it)
		{
			if (it->second.getNickname() != "NULL_NICKNAME" && it->second.getNickname() == nickname && it->second.getClientSocket() != socket)
			{
				send(socket, Print(ERR_NICKSAME).c_str(), Print(ERR_NICKSAME).length() + 1, 0);
				return (2);
			}
		}
		return (1);
	}
	return (-1);
}

// check if my client send a good username //

int Server::tryUser(std::string message)
{
	return (1);
}

// init my server //

int Server::InitServer( void )
{
	struct sockaddr_in serverAddr = {0};

	socklen_t serverAddrLen = sizeof(serverAddr);

	_servSocket = socket(AF_INET, SOCK_STREAM, 0);

	int on = 1;

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

	struct sockaddr_in clientAddr = {0};
	socklen_t clientAddrLen = sizeof(clientAddr);

	std::string message = WELCOME;
	std::string newChat;

	int cfd;
	int sent;
	int Connexion = -1;
	int maxSocket = NB_CLIENTS;
	int TmpClientSocket;
	int returner = 0;

	char buffer[512];

	std::vector<int> clients;
	std::map<int, User> users;
	while (true) 
	{
		signal(SIGPIPE, SIG_IGN);

        fd_set currentfd = readfds;

        if (select(maxSocket + 1, &currentfd, NULL, NULL, NULL) < 0) 
		{
            std::cerr << "Error in select.\n";
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

			users.insert(std::make_pair(cfd, User(cfd)));

            std::string welcomeMessage = WELCOME;
            send(cfd, welcomeMessage.c_str(), welcomeMessage.length() + 1, 0);
            clients.push_back(cfd);
        }
		
        for (std::vector<int>::iterator it = clients.begin(); it != clients.end(); ++it)
		{
    		int clientSocket = *it;
			std::map<int, User>::iterator itUser = users.find(clientSocket);
            if (FD_ISSET(clientSocket, &currentfd)) 
			{
                int bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
				returner = tryNick(buffer, users, clientSocket);
				message = buffer;
                if (bytesRead <= 0) 
				{
					if (bytesRead < 0)
                		std::cerr << "Error in recv from client: " << strerror(errno) << std::endl;
                    close(clientSocket);
                    FD_CLR(clientSocket, &readfds);
                    clients.erase(std::remove(clients.begin(), clients.end(), clientSocket), clients.end());
					break;
                }
				else if (bytesRead > 0 && (!itUser->second.getPasswordStatus() || !itUser->second.getNickStatus() || !itUser->second.getUserStatus()))
				{
					if (bytesRead > 512)
					{
						send(clientSocket, Print(ERR_LENGTH).c_str(), Print(ERR_LENGTH).length() + 1, 0);
						continue;
					}
					else if (tryPassword(buffer) == 1 && itUser->second.getPasswordStatus() == false)
					{
						itUser->second.MakeTrue("PASS");
						send(clientSocket, Print(GoodPass).c_str(), Print(GoodPass).length() + 1, 0);
					}
					else if (returner > 0 && itUser->second.getPasswordStatus() == true && itUser->second.getNickStatus() == false)
					{
						if (returner == 1)
						{
							itUser->second.MakeTrue("NICK");
							itUser->second.CompleteUser(message.c_str() + 5, itUser->second.getUsername());
							send(clientSocket, Print(GoodNick).c_str(), Print(GoodNick).length() + 1, 0);
						}
					}
					else if (tryUser(buffer) == 1 && itUser->second.getUserStatus() == false && itUser->second.getPasswordStatus() == true && itUser->second.getNickStatus() == true)
					{
						// pas fait encore

						itUser->second.MakeTrue("USER");
						itUser->second.CompleteUser(itUser->second.getNickname(), "Par defaut");
						send(clientSocket, Print(GoodUser).c_str(), Print(GoodUser).length() + 1, 0);
					}
					else if (message == "HELP\n")
						send(clientSocket, Print(HELP_MESSAGE).c_str(), Print(HELP_MESSAGE).length() + 1, 0);
					else
						send(clientSocket, Print(ERR_USELESS).c_str(), Print(ERR_USELESS).length() + 1, 0);
				}
				else 
				{
                    buffer[bytesRead] = '\0';
					if (bytesRead > 512)
					{
						send(clientSocket, Print(ERR_LENGTH).c_str(), Print(ERR_LENGTH).length() + 1, 0);
						continue;
					}
					itUser = users.find(clientSocket);
					newChat = makeIdenticalChat(buffer, itUser->second.getNickname());
                    for (std::vector<int>::iterator it = clients.begin(); it != clients.end(); ++it)
					{
    					TmpClientSocket = *it;
                        if (TmpClientSocket != clientSocket)
                            send(TmpClientSocket, newChat.c_str(), strlen(newChat.c_str()), 0);
                    }
                }
				cleanBuffer(buffer, 512);
            }
        }
    }
	close(_servSocket);
	return (0);
}

/* semi-usefull */

// print error message when the client send a message with more character than 512 //

std::string Server::Print(std::string string) const
{
	return (string);
}

// clean my buffer for recv //

void Server::cleanBuffer(char *buffer, int len)
{
	for (int i = 0; i < len; i++)
		buffer[i] = '\0';
}

// strjoin to reconize the client //

std::string Server::makeIdenticalChat(char *buffer, std::string name)
{
	std::string namedChat;

	namedChat = name.erase(name.length() - 1, name.length());
	namedChat.append(" : ");
	namedChat.append(buffer);

	return (namedChat);
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