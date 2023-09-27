#include "Bot.hpp"

static void ExecBot(int sockfd, std::string password)
{
	int buffer_size;

	bool Pass = false;
	bool Nick = false;
	bool User = false;

	char buffer[513];

	fd_set readfds;

	while (1)
	{
		FD_ZERO(&readfds);
		FD_SET(sockfd, &readfds);

		if (Pass == false)
		{
			send_msg(sockfd, PASS(password));
			Pass = true;
			sleep(1);
		}
		if (Nick == false)
		{
			send_msg(sockfd, NICK);
			Nick = true;
			sleep(1);
		}
		if (User == false)
		{
			send_msg(sockfd, USER);
			User = true;
			sleep(1);
		}

		buffer_size = recv(sockfd, buffer, sizeof(buffer), 0);

		if (buffer_size <= 0)
		{
			close(sockfd);
			return ;
		}

		TreatMessage(sockfd, buffer);

		clearBuffer(buffer, 513);
	}
}

int main(int argc, char **argv)
{
	int sockfd;
	int port;

	struct sockaddr_in serv_addr;

	if (argc != 3)
	{
		std::cerr << "Wrong arguments.\nExemple : ./IrcBot <port> <Server Password>" << std::endl;
		return (-1);
	}

	port = std::atoi(argv[1]);

	sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sockfd < 0) 
    {
		std::cerr << "Socket error" << std::endl;
		return (-1);
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port);

	if (inet_aton("127.0.0.1", &serv_addr.sin_addr) <= 0)
		return (0);

	if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
	{
		std::cout << "Connect Error" << std::endl;
        return (0);
	}

	ExecBot(sockfd, argv[2]);

	return (0);

}