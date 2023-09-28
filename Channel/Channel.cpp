#include "Channel.hpp"

Channel::Channel()
{

}

Channel::Channel(std::string ChannelName, User *user) : _name(ChannelName)
{
	_channel_creator = user->getNickname();
	_actif_password = false;
	_invitation = false;
	_topic_restric = false;
	_topic = "\r\n";
	_channel_mode_list = "";
	_sizeofClient = 1;
	_clients_limits = -1;
	_client_have_limits = false;
	_creationTime = std::time(nullptr);
	_topicCreationTime = std::time(nullptr);
	_topic_seter = user->getNickname();
}

Channel::Channel( Channel const & src )
{
	*this = src;

	return ;
}

Channel& Channel::operator=(const Channel& rhs)
{
    if (this == &rhs)
        return *this;

    _name = rhs.getChannelName();
    _password = rhs.getChannelPass();
    _topic = rhs.getChannelTopic();
	_authorized = rhs.getChannelAuthorized();
    _invitation = rhs.getChannelInvitation();
	_operator_list = rhs.getChannelOperator();
    _actif_password = rhs.getChannelActifPass();
    _clients_limits = rhs.getChannelClientsLimits();
    _sizeofClient = rhs.getChannelSizeofClients();
	_channel_creator = rhs.getChannelCreator();
	_channel_mode_list = rhs.getChannelMode();
	_user_mode = rhs.getUserModeList();
	_invited = rhs.getInvitedList();
	_topic_restric = rhs.getChannelTopicRestric();
	_client_have_limits = rhs.getChannelHaveClientsLimits();
	_creationTime = rhs.getCreationTime();

    return *this;
}

// Add user into channel //

void Channel::AddToChannel(User *user, std::string name)
{
	// check client limit //
	// supp les invited //
	for (std::vector<User *>::iterator it = _authorized.begin(); it != _authorized.end(); ++it)
		if (user->getClientSocket() != (*it)->getClientSocket())
			send_msg((*it)->getClientSocket(), JOIN_CHANNEL(user->getNickname(), name));
	send_msg(user->getClientSocket(), JOIN_CHANNEL(user->getNickname(), _name));
	SendTopic(user);
	setChannelSizeofClients(getChannelSizeofClients() + 1);
}

void Channel::SendMsgToChannel(std::string message, int socket)
{
	for (std::vector<User *>::iterator socke = _authorized.begin(); socke != _authorized.end(); ++socke)
		if (socket != (*socke)->getClientSocket())
			send_msg((*socke)->getClientSocket(), message);
}

void Channel::SendTopic(User *user)
{
	if (getChannelTopic() == "\r\n")
		send_msg(user->getClientSocket(), RPL_NOTOPIC(user->getNickname(), getChannelName()));
	else
		send_msg(user->getClientSocket(), RPL_TOPIC(user->getNickname(), getChannelName(), getChannelTopic()));
	send_msg(user->getClientSocket(), RPL_TOPICWHOTIME(user->getNickname(), getChannelName(), getTopicSeter(), std::to_string(getTopicCreationTime())));
}

void Channel:: KickUser(User *user, User *sender, std::string reason)
{
	if (VerifVector(_operator_list, user) == true)
		_operator_list.erase(std::remove(_operator_list.begin(), _operator_list.end(), user), _operator_list.end());
	_authorized.erase(std::remove(_authorized.begin(), _authorized.end(), user), _authorized.end());
	send_msg(sender->getClientSocket(), KICKED(sender->getNickname(), getChannelName(), user->getNickname(), reason));
	send_msg(user->getClientSocket(), KICKED(sender->getNickname(), getChannelName(), user->getNickname(), reason));
}

void Channel::PartChannel(User *user)
{
	if (VerifVector(_operator_list, user) == true)
		_operator_list.erase(std::remove(_operator_list.begin(), _operator_list.end(), user), _operator_list.end());
	if (VerifVector(_authorized, user) == true)
		_authorized.erase(std::remove(_authorized.begin(), _authorized.end(), user), _authorized.end());
}

void Channel::EraseChannelUser(User *user)
{
	if (VerifVector(_operator_list, user) == true)
		_operator_list.erase(std::remove(_operator_list.begin(), _operator_list.end(), user), _operator_list.end());
	if (VerifVector(_invited, user) == true)
		_invited.erase(std::remove(_invited.begin(), _invited.end(), user), _invited.end());
	if (VerifVector(_authorized, user) == true)
		_authorized.erase(std::remove(_authorized.begin(), _authorized.end(), user), _authorized.end());
}

void Channel::DelOperator(User *user)
{
	if (VerifVector(_operator_list, user) == true)
		_operator_list.erase(std::remove(_operator_list.begin(), _operator_list.end(), user), _operator_list.end());
}

bool Channel::VerifVector(std::vector<User *> all_users, User *user)
{
	for (std::vector<User *>::iterator it = all_users.begin(); it != all_users.end(); ++it)
		if (user->getNickname() == (*it)->getNickname())
			return (true);
	return (false);
}

// geter //

std::string Channel::getChannelName(void) const
{
	return(_name);
}

std::string Channel::getChannelPass(void) const
{
	if (getChannelActifPass() == true)
		return(_password);
	return ("false");
}	

std::string Channel::getChannelTopic(void) const
{
	return(_topic);
}

std::string Channel::getChannelMode(void) const
{
	return(_channel_mode_list);
}

std::string Channel::getTopicSeter(void) const
{
	return (_topic_seter);
}

std::string Channel::getChannelCreator(void) const
{
	return (_channel_creator);
}

std::vector<User *> Channel::getChannelAuthorized(void) const
{
	return(_authorized);
}

std::vector<User *> Channel::getChannelOperator(void) const
{
	return(_operator_list);
}

std::vector<User *> Channel::getInvitedList(void) const
{
	return (_invited);
}

std::map<std::string,std::string>  Channel::getUserModeList(void) const
{
	return (_user_mode);
}

time_t Channel::getCreationTime(void) const
{
	return (_creationTime);
}

time_t Channel::getTopicCreationTime(void) const
{
	return (_topicCreationTime);
}

bool Channel::getChannelInvitation(void) const
{
	return(_invitation);
}

bool Channel::getChannelTopicRestric(void) const
{
	return(_topic_restric);
}

bool Channel::getChannelActifPass(void) const
{
	return(_actif_password);
}



bool Channel::getChannelHaveClientsLimits(void) const
{
	return(_client_have_limits);
}

int  Channel::getChannelClientsLimits(void) const
{
	return(_clients_limits);
}

int  Channel::getChannelSizeofClients(void) const
{
	return(_sizeofClient);
}

// set //

void Channel::setChannelName(std::string set)
{
	this->_name = set;
}

void Channel::setChannelPass(std::string set)
{
	this->_password = set;
}

void Channel::setChannelTopic(std::string set)
{
	this->_topic = set;
}

void  Channel::setChannelClientsLimits(int set)
{
	this->_clients_limits = set;
}

void  Channel::setChannelSizeofClients(int set)
{
	this->_sizeofClient = set;
}

void Channel::setChannelInvitation(bool set)
{
	this->_invitation = set;
}

void Channel::setChannelHaveClientsLimits(bool set)
{
	this->_client_have_limits = set;
}


void Channel::setChannelActifPass(bool set)
{
	this->_actif_password = set;
}

void Channel::setChannelTopicRestric(bool set)
{
	this->_topic_restric = set;
}

void Channel::setTopicCreationTime(void)
{
	this->_topicCreationTime = std::time(nullptr);
}

void Channel::setTopicSeter(std::string set)
{
	this->_topic_seter = set;
}

void Channel::AddChannelAuthorized(User *user)
{
	this->_authorized.push_back(user);
}

void Channel::AddChannelOperator(User *user)
{
	this->_operator_list.push_back(user);
}

void Channel::AddInvitedList(User *user)
{
	this->_invited.push_back(user);
}

void Channel::setChannelMode(std::string set)
{
	// check if mode is always here //
	_channel_mode_list.append(set);
}

void Channel::DelChannelMode(std::string set)
{
	std::string appen;
	std::string oldSet = _channel_mode_list;
	std::string newSet = "";

	for (int i = 0; oldSet.c_str()[i]; i++)
	{
		if (set.find(oldSet.c_str()[i]) == std::string::npos)
		{
			appen = (char)oldSet.c_str()[i];
			newSet.append(appen);
		}
	}
	_channel_mode_list = newSet;
}

void Channel::DeleteInvited(User *user)
{
	this->_invited.erase(std::remove(_invited.begin(), _invited.end(), user), _invited.end());
}

void Channel::ChannelDeleteAll(void)
{
	for (std::vector<User *>::iterator it = _authorized.begin(); it != _authorized.end(); ++it)
	{
		delete *it;
	}
	_authorized.clear();
	for (std::vector<User *>::iterator it = _operator_list.begin(); it != _operator_list.end(); ++it)
	{
		delete *it;
	}
	_operator_list.clear();
	for (std::vector<User *>::iterator it = _invited.begin(); it != _invited.end(); ++it)
	{
		delete *it;
	}
	_invited.clear();
}

Channel::~Channel()
{

}