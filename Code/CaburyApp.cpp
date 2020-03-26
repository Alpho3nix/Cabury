#include "CaburyApp.h"
#include "MenuState.h"
#include "PauseState.h"
#include "DeathState.h"
#include "SettingsState.h"

CaburyApplication::CaburyApplication()
	:fWindow(sf::VideoMode(WIDTH, HEIGHT), "Cabury", sf::Style::Close)
	, FrameRate(sf::seconds(1.f / 120.f)), StatsText(), fElapsedStatsTime(sf::Time::Zero), NumFrames(0), fFonts()
	, Focus(true), fPlayerData(), fStack(State::Context(fWindow, fTextures, fFonts, fPlayerData))
{
	fFonts.load("Fonts/Sansation.ttf", Fonts::ID::Sansation);
	fFonts.load("Fonts/GUI_Font.TTF", Fonts::ID::GUI);
	fFonts.load("Fonts/NPCSpeach.ttf", Fonts::ID::NPCSpeach);
	StatsText.setFont(fFonts.get(Fonts::ID::Sansation));
	StatsText.setCharacterSize(10U);

	loadTextures();

	registerStates();
	fStack.pushState(States::ID::Title);

	
}

void CaburyApplication::loadTextures()
{
	//Backgrounds
	fTextures.load("Textures/Backgrounds/Forest.png", Textures::ID::Forest);
	fTextures.load("Textures/Tavern/OutsideTavern.png", Textures::ID::TavernOutside);
	fTextures.load("Textures/Backgrounds/Kanalisation.png", Textures::ID::Kanalisation);
	fTextures.load("Textures/Tavern/TavernInteriorBack.png", Textures::ID::TavernInsideBack);
	fTextures.load("Textures/Tavern/TavernInteriorFront.png", Textures::ID::TavernInsideFront);
	fTextures.load("Textures/Tavern/CellarTavernClose.png", Textures::ID::TavernCellarClose);
	fTextures.load("Textures/Tavern/CellarTavernOpen.png", Textures::ID::TavernCellarOpen);
	fTextures.load("Textures/Backgrounds/MinotaurBG.png", Textures::ID::BossBG_Test);
	//Player textures
	fTextures.load("Textures/PlayerCharacter/Animations/Animations.png", Textures::ID::PlayerAnim);
	fTextures.load("Textures/PlayerCharacter/Inventory/Inventory.png", Textures::ID::PlayerInventory);
	//GUI textures
	fTextures.load("Textures/GUI/GUI.png", Textures::ID::Buttons);
	fTextures.load("Textures/GUI/Shop/Buttons.png", Textures::ID::ShopButtons);
	//Enemies textures
	fTextures.load("Textures/Enemies/Slime/slimeAnim.png", Textures::ID::Slime);
	fTextures.load("Textures/Enemies/Goblin/goblinAnim.png", Textures::ID::Goblin);
	fTextures.load("Textures/Enemies/Oculothorax/oculothoraxAnim.png", Textures::ID::Oculothorax);
	fTextures.load("Textures/Enemies/Ogre/ogreAnim.png", Textures::ID::Ogre);
	fTextures.load("Textures/Enemies/Wisps/earthAnim.png", Textures::ID::EarthWisp);
	fTextures.load("Textures/Enemies/Wisps/fireAnim.png", Textures::ID::FireWisp);
	fTextures.load("Textures/Enemies/Wisps/waterAnim.png", Textures::ID::WaterWisp);
	fTextures.load("Textures/Enemies/Wisps/windAnim.png", Textures::ID::WindWisp);
	fTextures.load("Textures/Enemies/Wisps/Projectiles/earth-projectileAnim.png", Textures::ID::EarthWispProjectile);
	fTextures.load("Textures/Enemies/Wisps/Projectiles/fire-projectileAnim.png", Textures::ID::FireWispProjectile);
	fTextures.load("Textures/Enemies/Wisps/Projectiles/water-projectileAnim.png", Textures::ID::WaterWispProjectile);
	fTextures.load("Textures/Enemies/Wisps/Projectiles/wind-projectileAnim.png", Textures::ID::WindWispProjectile);
	//Boss textures
	fTextures.load("Textures/Boss/Minotaur.png", Textures::ID::Minotaur);
	//NPC textures
	fTextures.load("Textures/NPCs/NPC1_Idle.png", Textures::ID::NPC1);
	fTextures.load("Textures/NPCs/NPC2_Idle.png", Textures::ID::NPC2);
	//Items texture
	fTextures.load("Textures/Items/ItemsSpriteSheet.png", Textures::ID::Items);
	fTextures.load("Textures/Items/MagicBookItem.png", Textures::ID::MagicBookItem);
	//Block texture
	fTextures.load("Textures/Blocks/BlockTextures.png", Textures::ID::Block);
	fTextures.load("Textures/Blocks/RepeatedBlock_Long.png", Textures::ID::RepeatedBlock_Long);
	fTextures.load("Textures/Blocks/RepeatedBlock_Small.png", Textures::ID::RepeatedBlock_Small);
	//Breakable box texture
	fTextures.load("Textures/BreakableBoxes/crateSheet.png", Textures::ID::Crate);
	//Ladder textures
	fTextures.load("Textures/Ladders/Ladder_1.png", Textures::ID::Ladder_Normal);
	fTextures.load("Textures/Ladders/Ladder_2.png", Textures::ID::Ladder_Mossy);
	//Cabury texture
	fTextures.load("Textures/Cabury/Animations.png", Textures::ID::Cabury);
	fTextures.load("Textures/Cabury/cage.png", Textures::ID::Cage);
	fTextures.load("Textures/Cabury/MagicBook.png", Textures::ID::MagicBook);

}

void CaburyApplication::run()
{
	sf::Clock clock;
	sf::Time deltaTime = sf::Time::Zero;
	sf::Time Accumulator = sf::Time::Zero;
	while (fWindow.isOpen())
	{
		deltaTime = clock.restart();
		Accumulator += deltaTime;
		while (Accumulator >= FrameRate)
		{
			Accumulator -= FrameRate;
			ProcessInput(FrameRate);
			if (Focus) {
				fStack.update(FrameRate);
			}
		}

		updateStats(deltaTime);
		render();
	}
}

void CaburyApplication::registerStates()
{
	fStack.registerState<MenuState>(States::ID::Title);
	fStack.registerState<GameState>(States::ID::Game);
	fStack.registerState<PauseState>(States::ID::Pause);
	fStack.registerState<DeathState>(States::ID::Death);
	fStack.registerState<SettingsState>(States::ID::Settings);
}

void CaburyApplication::updateStats(sf::Time dt)
{
	fElapsedStatsTime += dt;
	NumFrames++;

	if (fElapsedStatsTime >= sf::seconds(1.0f))
	{
		StatsText.setString("Frames / Second = " + std::to_string(NumFrames) +
			"\nTime / Update = " + std::to_string(fElapsedStatsTime.asMicroseconds() / NumFrames) + "us");

		fElapsedStatsTime -= sf::seconds(1.0f);
		NumFrames = 0;
	}
}

void CaburyApplication::ProcessInput(sf::Time dt)
{
	sf::Event e;
	while (fWindow.pollEvent(e))
	{
		if (e.type == sf::Event::Closed)
		{
			fWindow.close();
		}
		else if (e.type == sf::Event::LostFocus)
		{
			Focus = false;
		}
		else if (e.type == sf::Event::GainedFocus)
		{
			Focus = true;
		}
		fStack.handleEvent(e);
	}
}

void CaburyApplication::render()
{
	fWindow.clear();
	fStack.draw();
	fWindow.setView(fWindow.getDefaultView());
	fWindow.draw(StatsText);
	fWindow.display();
}
