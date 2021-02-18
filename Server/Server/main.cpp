#include <iostream>
#include <Game.h>
#include <Client_Network.h>
#include <Server_Network.h>

int main() {
	Ship game;

	//Network expample
	Client_Network client;
	Server_Network server;
	client.send("Hello");
	server.receive();
	std::cout << server.get_last_message();
}