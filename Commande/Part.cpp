#include "../Server/Server.hpp"

void Server::PartOfChannel(User *user, std::map<std::string, Channel *> channel, std::string)
{

}

void Server::Part(User *user, std::map<std::string, Channel *> channel, std::string line)
{
	 // PART #CHANNEL,<CHANNEL>
	std::string word;
	std::vector<std::string> channelname;
	std::vector<std::string> split = s_split(line);
	std::vector<std::string>::iterator w = split.begin();
	std::map<std::string, Channel *>::iterator it;

	++w;
	word = *w;
	channelname = newSplit(word, ",");
if (split.size() == 2)
	{
		for (std::vector<std::string>::iterator it = channelname.begin(); it != channelname.end(); ++it)
		{
			word = *it;
			word.erase(0, 1);
			if (ChannelAlreadyExists(word, channel, 1) == false)
			{
				send_msg(user->getClientSocket(), ERR_NOSUCHCHANNEL(user->getNickname(), word));
				continue;
			}
			Channel *chan = FindChannel(word);
			if (AlreadyInChannel(user, chan) == false)
			{
				send_msg(user->getClientSocket(), ERR_NOTONCHANNEL(user->getNickname(), *it));
				delete chan;
				continue;
			}
			PartOfChannel(user, channel, *it);
			delete chan;
		}
	}
}