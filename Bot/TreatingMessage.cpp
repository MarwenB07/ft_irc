#include "Bot.hpp"

void TreatMessage(int sockfd, std::string line)
{
	std::vector<std::string> split;
	std::vector<std::string>::iterator it;

	std::vector<std::string> split_space;
	std::vector<std::string>::iterator it_space;
	if (line.find("\r\n") != std::string::npos)
		split = newSplit(line, "\r\n");
	else
		split = newSplit(line, "\n");

	for (it = split.begin(); it != split.end(); ++it)
	{
		if ((*it).find("INVITE") != std::string::npos)
			InviteBot(sockfd, *it);
		else if ((*it).find("PRIVMSG") != std::string::npos)
			ReturnReponse(sockfd, *it);
		else if ((*it).find("KICK") != std::string::npos)
			IfKickedBot(sockfd, *it);
	}
}