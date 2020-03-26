#pragma once
#include "ResourceHolder.hpp"
#include "ResourceIdentifiers.h"
#include "GameState.h"
#include "PlayerData.h"

class CaburyApplication
{
public:
	CaburyApplication();

	void run();

private:
	sf::RenderWindow fWindow;
	bool Focus;

	void registerStates();

	void loadTextures();

	FontsHolder fFonts;
	Stack<State, States::ID, State::Context> fStack;
	TexturesHolder fTextures;
	Player_Data fPlayerData;

	//statistics stuff
	sf::Text StatsText;
	sf::Time fElapsedStatsTime;
	unsigned int NumFrames;

	sf::Time FrameRate;

	void updateStats(sf::Time dt);
	void ProcessInput(sf::Time dt);
	void render();
};