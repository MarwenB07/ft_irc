#include "Bot.hpp"

static void sended_bot_rep(int sockfd, std::string sender, int type, int num)
{
	if (type == 0)
	{
		if (num == 0)
			send_msg(sockfd, PRIVMSG_BOT(sender, FEUR_BASE));
		else if (num == 1)
			send_msg(sockfd, PRIVMSG_BOT(sender, FEUR_P2));
		else if (num == 2)
			send_msg(sockfd, PRIVMSG_BOT(sender, FEUR_P3));
	}
	else
	{
		if (num == 0)
			send_msg(sockfd, PRIVMSG_BOT(sender, PHRASE_1));
		else if (num == 1)
			send_msg(sockfd, PRIVMSG_BOT(sender, PHRASE_2));
		else if (num == 2)
			send_msg(sockfd, PRIVMSG_BOT(sender, PHRASE_3));
		else if (num == 3)
			send_msg(sockfd, PRIVMSG_BOT(sender, PHRASE_4));
		else if (num == 4)
			send_msg(sockfd, PRIVMSG_BOT(sender, PHRASE_5));
		else if (num == 5)
			send_msg(sockfd, PRIVMSG_BOT(sender, PHRASE_6));
	}
}

static void bot_send(int sockfd, std::string sender, std::string reponse, int c)
{
	int choice;
	int size = reponse.length();
	if (reponse.find("quoi") != std::string::npos || reponse.find("Quoi") != std::string::npos || reponse.find("QUOI") != std::string::npos)
	{
		choice = size % 3;
		sended_bot_rep(sockfd, sender, 0, choice);
	}
	else if (c == 0)
	{
		choice = size % 6;
		sended_bot_rep(sockfd, sender, 1, choice);
	}
}

void ReturnReponse(int sockfd, std::string line)
{
	std::vector<std::string> split = s_split(line);
	std::vector<std::string>::iterator it = split.begin();
	std::string message;
	std::string name;

	if (split.size() < 4)
		return ;

	message = takeMessage(line.c_str() + 1);
	message.erase(0, 1);
	name = (*it).c_str() + 1;
	++it;
	++it;
	if (*it == "BotFeur")
		bot_send(sockfd, name, message, 0);
	else
		bot_send(sockfd, *it, message, 1);
}

void IfKickedBot(int sockfd, std::string line)
{
	std::vector<std::string> split = s_split(line);
	std::vector<std::string>::iterator it = split.begin();
	std::string message;
	std::string name;
	int size;
	int num;

	if (split.size() < 4)
		return ;

	message = takeMessage(line.c_str() + 1);
	message.erase(0, 1);
	name = (*it).c_str() + 1;

	size = message.size();
	num = size % 5;

	if (num == 0)
		send_msg(sockfd, PRIVMSG_BOT(name, RAGE_1));
	else if (num == 1)
		send_msg(sockfd, PRIVMSG_BOT(name, RAGE_2));
	else if (num == 2)
		send_msg(sockfd, PRIVMSG_BOT(name, RAGE_3));
	else if (num == 3)
		send_msg(sockfd, PRIVMSG_BOT(name, RAGE_4));
	else if (num == 4)
		send_msg(sockfd, PRIVMSG_BOT(name, RAGE_5));
}

void InviteBot(int sockfd, std::string line)
{
	std::vector<std::string> split = s_split(line);
	std::vector<std::string>::iterator it = split.begin();
	if (split.size() != 4)
		return ;
	++it;
	++it;
	if (*it != "BotFeur")
		return ;
	++it;
	send_msg(sockfd, BOT_JOIN((*it)));
}