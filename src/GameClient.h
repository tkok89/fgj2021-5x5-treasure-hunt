#pragma once
#include "SFML/Network.hpp"
#include <thread>

static bool imHost = false;
static bool connectedToHost = false;
static bool acceptingConnections = false;
static bool gameOn = false;
static short connectedClientAmount = 0u;

struct GameNetState
{
	std::vector<sf::Vector2f> playerPositions;
};

enum PacketType : sf::Uint8
{
	PacketUpdateGameState,
	PacketUpdatePositionToHost,
};

class GameClient
{
public:
	void host();
	void join();

	void update();
	void startAcceptingConnections(short port);
	void stopAcceptingConnections();
	void connectToHost(std::string ip, short port);
	void sendPosition(sf::Vector2f position);
	void sendGameState(GameNetState state);

private:
	void updateGameState(GameNetState packet);
	void updatePlayerPosition(short playerNumber, sf::Vector2f playerPosition);
	std::thread *acceptConnectionsThread;
	sf::TcpSocket clientSocket;
};
