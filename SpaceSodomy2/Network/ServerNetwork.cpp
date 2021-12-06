#include <pch.h>
#include "ServerNetwork.h"

//basic constuctors
ServerNetwork::ServerNetwork() {
	socket.bind(8001);
	socket.setBlocking(0);
}

ServerNetwork::ServerNetwork(int port_) {
	socket.bind(port_);
	socket.setBlocking(0);
}

//Set methods
void ServerNetwork::set_port(int port_) {
	socket.bind(port_);
	socket.setBlocking(0);
}

void ServerNetwork::set_replay_path(std::string replay_path_) {
	replay_path = replay_path_;
}

//Get methods
std::deque<std::string> ServerNetwork::get_messages() {
	return messages;
}

std::string ServerNetwork::get_last_message() {
	if (messages.empty())
		return "";
	return messages.back();
}

//Delete methods
void ServerNetwork::del_last_message() {
	if (!messages.empty())
		messages.pop_back();
}

void ServerNetwork::clear_messages() {
	messages.clear();
}

void ServerNetwork::del_address(std::string address_) {
	addresses.erase(IPconvert[address_]);
}

void ServerNetwork::receive() {
	// Receiving
	socket.receive(buffer, sizeof(buffer), received, sender, port);
	if (received)
	{
		// Applying sender info
		addresses.insert(sender);
		ports[sender] = port;
		IPconvert[sender.toString()] = sender;

		// Applying received info
		std::string message_ = sender.toString() + " " + std::string(buffer); // adding IP address to message
		messages.push_back(message_);
		received = 0;
	}
}

void ServerNetwork::send(std::string message) {
	if (replay_path != "") {
		if (!fout.is_open())
			fout.open(replay_path, std::ios::binary);
		fout << message;
	}
	//send message for all users
	for (auto addr : addresses) {
		socket.send(message.c_str(), message.size() + 1, addr, ports[addr]);
	}
}