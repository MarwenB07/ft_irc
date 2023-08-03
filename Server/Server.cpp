#include "Server.hpp"

/* constructor */

// Global variable to keep track of the SIGINT signal
volatile sig_atomic_t sigintReceived = 0;

// Signal handler for SIGINT (Ctrl+C)
static void Anti_Ctrl_C(int signum)
{
    if (signum == SIGINT)
    {
        sigintReceived = 1;
    }
}

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

// init my server //

int Server::InitServer( void )
{
	// en gros je pense aue les truc du genre client socket etc ce sera dans une class User donc pas maintenant //

	struct sockaddr_in serverAddr = {0};

	socklen_t serverAddrLen = sizeof(serverAddr);

	_servSocket = socket(AF_INET, SOCK_STREAM, 0);

	if (_servSocket < 0)
	{
		std::cerr << "Port Error" << std::endl;
        return (-1);
	}

	if (fcntl(_servSocket, 0, O_NONBLOCK) < 0)
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

	if (listen(getSocket(), NB_CLIENTS) < 0)
	{
		std::cerr << "Listen Error" << std::endl;
		close(_servSocket);
        return (-1);
	}

	std::cout << "start ..." << std::endl; 

	return (0);
}

// execute my Server //

int Server::StartServer( void )
{

	fd_set readfds;

	FD_ZERO(&readfds);
    FD_SET(getSocket(), &readfds);

	struct timeval timeout;
    timeout.tv_sec = 1;
    timeout.tv_usec = 0;

	struct sockaddr_in clientAddr = {0};
	socklen_t clientAddrLen = sizeof(clientAddr);

	std::string message = WELCOME;

	ssize_t sent;

	int Connexion = -1;

	while (1)
	{
		fd_set currentfds = readfds;

		char chat[512];

		int multipleActivity = select(getSocket() + 1, &currentfds, NULL, NULL, &timeout);

		if (multipleActivity < 0)
		{
			std::cerr << "MultipleActivity Error" << std::endl;
			close(_servSocket);
   			return (-1);
		}

		// ctrl c fais comme si on le kick //
		if (sigintReceived)
			break;

		std::cout << multipleActivity << std::endl;

		if (multipleActivity == 0)
			continue;

		int cfd = accept(_servSocket, (struct sockaddr*)&clientAddr, &clientAddrLen);
		_clientSocket = cfd;

		if (cfd < 0)
		{
			std::cerr << "Accept Error" << std::endl;
			close(_servSocket);
   			return (-1);
		}
		else
		{
			std::cout << "a connexion at " << time(NULL) - getTime() << " seconds detected by clientSocket [" << getClientSocket() << "]" << std::endl;
			_nbClient += 1;
			sent = send(cfd, message.c_str(), message.length(), 0);
		}

		// faire un tableau 

		Connexion = recv(cfd, chat, 512, MSG_PEEK);
		if (strlen(chat) > 512)
			sent = send(cfd, (getLengthError()).c_str(), (getLengthError()).length(), 0);
		if (Connexion == 0)
		{
			std::cout << "At " << time(NULL) - getTime() << " seconds clientSocket [" << getClientSocket() << "] was deconnected ..." << std::endl;
			_nbClient -= 1;
			close(_clientSocket);
			FD_CLR(_clientSocket, &readfds);
		}
		message = chat;
		if (message == "HELP\n")
			sent = send(cfd, (getHelp()).c_str(), (getHelp()).length(), 0);
		else if (tryPassword(message) == 1)
			std::cout << "Good password" << std::endl;
		else
			sent = send(cfd, (getUselessError()).c_str(), (getUselessError()).length(), 0);
	}

	close(_servSocket);
	return (0);
}

/* message */

// print all cmd (passord, login) //

std::string Server::getHelp(void) const
{
	return (HELP_MESSAGE);
}

// print error message when the client send a message with more character than 512 //

std::string Server::getLengthError(void) const
{
	return ("INVALIDE LENGTH\n");
}

// print error message when the client send a useless message //

std::string Server::getUselessError(void) const
{
	return ("INVALIDE MESSAGE, you can use 'HELP' to have many information\n");
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