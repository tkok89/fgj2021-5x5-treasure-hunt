#pragma once
#include "SFML/Network.hpp"
#include <thread>


static short idCounter = 0u;
struct NetPlayer
{
	short socketIndex = ~0u;
	short id = 0;
	sf::Vector2f position = sf::Vector2f(492942.f,29595229.f);
	NetPlayer::NetPlayer() {};
	NetPlayer::NetPlayer(short socketIndex) :socketIndex(socketIndex), id(idCounter++){};
};

// The most current server state.
struct GameNetState
{
	std::vector<NetPlayer> players;
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
	void startAcceptingConnections(unsigned short port);
	void stopAcceptingConnections();
	void connectToHost(std::string ip, unsigned short port);
	void sendPosition(sf::Vector2f position);
	void sendGameState(GameNetState state);
	static GameClient& getClient();
	static bool imHost;
	static bool connectedToHost;
	static bool acceptingConnections;
	static bool gameOn;
	static short connectedClientAmount;
	static GameNetState gameNetState;
private:
	void receivePacket(sf::TcpSocket& socket, const short socketIndex);
	void resetState();
	void updateGameState(GameNetState packet);
	void updatePlayerPosition(short playerNumber, sf::Vector2f playerPosition);
	std::thread *acceptConnectionsThread;
	sf::TcpSocket clientSocket;
};
