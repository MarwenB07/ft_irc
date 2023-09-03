#include "Channel.hpp"

Channel::Channel()
{

}

Channel::Channel(std::string ChannelName, User *user) : _name(ChannelName)
{
	_actif_password = false;
	_invitation = false;
	_topic = "";
	_channel_mode_list = "";
	_sizeofClient = 1;
	_clients_limits = 20;
	_operator_list.push_back(user);
	_authorized.push_back(user);
}

Channel::Channel( Channel const & src )
{
	*this = src;

	return ;
}

Channel& Channel::operator=(const Channel& rhs)
{
    if (this == &rhs) {
        return *this;
    }

    // Copie des membres simples
    _name = rhs._name;
    _password = rhs._password;
    _topic = rhs._topic;
    _invitation = rhs._invitation;
    _actif_password = rhs._actif_password;
    _clients_limits = rhs._clients_limits;
    _sizeofClient = rhs._sizeofClient;

    return *this;
}
// Add user into channel //

void Channel::AddToChannel(User *user, std::string name)
{
	int n_socket;
	std::string welcome_to_channel = name;
	welcome_to_channel.append(" JOIN #");
	welcome_to_channel.append(getChannelName());
	welcome_to_channel.append("\r\n");

	for (std::vector<User *>::iterator it = _authorized.begin(); it != _authorized.end(); ++it)
	{
		User *users = CpyUser(*it);
		n_socket = users->getClientSocket();
		if (user->getClientSocket() != n_socket)
			send(n_socket, welcome_to_channel.c_str(), welcome_to_channel.length(), 0);
		delete users;
	}
	send(user->getClientSocket(), welcome_to_channel.c_str(), welcome_to_channel.length(), 0);
	send(user->getClientSocket(), getChannelTopic().c_str(), getChannelTopic().length(), 0);
}

User *Channel::CpyUser(User *user)
{
	User *n_user = new User(*user);
	return (n_user);
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

std::vector<std::string> Channel::getBanList(void) const
{
	return (_BanList);
}

bool Channel::getChannelInvitation(void) const
{
	return(_invitation);
}

bool Channel::getChannelActifPass(void) const
{
	return(_actif_password);
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

void Channel::setChannelActifPass(bool set)
{
	this->_actif_password = set;
}

void Channel::AddChannelAuthorized(User *user)
{
	this->_authorized.push_back(user);
}

void Channel::AddBanUser(User *user)
{
	this->_BanList.push_back(user->getNickname());
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

Channel::~Channel()
{

}