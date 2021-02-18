#include <pch.h>
#include "Server_Network.h"

Server_Network::Server_Network() {
	socket.bind(8001);
	socket.setBlocking(0);
}

Server_Network::Server_Network(int port_) {
	socket.bind(port_);
	socket.setBlocking(0);
}

std::deque<std::string> Server_Network::get_messages() {
	return messages;
}

std::string Server_Network::get_last_message() {
	if (messages.empty())
		return "";
	return messages.back();
}

void Server_Network::del_last_message() {
	if (!messages.empty())
		messages.pop_back();
}

void Server_Network::clear_messages() {
	messages.clear();
}

std::set<sf::IpAddress> Server_Network::get_addresses() {
	return addresses;
}

void Server_Network::receive() {
	// Receiving
	socket.receive(buffer, sizeof(buffer), received, sender, port);
	if (received)
	{
		// Applying sender info
		addresses.insert(sender);
		ports[sender] = port;

		// Applying received info
		messages.push_back(std::string(buffer));
		received = 0;
	}
}

void Server_Network::send(std::string message) {
	for (auto addr : addresses) {
		socket.send(message.c_str(), message.size() + 1, addr, ports[addr]);
	}
}