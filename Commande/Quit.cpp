
#include "../Server/Server.hpp"


/*

QUIT :Gone to have lunch         ; Client exiting from the network
Message Example:

  :dan-!d@localhost QUIT :Quit: Bye for now!
                                   ; dan- is exiting the network with
                                   the message: "Quit: Bye for now!"

*/

void Server::Quit(User *user, std::map<std::string, Channel *> channel_list, std::string line)
{
	int socket = user->getClientSocket();
	std::string reason = "";
	std::vector<std::string> split = s_split(line);
	std::vector<std::string>::iterator it = split.begin();

	if (split.size() > 1 )
	{
		++it;
		if ((*it).find(":") < 2)
			reason = takeMessage(line);
	}
	if (line == "QUIT" || (split.size() > 1 && (*it).find(":") < 2))
	{
		(void)channel_list;
		JoinZero(user);
		close(socket);
		send_msg(_servSocket, QUIT(user->getNickname(), reason));
		_users_nick_list.erase(std::remove(_users_nick_list.begin(), _users_nick_list.end(), user->getNickname()), _users_nick_list.end());
		eraseUserInMap(user->getClientSocket());
	}
}