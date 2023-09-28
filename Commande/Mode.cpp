#include "../Server/Server.hpp"

/*

i : false

t : false

k : false

o : false

l : false

· i: Set/remove Invite-only channel
· t: Set/remove the restrictions of the TOPIC command to channel operators
· k: Set/remove the channel key (password)
· o: Give/take channel operator privilege
· l: Set/remove the user limit to channel

*/

void Server::Mode_i(Channel *channel, bool set)
{
	channel->setChannelInvitation(set);
	if (set == true)
		channel->setChannelMode("i");
	else
		channel->DelChannelMode("i");
}

void Server::Mode_t(Channel *channel, bool set)
{
	channel->setChannelTopicRestric(set);
	if (set == true)
		channel->setChannelMode("t");
	else
		channel->DelChannelMode("t");
}

void Server::Mode_k(Channel *channel, bool set, std::string setPass)
{
	if (set == false)
	{
		channel->setChannelActifPass(set);
		channel->DelChannelMode("k");
	}
	else if (set == true && setPass.length() > 0)
	{
		channel->setChannelPass(setPass);
		channel->setChannelActifPass(set);
		channel->setChannelMode("k");
	}
	else
		return ; // msg de coco
}

void Server::Mode_o(Channel *channel, std::string nick, bool set)
{
	std::string list = "";
	if (checkUserExist(nick) == false)
		return ; // caca
	User *user = FindUser(nick);
	if (AlreadyInChannel(user, channel) == false)
		return ; // caca
	else if (set == false && checkIsOperator(user, channel) == false)
		return ; // caca
	else if (set == true && checkIsOperator(user, channel) == false)
	{
		channel->AddChannelOperator(user);
		list = createListOfMember(channel->getChannelAuthorized(), channel);
		channel->setChannelMode("o");
		sends_msg(user->getClientSocket(), RPL_NAMREPLY(user->getNickname(), "=", channel->getChannelName() ,list), channel->getChannelAuthorized(), 0);
		return (sends_msg(user->getClientSocket(), RPL_ENDOFNAMES(user->getNickname(), channel->getChannelName()), channel->getChannelAuthorized(), 0));
	}
	else if (set == false && checkIsOperator(user, channel) == true)
	{
		channel->DelOperator(user);
		list = createListOfMember(channel->getChannelAuthorized(), channel);
		channel->DelChannelMode("o");
		sends_msg(user->getClientSocket(), RPL_NAMREPLY(user->getNickname(), "=", channel->getChannelName() ,list), channel->getChannelAuthorized(), 0);
		return (sends_msg(user->getClientSocket(), RPL_ENDOFNAMES(user->getNickname(), channel->getChannelName()), channel->getChannelAuthorized(), 0));
	}
}

void Server::Mode_l(Channel *channel, int limits, bool set)
{
	if (set == false)
	{
		channel->setChannelHaveClientsLimits(false);
		channel->DelChannelMode("l");
	}
	else if (set == true && limits > channel->getChannelClientsLimits())
	{
		channel->setChannelHaveClientsLimits(true);
		channel->setChannelClientsLimits(limits);
		channel->setChannelMode("l");
	}
}

char Server::getModeType(char c)
{
	if (c == 'k')
		return ('k');
	else if (c == 'o')
		return ('o');
	else if (c == 'i')
		return ('i');
	else if (c == 'l')
		return ('l');
	else if (c == 't')
		return ('t');
	return ('u'); // for unknow
}

void Server::addMode(char mode, std::string key, Channel *channel)
{

	if (mode == 'i')
		Mode_i(channel, true);
	else if (mode == 't')
		Mode_t(channel, true);
	else if (mode == 'k')
		Mode_k(channel, true, key);
	else if (mode == 'o')
		Mode_o(channel, key, true);
	else if (mode == 'l')
	{
		int lim = atoi(key.c_str());
		Mode_l(channel, lim, true);
	}
}	

void Server::delMode(char mode, std::string key, Channel *channel)
{
	if (mode == 'i')
		Mode_i(channel, false);
	else if (mode == 't')
		Mode_t(channel, false);
	else if (mode == 'k')
		Mode_k(channel, false, key);
	else if (mode == 'o')
		Mode_o(channel, key, false);
	else if (mode == 'l')
		Mode_l(channel, 0, false);
}

// k o l

void Server::Mode(User *user, std::map<std::string, Channel *> channel, std::string line)
{
	int status = 0;
	int i = 0;
	char c;
	std::string unknow = "";
	std::string word;
	std::string add = "+";
	std::string del = "-";
	std::string mod = "";
	std::vector<std::string> split_list = s_split(line);
	std::vector<std::string>::iterator list = split_list.begin();

	if (line == "MODE")
		return (send_msg(user->getClientSocket(), ERR_NEEDMOREPARAMS(user->getNickname(), (*list))));

	++list;
	word = *list;

	if (split_list.size() < 2)
		return ;

	if (ChannelAlreadyExists(word, channel, 0) == false)
		return ;
	Channel *chan = FindChannel(word.c_str() + 1);
	if (split_list.size() == 2)
	{
		if (chan->getChannelMode() == "")
			send_msg(user->getClientSocket(), RPL_CHANNELMODEIS(user->getNickname(), chan->getChannelName(), "", chan->getChannelMode()));
		else
			send_msg(user->getClientSocket(), RPL_CHANNELMODEIS(user->getNickname(), chan->getChannelName(), "+", chan->getChannelMode()));
		send_msg(user->getClientSocket(), RPL_CREATIONTIME(user->getNickname(), chan->getChannelName(), std::to_string(chan->getCreationTime())));
	}
	if (split_list.size() < 3)
		return ;
	else if (checkIsOperator(user, chan) == false)
		return (send_msg(user->getClientSocket(), ERR_CHANOPRIVSNEEDED(user->getNickname(), word)));

	++list;
	word = *list;

	if (word.find("+") == std::string::npos && word.find("-") == std::string::npos)
		return ;
	else if (word.find("+") > 2 && word.find("-") > 2)
		return ;

	for (; word.c_str()[i] ;)
	{
		if (word.c_str()[i] == '+')
		{
			status = 1;
			i++;
		}
		else if (word.c_str()[i] == '-')
		{
			status = 2;
			i++;
		}
		c = getModeType(word.c_str()[i]);
		if (c == 'u')
		{
			i++;
			send_msg(user->getClientSocket(), ERR_UNKNOWNMODE(user->getNickname(), c));
			continue;
		}
		else
		{
			if (status == 2)
			{
				mod = c;
				del = "-";
				del.append(mod);
				if (c == 'i')
					delMode(c, unknow, chan);
				else if (c == 't')
					delMode(c, unknow, chan);
				else if (c == 'k')
					delMode(c, unknow, chan);
				else if (c == 'l')
					delMode(c, unknow, chan);
				else if (c == 'o')
				{
					if (list + 1 != split_list.end())
						++list;
					delMode(c, *list, chan);
				}
				send_msg(user->getClientSocket(), MODE(user->getNickname(), chan->getChannelName(), del));
				
			}
			else if (status == 1)
			{
				mod = c;
				add = "+";
				add.append(mod);
				if (c == 'i')
					addMode(c, unknow, chan);
				else if (c == 't')
					addMode(c, unknow, chan);
				else if (c == 'k')
				{
					if (list + 1 != split_list.end())
						++list;
					addMode(c, *list, chan);
				}
				else if (c == 'l')
				{
					if (list + 1 != split_list.end())
						++list;
					addMode(c, *list, chan);
				}
				else if (c == 'o')
				{
					if (list + 1 != split_list.end())
						++list;
					addMode(c, *list, chan);
				}
				send_msg(user->getClientSocket(), MODE(user->getNickname(), chan->getChannelName(), add));
			}
			i++;
		}
	}
}