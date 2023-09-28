
#include "../Server/Server.hpp"

void Server::Quit(User *user, std::string line)
{
	(void)user;
	std::string reason = "";
	std::vector<std::string> split = s_split(line);
	std::vector<std::string>::iterator it = split.begin();

	if (split.size() > 1 )
	{
		++it;
		if ((*it).find(":") < 2)
			reason = takeMessage(line, 1);
	}
	if (line == "QUIT" || (split.size() > 1 && (*it).find(":") < 2))
		;
}
