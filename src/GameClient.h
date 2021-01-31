#pragma once
#include "SFML/Network.hpp"
#include <thread>


static sf::Uint8 idCounter = 0u;
struct NetPlayer
{
	short socketIndex = 88u;
	sf::Uint8 id = 0;
	sf::Vector2f position = sf::Vector2f(492942.f,29595229.f);
	sf::Vector2f velocity = sf::Vector2f(0.0f, 0.0f);
	NetPlayer() {};
    NetPlayer(short socketIndex) :socketIndex(socketIndex), id(idCounter++){};
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
	PacketKnowYourself
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
	void sendFrequentDataToHost();
	void sendGameState(GameNetState state);
	NetPlayer *getMyPlayer();
	static GameClient& getClient();
	static bool imHost;
	static bool connectedToHost;
	static bool acceptingConnections;
	static bool gameOn;
	static short connectedClientAmount;
	static GameNetState gameNetState;
	static sf::Uint8 myPlayerId;
private:
	void receivePacket(sf::TcpSocket& socket, const short socketIndex);
	void resetState();
	void updateGameState(GameNetState packet);
	void updatePlayerPositionAndVelocity(short playerNumber, sf::Vector2f position, sf::Vector2f velocity);
	std::thread *acceptConnectionsThread;
	sf::TcpSocket clientSocket;
};
