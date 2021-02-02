#include "GameClient.h"
#include "Global.h"

std::vector<sf::TcpSocket*> sockets;

bool GameClient::imHost = false;
bool GameClient::connectedToHost = false;
bool GameClient::acceptingConnections = false;
bool GameClient::gameOn = false;
short GameClient::connectedClientAmount = 0u;
sf::Uint8 GameClient::myPlayerId = 111u;
GameNetState GameClient::gameNetState;

GameClient& GameClient::getClient()
{
	static GameClient gameclient;
	return gameclient;
}

NetPlayer *GameClient::getMyPlayer()
{
	for (NetPlayer& playah : GameClient::gameNetState.players)
	{
		if (playah.id == myPlayerId)
			return &playah;
	}
	return nullptr;
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
	if (connectedToHost)
	{
		printf("Cannot host, already connected to host!\n");
		return;
	}
	
	startAcceptingConnections(50000);
}

void GameClient::joinDefault()
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
	// tomi
	//connectToHost(std::string("192.168.2.26"), 50000);
	// rike
	//connectToHost(std::string("192.168.2.43"), 50000);
    //janne
    //connectToHost(std::string("192.168.2.59"), 50000);
    //vesa
   // connectToHost(std::string("192.168.2.84"), 50000);
	// koti
	connectToHost(std::string("localhost"), 50000);
}

void GameClient::join(std::string ip)
{
	if (ip.empty())
	{
		joinDefault();
		return;
	}

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

	connectToHost(ip, 50000);
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
	{
		packet << player.position << player.velocity << player.id << player.score << player.randomId;
		packet << sf::Uint8(player.colledtedTreasures.size());
		for (sf::Uint8 treasureId : player.colledtedTreasures)
			packet << treasureId;
	}
	packet << sf::Uint8(state.treasures.size());
	for (NetTreasure treasure : state.treasures)
		packet << treasure.position << sf::Uint8(treasure.itemType) << sf::Uint8(treasure.itemState) << treasure.id;

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
		packet >> player.position >> player.velocity >> player.id >> player.score >> player.randomId;
		
		sf::Uint8 treasureAmount;
		packet >> treasureAmount;
		for (sf::Uint8 treasureNumbah = 0; treasureNumbah < treasureAmount; treasureNumbah++)
		{
			sf::Uint8 treasureId;
			packet >> treasureId;
			player.colledtedTreasures.push_back(treasureId);
		}
		state.players.push_back(player);
	}
	
	sf::Uint8 treasureAmount;
	packet >> treasureAmount;

	for (sf::Uint8 number = 0; number < treasureAmount; number++)
	{
		NetTreasure treasure;
		sf::Uint8 type;
		sf::Uint8 itemState;
		packet >> treasure.position >> type >> itemState >> treasure.id;
		treasure.itemState = ItemState(itemState);
		treasure.itemType = Item(type);
		state.treasures.push_back(treasure);
	}


	return packet;
}

// client received
void GameClient::updateGameState(const GameNetState &state)
{
	gameNetState = state;
	gameNetState.unread = true;
	unsigned short numberOfPlayer = 0u;

	for (const NetPlayer& player : state.players)
	{
		// printf("\t#%hu id: %hu x: %f, y: %f\n", numberOfPlayer++, player.id, player.position.x, player.position.y);
	}
}

// host received 
void GameClient::updatePlayerPositionAndVelocity(short socketIndex, sf::Vector2f position, sf::Vector2f velocity, sf::Uint16 score, sf::Uint16 randomId)
{
	gameNetState.unread = true;
	for (NetPlayer &playah : gameNetState.players)
	{
		if (playah.socketIndex == socketIndex)
		{
			playah.position = position;
			playah.velocity = velocity;
            playah.score = score;
			playah.randomId = randomId;
		}
	}
}

void GameClient::updateTreasure(sf::Uint8 treasureId, ItemState state, sf::Vector2f position)
{
	for (NetTreasure& netTreasure : gameNetState.treasures)
	{
		if (treasureId == netTreasure.id)
		{
			netTreasure.position = position;
			netTreasure.itemState = state;

			if (state == ItemState::Carried)
			{
				NetPlayer* playah = getMyPlayer();
				if (playah)
				{
					playah->colledtedTreasures.push_back(netTreasure.id);
				}
			}
			return;
		}
	}
}

void GameClient::heyIChangedTreasure(sf::Uint8 treasureId, sf::Vector2f position, ItemState state)
{
	if (imHost)
	{
		updateTreasure(treasureId, state, position);
		return;
	}
	sf::Packet treasurePacket;
	treasurePacket << PacketUpdateTreasure;
	treasurePacket << treasureId;
	treasurePacket << sf::Uint8(state);
	treasurePacket << position;

	clientSocket.send(treasurePacket);
}

// Client -> host
void GameClient::sendFrequentDataToHost()
{
	NetPlayer *me = GameClient::getClient().getMyPlayer();
	if (!me)
		return;

	if (imHost) 
		return;

	sf::Packet positionPacket;
	positionPacket << PacketUpdatePositionToHost;
	positionPacket << me->position;
	positionPacket << me->velocity;
	positionPacket << me->score;
	positionPacket << me->randomId;
	
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

	for (sf::TcpSocket *socket : sockets)
	{
		socket->send(sendPacket);
	}
}

void GameClient::update()
{
	// Stop accepting connections
	if (imHost && !acceptingConnections && acceptConnectionsThread->joinable())
	{
		acceptConnectionsThread->join();
		delete acceptConnectionsThread;
		acceptConnectionsThread = nullptr;
	}


	if (imHost)
	{
		for (unsigned i = 0; i < connectedClientAmount; i++)
		{
			receivePacket(*sockets[i], i);
		}
	}
	else if(connectedToHost)
	{
		receivePacket(clientSocket, 77);
	}
}

void GameClient::receivePacket(sf::TcpSocket &socket, const short socketIndex)
{
	sf::Packet receivedPacket;
	if (socket.receive(receivedPacket) != sf::Socket::Status::Done)
		return;

	if (receivedPacket.getDataSize() == 0u)
		return;

	PacketType packetType;
	receivedPacket >> packetType;
	switch (packetType)
	{
	case PacketUpdateGameState:
	{
		if (imHost)
		{
			printf("Host updated game state???? socket #%hu send packet, ignoring!\n Packet size %hu", socketIndex, receivedPacket.getDataSize());
		}
		GameNetState gameState;
		receivedPacket >> gameState;
		updateGameState(gameState);
		break;
	}
	case PacketUpdatePositionToHost:
	{
		sf::Vector2f position, velocity;
		sf::Uint16 score;
		sf::Uint16 randomId;
		receivedPacket >> position;
		receivedPacket >> velocity;
        receivedPacket >> score;
		receivedPacket >> randomId;
		updatePlayerPositionAndVelocity(socketIndex, position, velocity, score, randomId);
		break;
	}
	case PacketUpdateTreasure:
	{
		sf::Uint8 treasureId;
		sf::Uint8 treasureState;
		sf::Vector2f treasurePosition;
		
		receivedPacket >> treasureId;
		receivedPacket >> treasureState;
		receivedPacket >> treasurePosition;
		updateTreasure(treasureId, ItemState(treasureState), treasurePosition);
	}
	case PacketKnowYourself:
	{
		receivedPacket >> myPlayerId;
		printf("Mom i`m %hu\n", myPlayerId);
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
		sf::TcpSocket *socket = new sf::TcpSocket();
		sf::Socket::Status status = listener.accept(*socket);
	
		if(status == sf::Socket::Done)
		{
			socket->setBlocking(false);
			printf("New Friend appeared :-)\n Accepted connection from remote address %s \n", socket->getRemoteAddress().toString().c_str());
			GameClient::gameNetState.players.push_back(NetPlayer(sockets.size()));
			sf::Packet positionPacket;
			positionPacket << PacketKnowYourself;
			positionPacket << GameClient::gameNetState.players.back().id;
			socket->send(positionPacket);
			sockets.push_back(socket);
			clientAmount = sockets.size();
		}
		else
		{
			delete socket;
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

	g_map->randomize();
	resetState();

	imHost = true;
	acceptingConnections = true;
	gameNetState.players.push_back(NetPlayer(99));
	myPlayerId = 0u;
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
		clientSocket.setBlocking(false);
		connectedToHost = true;
	}
}
