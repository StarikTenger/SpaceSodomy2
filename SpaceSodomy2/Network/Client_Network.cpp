#include "pch.h"
#include "Client_Network.h"

//Getting local time
int Client_Network::get_milli_count() {
	timeb tb;
	ftime(&tb);
	int nCount = tb.millitm + (tb.time & 0xfffff) * 1000;
	return nCount;
}

//basic constructors
Client_Network::Client_Network() {
	socket.setBlocking(0);
}

Client_Network::Client_Network(std::string serverIP_, int port_, int id_, std::string name_) {
	port = port_;
	serverIP = serverIP_;
	id = id_;
	name = name_;
	socket.setBlocking(0);
}

//Set modules
void Client_Network::set_port(int port_) {
	port = port_;
}

void Client_Network::set_server(std::string serverIP_) {
	serverIP = serverIP_;
}

void Client_Network::set_id(int id_) {
	id = id_;
}

void Client_Network::set_name(std::string name_) {
	name = name_;
}

//Get modules
int Client_Network::get_port() {
	return port;
}

std::string Client_Network::get_serverIP() {
	return serverIP;
}

int Client_Network::get_id() {
	return id;
}

std::string Client_Network::get_name() {
	return name;
}

void Client_Network::send(std::string data) {
	//Client message constructor
	data = std::to_string(id) + " " +
		std::to_string(get_milli_count()) + " " +
		name + " #" +
		data;
	//Sending
	socket.send(data.c_str(), data.size() + 1, serverIP, port);
}

std::string Client_Network::receive() {
	//Cycle used to keep network stack empty
	for (int i = 0; i < 10; i++) {
		std::size_t received = 0;
		sf::IpAddress sender;
		unsigned short port_ = 0;
		//buffer saves only last message
		socket.receive(buffer, sizeof(buffer), received, sender, port_);
	}
	// return basic string converted to string
	return std::string(buffer);
}