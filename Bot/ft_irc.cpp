#include "Server/Server.hpp"

// Tu as des ennemies maintenan, date limite 06/08/2023 //

static int port_check(char *port)
{
	for (int i = 0; port[i]; i++)
	{
		if (!(port[i] >= '0' && port[i] <= '9'))
			return (-1);
	}

	int true_port = std::atoi(port);

	if (true_port > 65536)
		return (-1);
	return (true_port);
} 

int main(int argc, char **argv)
{
    // check d'argument //

    if (argc != 3)
    {
        std::cerr << "Argument Error\n" << "Exemple : ./ircserv <port> <password>" << std::endl;
        return (-1);
    }

	// port + password + check port //

	int port = port_check(argv[1]);
	std::string password = argv[2];

	if (port == -1)
    {
        std::cerr << "Port Error" << std::endl;
        return (-1);
    }

	Server IRC_Server = Server(port, password);

	IRC_Server.InitServer();
	
	IRC_Server.StartServer();

}