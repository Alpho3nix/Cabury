#pragma once
#include "Entity.h"
#include "DataTables.h"
#include "State.h"

class NonPlayerChar : public Entity
{
public:

	enum Type
	{
		Goblin, Mage
	};

	NonPlayerChar(int hp, Type type, State::Context context);

	unsigned int getCategory() const override;
	virtual void updateNode(sf::Time dt, CommandQueue& commands) override;
	
	bool isRemovable() const override;
	bool isDestroyed() const override;
	bool applyDamage(int amount) override;
	void showHelpText();
	void stopInteraction(sf::Time delay);
	void playInteraction(); 
	bool isInteractionPaused() const;

	void setBeginInteraction(bool flag);
	bool getBeginInteraction() const;

	virtual void handleEvent(const sf::Event& e, CommandQueue& commands);
	virtual void createMenu(CommandQueue& commands);
	virtual void reset();

private:
	void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;

	void handleType(Type type);
protected:

	virtual void drawSthBetween(sf::RenderTarget& target, sf::RenderStates states) const;

	sf::Text fText;
	bool fBeginInteraction = false;
	bool fStopInteract = false;
	bool fStopShowing = false;
	
	State::Context fContext;

	const FontsHolder& fFonts;
	const std::string fHelpingText;
private:
	sf::Time fStopShowingAccu = sf::Time::Zero;
	sf::Time fStopShowingDelay = sf::Time::Zero;

protected:

	void setupAnims();
	NPCAnimData fAnimData;
};