#pragma once

#include "gl_core_4_4.h"
#include <GLFW\glfw3.h>
#include <iostream>
#include <string>
#include <thread>

#include <raknet\RakPeerInterface.h>
#include <raknet\MessageIdentifiers.h>
#include <raknet\BitStream.h>

class Network
{
	static Network* networkInstance;
	RakNet::RakPeerInterface* peerInterface;
	RakNet::Packet* packet = nullptr;
	std::thread networkThread;

	const GLuint port = 15255;

	void HandleNetworkMessages(RakNet::RakPeerInterface* peerInterface);
	void outputToconsole();


public:
	static Network* Initialize();
	void InitializeRaknet();
	void Connect(const char* ip = "127.0.0.1");

};
