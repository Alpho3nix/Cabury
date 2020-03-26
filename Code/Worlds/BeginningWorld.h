#pragma once
#include "BackGroundNode.h"
#include "CommandQueue.h"
#include <array>
#include "Enemy.h"
#include "PlayerData.h"
#include "UtilityFunc.h"
#include "WorldIdentifiers.h"

class Player;

template<typename S, typename ID, typename Context>
class Stack;

class BeginningWorld : private sf::NonCopyable
{
public:

	struct Context
	{
		Context(sf::RenderWindow& win, TexturesHolder& tex, FontsHolder& fonts, Player_Data& player);
		sf::RenderWindow* window;
		TexturesHolder* textures;
		FontsHolder* fonts;
		Player_Data* player;
		

	};

protected:

	bool viewCanMove = true;
	enum SceneLayers
	{
		Background, Interaction, PlayerChar, LayerCount
	};

	sf::RenderWindow& fWindow;
	sf::View fWorldView;
	sf::FloatRect fWorldBounds;

	TexturesHolder& fTextures;
	FontsHolder& fFonts;
	CommandQueue fCommands;
	std::array<SceneNode*, LayerCount> Layers;
	SceneNode fSceneGraph;

	Player* fPlayer;
	Player_Data* fPlayerData;
	int fPlayerHP = 200;

	float fFloorLevel;

	//not sure about this virtual
	/*virtual*/ void loadTextures();
	
	virtual void buildWorld();
	void transitionToWorld(Worlds::ID world);

	void getCoinsItem(std::vector<struct ItemType>& coins) const;
	void getHelmetsItem(std::vector<struct ItemType>& helmets) const;
	void getSwordsItem(std::vector<struct ItemType>& swords) const;
	std::vector<struct ItemType> getDiamonds() const;

	//consider using a container with less copying ?
	std::vector<std::vector<struct ItemType>> getMobItems() const;
	std::vector<std::vector<struct ItemType>> getBossItems() const;

	void requestWorldPush(Worlds::ID world);
	void requestWorldPop();
	void requestWorldClear();

private:

	void handleCollisions(sf::Time dt);
	bool isMatching(SceneNode::Pair& pair, unsigned int type1, unsigned int type2);
	virtual void handleMovement(sf::Time dt);
	void destroyProjectilesOutsideView(CommandQueue& commands);
	

	bool isPlayerCentered();

	bool fTransitionToWorld = false;
	bool fFinishTransitionToWorld = true;
	sf::Time fTransitionWorldDuration = sf::seconds(1.f);
	sf::Time fTransitionWorldAccu = sf::Time::Zero;

	sf::RectangleShape fTransitionWorldRect;
	Worlds::ID fWorldToTransitionTo = Worlds::ID::None;

	void boringStuff(sf::Time dt);

public:
	
	explicit BeginningWorld(Context context, Stack<BeginningWorld, Worlds::ID, Context>& stack);
	virtual ~BeginningWorld();

	virtual bool draw();
	virtual bool update(sf::Time dt);
	Utility::OutSide isOutOfTheWorld(const sf::FloatRect& pos) const;
	Utility::OutSide isOutOfTheView(const sf::FloatRect& pos) const;

	bool hasAlivePlayer() const;

	virtual bool handleEvent(const sf::Event& e);

private:

	Stack<BeginningWorld, Worlds::ID, Context>* fStack;
};

