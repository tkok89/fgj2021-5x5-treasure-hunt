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

void resetState()
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
	packet << sf::Uint8(state.playerPositions.size());
	for (sf::Vector2f position : state.playerPositions)
		packet << position;
	return packet;
}

sf::Packet& operator >>(sf::Packet& packet, GameNetState& state)
{
	sf::Uint8 playerAmount;
	packet >> playerAmount;
	
	for (sf::Uint8 number = 0; number < playerAmount; number++)
	{
		sf::Vector2f playerPosition;
		packet >> playerPosition;
		state.playerPositions.push_back(playerPosition);
	}
	return packet;
}

// Host -> client
void GameClient::updateGameState(GameNetState packet)
{

}

// Client -> host
void GameClient::updatePlayerPosition(short playerNumber, sf::Vector2f playerPosition)
{

}

// Client -> host
void GameClient::sendPosition(sf::Vector2f position)
{
	if (imHost)
		return;

	sf::Packet positionPacket;
	positionPacket << position;
	clientSocket.send(positionPacket);
}

// Host -> client
void GameClient::sendGameState(GameNetState state)
{
	if (!imHost)
		return;

	for (sf::TcpSocket &socket : sockets)
	{
		sf::Packet sendPacket;
		sendPacket << state;
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

	short playerNumber = 0;
	for (sf::TcpSocket& socket : sockets)
	{
		playerNumber++;

		sf::Packet receivedPacket;
		socket.receive(receivedPacket);
		PacketType packetType;
		receivedPacket >> packetType;
		switch (packetType)
		{
		case UpdateGameState:
		{
			GameNetState gameState;
			receivedPacket >> gameState;
			updateGameState(gameState);
			break;
		}
		case UpdatePositionToHost:
			sf::Vector2f position;
			receivedPacket >> position;
			updatePlayerPosition(playerNumber, position);
			break;
		}
	}
}

void listenerThread(short port)
{
	sf::TcpListener listener;
	if (listener.listen(port) != sf::Socket::Done)
	{
		printf("I failed to bind to port %u\n", port);
	}
	while (acceptingConnections)
	{
		if (listener.accept(sockets[connectedClientAmount]) != sf::Socket::Done)
		{
			printf("I failed to listen my friends :(\n");
		}
		else
		{
			connectedClientAmount++;
		}
	}
}

void GameClient::startAcceptingConnections(short port)
{
	if (acceptConnectionsThread != nullptr)
	{
		printf("Already accepting connections \n");
		return;
	}

	resetState();

	acceptingConnections = true;
	acceptConnectionsThread = new std::thread(listenerThread, port);
}

void GameClient::stopAcceptingConnections()
{
	acceptingConnections = false;
}

void GameClient::connectToHost(std::string ip, short port)
{
	resetState();
	
	printf("Connecting to address %s:%u ... ", ip.c_str(), port);
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