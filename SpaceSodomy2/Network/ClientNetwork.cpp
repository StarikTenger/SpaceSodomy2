#include "pch.h"
#include "ClientNetwork.h"
#include <thread> 

//basic constructors
ClientNetwork::ClientNetwork() {
	socket.setBlocking(0);
}

ClientNetwork::ClientNetwork(std::string serverIP_, int port_, int id_, std::string name_, int token_) {
	port = port_;
	serverIP = serverIP_;
	id = id_;
	name = name_;
	socket.setBlocking(0);
	socket_receiving.setBlocking(0);
	token = token_;
}

//Set modules
void ClientNetwork::set_port(int port_) {
	port = port_;
}

void ClientNetwork::set_server(std::string serverIP_) {
	serverIP = serverIP_;
}

void ClientNetwork::set_id(int id_) {
	id = id_;
}

void Client_Network::set_name(std::string name_) {
	name = aux::censor_name(name_);
}

void ClientNetwork::set_token(int token_) {
	token = token_;
}

//Get modules
int ClientNetwork::get_port() {
	return port;
}

std::string ClientNetwork::get_serverIP() {
	return serverIP;
}

int ClientNetwork::get_id() {
	return id;
}

std::string ClientNetwork::get_name() {
	return name;
}
int ClientNetwork::get_token() {
	return token;
}

void Client_Network::send(std::string data) {
	// Client message constructor
	data = std::to_string(id) + " " +
		std::to_string(aux::get_milli_count()) + " " +
		name + " " + std::to_string(token) + " " +
		data;
	// Sending
	auto func = [](sf::UdpSocket* socket, std::string data,
		std::string serverIP, int port) {
		socket->send(data.c_str(), data.size() + 1, serverIP, port);
	};

	std::thread thread(func, &socket, data, serverIP, port);
	thread.detach();
}

void ClientNetwork::receive() {
	//Cycle used to keep network stack empty
	for (int i = 0; i < 10; i++) {
		std::size_t received = 0;
		sf::IpAddress sender;
		unsigned short port_ = 0;
		// buffer saves only last message
		socket.receive(buffer, sizeof(buffer), received, sender, port_);
	}	
}

std::string ClientNetwork::get_message() {
	return std::string(buffer);
}