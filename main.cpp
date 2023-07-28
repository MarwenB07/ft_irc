#include <fstream>
#include <iostream>
#include <poll.h>

// Tu as des ennemies maintenan, date limite 06/08/2023 //

// port limite 65 536 //

// mdp hacher 

int main(int argc, char **argv)
{
    // check d'argument //

    // mettre un fd de 
    if (argc != 3)
    {
        std::cerr << "Argument Error\n" << "Exemple : ./ircserv <port> <password>" << std::endl;
        return (1);
    }

    // server base en fonction du port aui est mis //

    return (0);
}