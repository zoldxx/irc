#include "../inc/server.hpp"

bool	Server::capLs(User & client, std::string cmd)
{
	(void)cmd;
	if (client.getStatus() == 0) 
		client.setStatus(1); 
	return 0;	
}
