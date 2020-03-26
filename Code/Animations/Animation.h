#pragma once
#include "ResourceIdentifiers.h"

class Animation
{
	sf::Vector2i StartPoint;
	const sf::Texture* fTexture;

	int fNumFrames;
	sf::Vector2i fFrameSize;
	size_t fCurrentFrame;
	//how much time does the full animation take
	sf::Time fDuration;

	sf::Time fElapsedTime;
	bool fRepeat;
	int fSpriteReadingDir;

	bool fStopped = false;
	
public:
	Animation();
	explicit Animation(const sf::Texture& texture);
	explicit Animation(const sf::Texture& texture, sf::Vector2i StartPoint, int SpriteReadingDir);

	void setStartPoint(sf::Vector2i newStartPoint);
	sf::Vector2i getStartPoint() const;

	const sf::Texture& getTexture() const;
	void setTexture(const sf::Texture& tex);

	int getReadingDirection() const;
	void setReadingDirection(int dir);

	void stop();
	void start();

	void setFrameSize(sf::Vector2i framesize);
	sf::Vector2i getFrameSize() const;

	void Repeat(bool flag);
	bool isRepeating() const;

	void setDuration(sf::Time newTime);
	sf::Time getDuration() const;

	size_t getNumFrames() const;
	void setNumFrames(size_t Num);
	size_t getCurrentFrame() const;

	bool isFinished() const;
	void Restart();
	void update(sf::Time dt, sf::Sprite& sprite);
	
};

