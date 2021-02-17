#include "pch.h"
#include "Client_Network.h"

int Client_Network::getMilliCount() {
	timeb tb;
	ftime(&tb);
	int nCount = tb.millitm + (tb.time & 0xfffff) * 1000;
	return nCount;
}

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

void Client_Network::change_port(int port_) {
	port = port_;
}

void Client_Network::change_server(std::string serverIP_) {
	serverIP = serverIP_;
}

void Client_Network::change_id(int id_) {
	id = id_;
}

void Client_Network::change_name(std::string name_) {
	name = name_;
}

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
	data = std::to_string(id) + " " +
		std::to_string(getMilliCount()) + " " +
		name + " #" +
		data;
	socket.send(data.c_str(), data.size() + 1, serverIP, port);
}

std::string Client_Network::receive() {
	for (int i = 0; i < 10; i++) {
		std::size_t received = 0;
		sf::IpAddress sender;
		unsigned short port_ = 0;
		socket.receive(buffer, sizeof(buffer), received, sender, port_);
	}
	return std::string(buffer);
}