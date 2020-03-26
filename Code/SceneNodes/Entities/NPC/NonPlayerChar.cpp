#include "NonPlayerChar.h"
#include "UtilityFunc.h"
#include "Answer.h"
#include "State.h"

NonPlayerChar::NonPlayerChar(int hp, Type type, State::Context context)
	:Entity(hp), fHelpingText("Entrer pour intéragir"), fText(fHelpingText, context.fonts->get(Fonts::ID::Sansation)), fAnimData(), fFonts(*context.fonts), fContext(context)
{
	handleType(type);

	fSprite.setTexture(*fAnimData.fTexture);
	fSprite.setTextureRect(fAnimData.fTextureRects[NPCAnim::Idle]);
	Utility::CenterOrigin(fSprite);


	fText.setCharacterSize(20);
	Utility::CenterOrigin(fText);
	fText.setPosition(0.f, - 1.5f * getBoundingRect().height / 2.f);

	setupAnims();
	setCurrentAnimation(AnimType::Idle);
}

unsigned int NonPlayerChar::getCategory() const
{
	return Category::NPC;
}

void NonPlayerChar::updateNode(sf::Time dt, CommandQueue & commands)
{

	Entity::updateNode(dt, commands);

	if (!fBeginInteraction && !fStopInteract) {
		reset();
	}

	if (fStopInteract)
	{
		if (!fStopShowing) {
			fStopShowingAccu += dt;
			if (fStopShowingAccu >= fStopShowingDelay)
			{
				fStopShowingAccu = sf::Time::Zero;
				fStopShowing = true;
				fBeginInteraction = false;
			}
			else
			{
				fBeginInteraction = true;
			}
		}
	}
}

bool NonPlayerChar::isRemovable() const
{
	return false;
}

bool NonPlayerChar::isDestroyed() const
{
	return false;
}

bool NonPlayerChar::applyDamage(int amount)
{
	return false;
}

void NonPlayerChar::showHelpText()
{	
	if (!fStopInteract) {
		if (!fBeginInteraction)
		{
			fText.setFont(fFonts.get(Fonts::ID::Sansation));
			fText.setCharacterSize(20);
		}
		fBeginInteraction = true;
	}
}


void NonPlayerChar::stopInteraction(sf::Time delay)
{
	if (!fStopInteract) {
		fStopInteract = true;
		fStopShowingDelay = delay;
		fStopShowingAccu = sf::Time::Zero;
		fBeginInteraction = true;
		fText.setFont(fFonts.get(Fonts::ID::Sansation));
	}
}

void NonPlayerChar::playInteraction()
{
	if (fStopInteract)
	{
		fStopInteract = false;
		fStopShowing = false;
	}
}

bool NonPlayerChar::isInteractionPaused() const
{
	return fStopInteract;
}

void NonPlayerChar::setBeginInteraction(bool flag)
{
	fBeginInteraction = flag;
}

bool NonPlayerChar::getBeginInteraction() const
{
	return fBeginInteraction;
}

void NonPlayerChar::handleEvent(const sf::Event& e, CommandQueue& commands)
{
}

void NonPlayerChar::createMenu(CommandQueue& commands)
{
}

void NonPlayerChar::reset()
{
	fText.setString(fHelpingText);
	Utility::CenterOrigin(fText);
}

void NonPlayerChar::drawCurrent(sf::RenderTarget & target, sf::RenderStates states) const
{
	
	target.draw(fSprite, states);
	drawSthBetween(target, states);
	if (fBeginInteraction && !fStopShowing) {
		target.draw(fText, states);
	}
}

void NonPlayerChar::handleType(Type type)
{
	switch (type)
	{
	case NonPlayerChar::Goblin:
	{
		sf::Texture& tex = fContext.textures->get(Textures::ID::NPC1);

		sf::IntRect texRects[NPCAnim::AnimCount]{ sf::IntRect(0, 0, 120, 86), sf::IntRect(), sf::IntRect() };
		size_t numFrames[NPCAnim::AnimCount]{ 7, 0, 0 };
		sf::Time durations[NPCAnim::AnimCount]{ sf::seconds(0.7f), sf::seconds(0.f), sf::seconds(0.f) };

		NPCAnimData data{ durations, texRects, numFrames, &tex };
		data.fReadDir[ReadDir::Right] = data.fReadDir[ReadDir::Left] = 1;

		fAnimData = data;
	}
		break;
	case NonPlayerChar::Mage:
	{
		sf::Texture& tex = fContext.textures->get(Textures::ID::NPC2);

		sf::IntRect texRects[NPCAnim::AnimCount]{ sf::IntRect(0, 0, 160, 140), sf::IntRect(2 * 160, 0, 160, 140), sf::IntRect(10 * 160, 0, 160, 140) };
		size_t numFrames[NPCAnim::AnimCount]{ 2, 8, 2 };
		sf::Time durations[NPCAnim::AnimCount]{ sf::seconds(0.5f), sf::seconds(1.f) ,sf::seconds(0.5f) };

		NPCAnimData data{ durations, texRects, numFrames, &tex };
		data.fReadDir[ReadDir::Right] = data.fReadDir[ReadDir::Left] = 1;

		fAnimData = data;
	}
		break;
	default:
		throw std::logic_error("the NPC type specified doesn't exist");
		break;
	}
}

void NonPlayerChar::drawSthBetween(sf::RenderTarget& target, sf::RenderStates states) const
{
}

void NonPlayerChar::setupAnims()
{
	//Idle animation
	Animation* Idle = new Animation(*fAnimData.fTexture, sf::Vector2i(fAnimData.fTextureRects[NPCAnim::Idle].left, fAnimData.fTextureRects[NPCAnim::Idle].top), fAnimData.fReadDir[ReadDir::Right]);
	Idle->setDuration(fAnimData.fDurations[NPCAnim::Idle]);
	Idle->Repeat(true);
	Idle->setFrameSize(sf::Vector2i(fAnimData.fTextureRects[NPCAnim::Idle].width, fAnimData.fTextureRects[NPCAnim::Idle].height));
	Idle->setNumFrames(fAnimData.fNumFrames[NPCAnim::Idle]);
	Idle->Restart();

	fAnimations[AnimType::Idle] = Idle;
}
