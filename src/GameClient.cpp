#include "GameClient.h"

sf::TcpSocket sockets[8]
{
	sf::TcpSocket(),
	sf::TcpSocket(),
	sf::TcpSocket(),
	sf::TcpSocket(),
	sf::TcpSocket(),
	sf::TcpSocket(),
	sf::TcpSocket(),
	sf::TcpSocket()
};

bool GameClient::imHost = false;
bool GameClient::connectedToHost = false;
bool GameClient::acceptingConnections = false;
bool GameClient::gameOn = false;
short GameClient::connectedClientAmount = 0u;
GameNetState GameClient::gameNetState;

GameClient& GameClient::getClient()
{
	static GameClient gameclient;
	return gameclient;
}

void GameClient::resetState()
{
	imHost = false;
	connectedToHost = false;
	acceptingConnections = false;
	gameOn = false;
}

//sf::Packet& operator <<(sf::Packet& packet, const PacketType& packetType)
//{
//	return packet << packetType;
//}

void GameClient::host()
{
	startAcceptingConnections(50000);
}

void GameClient::join()
{
	if (connectedToHost)
	{
		printf("cannot join, already connected!\n");
		return;
	}
	if (imHost)
	{
		printf("cannot join while hosting\n");
		return;
	}

	connectToHost(std::string("192.168.2.84"), 50000);
}

sf::Packet& operator >>(sf::Packet& packet, PacketType& packetType)
{
	sf::Uint8 uint;
	packet >> uint;
	packetType = PacketType(uint);
	return packet;
}

sf::Packet& operator <<(sf::Packet& packet, const sf::Vector2f &floatVec)
{
	return packet << floatVec.x << floatVec.y;
}

sf::Packet& operator >>(sf::Packet& packet, sf::Vector2f &floatVec)
{
	return packet >> floatVec.x >> floatVec.y;
}

sf::Packet& operator <<(sf::Packet& packet, const GameNetState& state)
{
	packet << sf::Uint8(state.players.size());
	for (NetPlayer player : state.players)
		packet << player.position << player.id;
	return packet;
}

sf::Packet& operator >>(sf::Packet& packet, GameNetState& state)
{
	state.players.clear();

	sf::Uint8 playerAmount;
	packet >> playerAmount;

	for (sf::Uint8 number = 0; number < playerAmount; number++)
	{
		NetPlayer player;
		packet >> player.position >> player.id;
		state.players.push_back(player);
	}
	return packet;
}

// client received
void GameClient::updateGameState(GameNetState state)
{
	gameNetState = state;
}

// host received 
void GameClient::updatePlayerPosition(short socketIndex, sf::Vector2f playerPosition)
{
	for (NetPlayer &playah : gameNetState.players)
	{
		if(playah.socketIndex == socketIndex)
			playah.position = playerPosition;
	}
}

// Client -> host
void GameClient::sendPosition(sf::Vector2f position)
{
	if (imHost)
		return;

	sf::Packet positionPacket;
	positionPacket << PacketUpdatePositionToHost;
	positionPacket << position;
	clientSocket.send(positionPacket);
}

// Host -> client
void GameClient::sendGameState(GameNetState state)
{
	if (!imHost)
		return;
	
	sf::Packet sendPacket;
	sendPacket << PacketUpdateGameState;
	sendPacket << state;

	for (sf::TcpSocket &socket : sockets)
	{
		socket.send(sendPacket);
	}
}

void GameClient::update()
{
	// Stop accepting connections
	if (acceptConnectionsThread->joinable() && !acceptingConnections)
	{
		acceptConnectionsThread->join();
		delete acceptConnectionsThread;
		acceptConnectionsThread = nullptr;
	}

	short i = 0;
	for (sf::TcpSocket& socket : sockets)
	{
		const short socketIndex = i++;
		sf::Packet receivedPacket;
		socket.receive(receivedPacket);
		PacketType packetType;
		receivedPacket >> packetType;
		switch (packetType)
		{
		case PacketUpdateGameState:
		{
			GameNetState gameState;
			receivedPacket >> gameState;
			updateGameState(gameState);
			break;
		}
		case PacketUpdatePositionToHost:
			sf::Vector2f position;
			receivedPacket >> position;
			updatePlayerPosition(socketIndex, position);
			break;
		}
	}
}

void listenerThread(unsigned short port, bool &accept, short &clientAmount)
{
	sf::TcpListener listener;
	if (listener.listen(port) != sf::Socket::Done)
	{
		printf("I failed to bind to port %hu\n", port);
		return;
	}
	else
	{
		printf("Listening to port %hu . . .", port);
	}
	while (accept)
	{
		if (listener.accept(sockets[clientAmount]) != sf::Socket::Done)
		{
			printf("but I failed to listen my friends :(\n");
		}
		else
		{
			printf("New Friend appeared :-)\n Accepted connection from remote address %s \n", sockets[clientAmount].getRemoteAddress().toString().c_str());
			clientAmount++;
		}
	}
}

void GameClient::startAcceptingConnections(unsigned short port)
{
	if (acceptConnectionsThread != nullptr)
	{
		printf("Already accepting connections \n");
		return;
	}

	resetState();

	acceptingConnections = true;
	acceptConnectionsThread = new std::thread(listenerThread, port, std::ref(acceptingConnections), std::ref(connectedClientAmount));
}

void GameClient::stopAcceptingConnections()
{
	acceptingConnections = false;
}

void GameClient::connectToHost(std::string ip, unsigned short port)
{
	resetState();
	
	printf("Connecting to address %s:%hu ... ", ip.c_str(), port);
	sf::Socket::Status status = clientSocket.connect(ip, port);
	if (status != sf::Socket::Done)
	{
		printf("failed, error code %u :(\n", status);

		/* 
		Done,         ///< The socket has sent / received the data
        NotReady,     ///< The socket is not ready to send / receive data yet
        Partial,      ///< The socket sent a part of the data
        Disconnected, ///< The TCP socket has been disconnected
        Error         ///< An unexpected error happened
		*/
	}
	else
	{
		printf("success!\n");
		connectedToHost = true;
	}
}