#pragma once
#include "SFML/Network.hpp"
#include <thread>
#include "Map.h"

static sf::Uint8 idCounter = 0u;
struct NetPlayer
{
	short socketIndex = 88u;
	sf::Uint8 id = 0;
    sf::Uint16 score = 0;
	sf::Vector2f position = sf::Vector2f(492942.f,29595229.f);
	sf::Vector2f velocity = sf::Vector2f(0.0f, 0.0f);
	std::vector<sf::Uint8> colledtedTreasures;
	NetPlayer() {};
    NetPlayer(short socketIndex) :socketIndex(socketIndex), id(idCounter++){};
};

struct NetTreasure
{
	sf::Vector2f position;
	Item itemType;
	ItemState itemState;
	sf::Uint8 id;
};

// The most current server state.
struct GameNetState
{
	bool unread = false;
	std::vector<NetPlayer> players;
	std::vector<NetTreasure> treasures;
};

enum PacketType : sf::Uint8
{
	PacketUpdateGameState,
	PacketUpdatePositionToHost,
	PacketUpdateTreasure,
	PacketKnowYourself
};

class GameClient
{
public:
	void host();
	void joinDefault();
	void join(std::string ip);

	void update();
	void startAcceptingConnections(unsigned short port);
	void stopAcceptingConnections();
	void connectToHost(std::string ip, unsigned short port);
	void sendFrequentDataToHost();
	void heyIChangedTreasure(sf::Uint8 treasureId, sf::Vector2f position, ItemState state);
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
	void updateGameState(const GameNetState &packet);
	void updatePlayerPositionAndVelocity(short playerNumber, sf::Vector2f position, sf::Vector2f velocity, sf::Uint16 score);
	void updateTreasure(sf::Uint8 treasureId, ItemState state, sf::Vector2f position);
	std::thread *acceptConnectionsThread;
	sf::TcpSocket clientSocket;
};
