#include "Animation.h"
#include <iostream>

Animation::Animation()
	:fNumFrames(0)
	, fFrameSize()
	, fCurrentFrame(0)
	, fDuration(sf::Time::Zero)
	, fElapsedTime(sf::Time::Zero)
	, fRepeat(false)
	, StartPoint()
	, fTexture(nullptr)
{
}

Animation::Animation(const sf::Texture & texture)
	:fNumFrames(0)
	, fFrameSize()
	, fCurrentFrame(0)
	, fDuration(sf::Time::Zero)
	, fElapsedTime(sf::Time::Zero)
	, fRepeat(false)
	, StartPoint(0, 0)
	, fTexture(&texture)
{
}

Animation::Animation(const sf::Texture & texture, sf::Vector2i StartPoint, int SpriteReadingDir)
	:fNumFrames(0)
	, fFrameSize()
	, fCurrentFrame(0)
	, fDuration(sf::Time::Zero)
	, fElapsedTime(sf::Time::Zero)
	, fRepeat(false)
	, StartPoint(StartPoint)
	, fSpriteReadingDir(SpriteReadingDir)
	, fTexture(&texture)
{
	assert(fSpriteReadingDir == 1 || fSpriteReadingDir == -1);
}

void Animation::setStartPoint(sf::Vector2i newStartPoint)
{
	StartPoint = newStartPoint;
}

int Animation::getReadingDirection() const
{
	return fSpriteReadingDir;
}

void Animation::setReadingDirection(int dir)
{
	fSpriteReadingDir = dir;
}

void Animation::stop()
{
	fStopped = true;
}

void Animation::start()
{
	fStopped = false;
}

sf::Vector2i Animation::getStartPoint() const
{
	return StartPoint;
}

const sf::Texture& Animation::getTexture() const
{
	return *fTexture;
}

void Animation::setTexture(const sf::Texture& tex)
{
	fTexture = &tex;
}

void Animation::setFrameSize(sf::Vector2i framesize)
{
	fFrameSize = framesize;
}

sf::Vector2i Animation::getFrameSize() const
{
	return fFrameSize;
}

void Animation::Repeat(bool flag)
{
	fRepeat = flag;
}

bool Animation::isRepeating() const
{
	return fRepeat;
}

void Animation::setDuration(sf::Time newTime)
{
	fDuration = newTime;
}

sf::Time Animation::getDuration() const
{
	return fDuration;
}

size_t Animation::getNumFrames() const
{
	return fNumFrames;
}

void Animation::setNumFrames(size_t Num)
{
	fNumFrames = Num;
}

size_t Animation::getCurrentFrame() const
{
	return fCurrentFrame;
}

bool Animation::isFinished() const
{
	return (fCurrentFrame >= fNumFrames) && (!fRepeat);
}

void Animation::Restart()
{
	fCurrentFrame = 0;
	fElapsedTime = fDuration;
	fStopped = false;
}

void Animation::update(sf::Time dt, sf::Sprite& sprite)
{
	if (!fStopped) {
		sf::Time fTimePerFrame = fDuration / static_cast<float>(fNumFrames);
		fElapsedTime += dt;

		sf::IntRect textureRect = sprite.getTextureRect();

		if (fCurrentFrame == 0)
		{
			textureRect = sf::IntRect(StartPoint.x, StartPoint.y, fFrameSize.x, fFrameSize.y);
		}

		while (fElapsedTime >= fDuration && (fCurrentFrame <= fNumFrames || fRepeat))
		{
			textureRect.left += fSpriteReadingDir * textureRect.width;	//going to the left or right of the texture
			fElapsedTime -= fTimePerFrame;

			if (fSpriteReadingDir == 1) {
				if (textureRect.left + textureRect.width > fTexture->getSize().x + 3)	//it has touched the right side of the whole texture
				{
					textureRect.left = 0;
					textureRect.top += textureRect.height;
				}
			}
			else
			{
				if (textureRect.left < -3)	//it has touched the left side of the whole texture
				{
					textureRect.left = fTexture->getSize().x - fFrameSize.x;
					textureRect.top += textureRect.height;
				}
			}

			if (fRepeat) {
				fCurrentFrame = (fCurrentFrame + 1) % fNumFrames;
				if (fCurrentFrame == 0)	//it is the last frame
				{
					textureRect = sf::IntRect(StartPoint.x, StartPoint.y, fFrameSize.x, fFrameSize.y);
				}
			}
			else
			{
				fCurrentFrame++;
			}


		}

		if (textureRect.top < fTexture->getSize().y)
		{
			sprite.setTextureRect(textureRect);
		}
	}
}