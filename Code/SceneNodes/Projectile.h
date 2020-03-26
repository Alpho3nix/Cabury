#pragma once
#include "SceneNode.h"
#include "Animation.h"
//projectiles for wisps 32 x 16
class Projectile : public SceneNode
{
public:
	Projectile(sf::Texture& tex, sf::IntRect texRect, int dir, float velocity = 200.f, int damage = 10);

	unsigned int getCategory() const override;
	sf::FloatRect getBoundingRect() const override;
	bool isRemovable() const override;
	bool isDestroyed() const override;
	void Destroy();
	int getDamage() const;
	int getDirection() const;
	void setDirection(int dir);

private:
	sf::Sprite fSprite;
	Animation fSpriteAnim;
	int fDamage = 10;
	float fVelocity = 200.f;
	int fDirection = 1;

	bool fIsDestroyed = false;

	void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;
	void updateNode(sf::Time dt, CommandQueue& commands) override;

};