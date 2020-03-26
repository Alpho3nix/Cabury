#pragma once
#include "Damageable.h"
#include "Animation.h"
#include "ResourceIdentifiers.h"

class BreakableBox : public Damageable
{
public:

	enum Type
	{
		Crate
	};

	BreakableBox(Type type, const TexturesHolder& textures, std::function<void()> breakAction = []() {});

	bool isDestroyed() const override;
	bool isRemovable() const override;

	unsigned int getCategory() const override;
	sf::FloatRect getBoundingRect() const override;
	sf::FloatRect getSpriteBoundingRect() const;

	sf::Vector2f getWorldPositionCentered() const;
	
	void setOnDestroyedAction(std::function<void()> newAction);

	void updateNode(sf::Time dt, CommandQueue& commands) override;

	bool applyDamage(int amount) override;
private:

	sf::Sprite fSprite;
	Animation fDeathAnim;
	bool fIsDead = false;
	bool fCanRemove = false;

	std::function<void()> fDestroyedAction;
	bool fHasCalledAction = false;

	sf::FloatRect fBoundingRect;

	void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;
	void handleType(Type type, const TexturesHolder& textures);

};