#include "Network.h"

Network* Network::networkInstance = nullptr;

void Network::HandleNetworkMessages(RakNet::RakPeerInterface* peerInterface)
{
	for (packet = peerInterface->Receive(); packet; peerInterface->DeallocatePacket(packet), packet = peerInterface->Receive())
	{
		switch (packet->data[0])
		{
		case ID_CONNECTION_REQUEST_ACCEPTED:
			std::cout << "Request accepted \n";
			break;
		case ID_REMOTE_DISCONNECTION_NOTIFICATION:
			std::cout << "Another client has disconnected\n";
			break;
		case ID_REMOTE_CONNECTION_LOST:
			std::cout << "Another client has lost the connection\n";
			break;
		case ID_REMOTE_NEW_INCOMING_CONNECTION:
			std::cout << "Another client has connected\n";
			break;
		case ID_NO_FREE_INCOMING_CONNECTIONS:
			std::cout << "The server is full\n";
			break;
		case ID_DISCONNECTION_NOTIFICATION:
			std::cout << "We have been disconnected\n";
			break;
		case ID_CONNECTION_LOST:
			std::cout << "Connection lost\n";
			break;
		case (ID_USER_PACKET_ENUM + 1) :
			outputToconsole();
			break;
		default:
			std::cout << "Unknown Message: " << packet->data[0] << std::endl;
			break;
		}
	}
}

Network* Network::Initialize()
{
	networkInstance = new Network();
	return networkInstance;
}

void Network::InitializeRaknet()
{
	peerInterface = RakNet::RakPeerInterface::GetInstance();
	RakNet::SocketDescriptor socket;

	RakNet::StartupResult result = peerInterface->Startup(1, &socket, 1);

	if (result != RakNet::RAKNET_STARTED)
	{
		std::cout << "Error: ";
		if (result == RakNet::SOCKET_PORT_ALREADY_IN_USE)
			std::cout << "Port in use";
		else
			std::cout << result;
		std::cout << std::endl;
	}

	Connect();
	networkThread = std::thread([this] { while (true) { this->HandleNetworkMessages(this->peerInterface); }  });
}

void Network::Connect(const char* ip)
{
	RakNet::ConnectionAttemptResult result = peerInterface->Connect(ip, port, nullptr, 0);

	if (result == RakNet::CONNECTION_ATTEMPT_STARTED)
		std::cout << "Connecting" << std::endl;
	else
		std::cout << "error" << std::endl;
}

void Network::outputToconsole()
{
	RakNet::BitStream bitStream(packet->data, packet->length, false);
	bitStream.IgnoreBytes(sizeof(RakNet::MessageID));
	RakNet::RakString input;
	bitStream.Read(input);
	std::cout << input.C_String() << ":";

	bitStream.Read(input);
	std::cout << input.C_String() << std::endl;
}