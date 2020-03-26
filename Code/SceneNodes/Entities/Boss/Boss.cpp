#include "Boss.h"
#include "Player.h"
#include "Item.h"
#include "UtilityFunc.h"

sf::Color fHurtColor(255, 0, 0, 120);

bool Boss::isTakingDamage() const
{
	return fSprite.getColor() == fHurtColor;
	
}

void Boss::handleType(Type type, TexturesHolder& textures)
{
	switch (type)
	{
	case Boss::Minotaur:
	{
		//Idle, Walk, Hurt, Attack_1, Attack_2, Attack_3, Death, JumpReady, JumpLoop, Fall
		sf::Time MinotaurDurations[BossAnim::AnimCount] = { sf::seconds(0.8f), sf::seconds(0.8f), sf::seconds(0.5f), sf::seconds(1.5f), sf::seconds(1.5f), sf::seconds(1.5f), sf::seconds(1.2f)
			, sf::seconds(0.5f), sf::seconds(0.3f), sf::seconds(0.3f) };
		sf::Texture* MinotaurTexture = &textures.get(Textures::ID::Minotaur);
		size_t MinotaurNumFrames[BossAnim::AnimCount] = { 5, 6, 4, 10, 9, 4, 7, 2, 2, 2 };

		//frame dimensions: 192 x 154, texture dimensions: 1152 x 2464
		sf::Vector2i MinotaurFrameDim(192, 154);
		sf::Vector2u MinotaurTexDim = MinotaurTexture->getSize();
		sf::IntRect MinotaurTextureRects[BossAnim::AnimCount2] =
		{
			sf::IntRect(sf::Vector2i(MinotaurTexDim.x - MinotaurFrameDim.x, MinotaurTexDim.y / 2), MinotaurFrameDim),	//IdleRight
			sf::IntRect(sf::Vector2i(0, 0), MinotaurFrameDim),	//IdleLeft
			sf::IntRect(sf::Vector2i(0, MinotaurTexDim.y / 2), MinotaurFrameDim),	//WalkRight
			sf::IntRect(sf::Vector2i(MinotaurTexDim.x - MinotaurFrameDim.x, 0), MinotaurFrameDim),	//WalkLeft
			sf::IntRect(sf::Vector2i(MinotaurTexDim.x - MinotaurFrameDim.x, MinotaurTexDim.y / 2 + 5 * MinotaurFrameDim.y), MinotaurFrameDim),	//HurtRight
			sf::IntRect(sf::Vector2i(0, 5 * MinotaurFrameDim.y), MinotaurFrameDim),	//HurtLeft
			sf::IntRect(sf::Vector2i(0, MinotaurTexDim.y / 2 + MinotaurFrameDim.y), MinotaurFrameDim),	//Attack_1Right
			sf::IntRect(sf::Vector2i(MinotaurTexDim.x - MinotaurFrameDim.x, MinotaurFrameDim.y), MinotaurFrameDim),	//Attack_1Left
			sf::IntRect(sf::Vector2i(2 * MinotaurFrameDim.x, MinotaurTexDim.y / 2 + 3 * MinotaurFrameDim.y), MinotaurFrameDim),	//Attack_2Right
			sf::IntRect(sf::Vector2i(3 * MinotaurFrameDim.x, 3 * MinotaurFrameDim.y), MinotaurFrameDim),	//Attack_2Left
			sf::IntRect(sf::Vector2i(MinotaurTexDim.x - MinotaurFrameDim.x, MinotaurTexDim.y - MinotaurFrameDim.y), MinotaurFrameDim),	//Attack_3Right
			sf::IntRect(sf::Vector2i(0, MinotaurTexDim.y / 2 - MinotaurFrameDim.y), MinotaurFrameDim),	//Attack_3Left
			sf::IntRect(sf::Vector2i(MinotaurFrameDim.x, MinotaurTexDim.y - 3 * MinotaurFrameDim.y), MinotaurFrameDim),	//DeathRight
			sf::IntRect(sf::Vector2i(MinotaurTexDim.x - 2 * MinotaurFrameDim.x, MinotaurTexDim.y / 2 - 3 * MinotaurFrameDim.y), MinotaurFrameDim),	//DeathLeft
			sf::IntRect(sf::Vector2i(MinotaurTexDim.x - MinotaurFrameDim.x, MinotaurTexDim.y - MinotaurFrameDim.y), MinotaurFrameDim),	//JumpReady_right
			sf::IntRect(sf::Vector2i(0, MinotaurTexDim.y / 2 - MinotaurFrameDim.y), MinotaurFrameDim),	//JumpReady_left
			sf::IntRect(sf::Vector2i(MinotaurTexDim.x - 3 * MinotaurFrameDim.x, MinotaurTexDim.y - MinotaurFrameDim.y), MinotaurFrameDim),	//JumpLoop_right
			sf::IntRect(sf::Vector2i(2 * MinotaurFrameDim.x, MinotaurTexDim.y / 2 - MinotaurFrameDim.y), MinotaurFrameDim),	//JumpLoop_left
			sf::IntRect(sf::Vector2i(MinotaurFrameDim.x, MinotaurTexDim.y - MinotaurFrameDim.y), MinotaurFrameDim),	//FallRight
			sf::IntRect(sf::Vector2i(MinotaurTexDim.x - 2 * MinotaurFrameDim.x, MinotaurTexDim.y / 2 - MinotaurFrameDim.y), MinotaurFrameDim),	//FallLeft
		};

		BossAnimationData MinotaurData{ MinotaurDurations, MinotaurTextureRects, MinotaurNumFrames, MinotaurTexture };
		MinotaurData.fReadDir[ReadDir::Left] = 1;
		MinotaurData.fReadDir[ReadDir::Right] = -1;

		fAnimData = MinotaurData;
	}
		break;
	default:
		throw std::logic_error("the specified boss type doesn't exist");
		break;
	}
}

Boss::Boss(int hp, Type type, TexturesHolder& textures, const FontsHolder& fonts, Player& player, std::vector<std::vector<ItemType>> itemTypes)
	:Entity(hp), fAnimData(), fPlayer(player), fItemTex(textures.get(Textures::ID::Items)), fItemTypes(itemTypes)
{
	handleType(type, textures);

	fSprite.setTexture(*fAnimData.fTexture);
	setAnimations();
	setCurrentAnimation(AnimType::Idle);
	fSprite.setTextureRect(fAnimData.fTextureRects[BossAnim::Idle]);
	Utility::CenterOrigin(fSprite);

	fDropItemCommand.category = Category::Scene;
	fDropItemCommand.action = [this](SceneNode& node, sf::Time)	//TODO: drop more than one item of each type
	{
		for (int i = 0; i < fItemTypes.size(); ++i)
		{

			unsigned int index = Utility::randomInt(fItemTypes[i].size());
			createItem(node, fItemTex, fItemTypes[i][index]);
		}
	};

}

void Boss::createItem(SceneNode & node, const sf::Texture & tex, ItemType type)
{
	Item* newItem = new Item(1, fItemTex, type);
	newItem->setPosition(getWorldPosition());
	assert(&node);
	node.attachChild(newItem);
}

bool Boss::applyDamage(int amount)
{
	HealthPoints -= amount;
	if (HealthPoints <= 0)
	{
		HealthPoints = 0;
		fIsDead = true;
	}
	else
	{
		if(!fIsBeingHurt)
			fIsBeingHurt = true;
	}
	return true;
}


unsigned int Boss::getCategory() const
{
	return Category::Bosses;
}

bool Boss::isRemovable() const
{
	return fRemovable;
}

Boss::~Boss()
{
}

void Boss::updateNode(sf::Time dt, CommandQueue & commands)
{

	Entity::updateNode(dt, commands);
	if(fIsDead)
	{

		setCurrentAnimation(Death);
		if (!fHasDroppedItems)
		{
			commands.push(fDropItemCommand);
			fHasDroppedItems = true;
		}
		if (fAnimationFinished)
		{
			fRemovable = true;	
		}
	}
	else if (isSteppingOnSth()) {

		
		setCurrentAnimation(Idle);
		

		if (fIsBeingHurt)
		{
			fHurtAccu += dt;
			if (fHurtAccu <= fHurtTimer)
			{
				fSprite.setColor(fHurtColor);
			}
			else
			{
				fHurtAccu = sf::Time::Zero;
				fIsBeingHurt = false;
				fSprite.setColor(sf::Color::White);
			}
		}
		else
		{
			fHurtAccu = sf::Time::Zero;
		}
	}


}


void Boss::setAnimations()
{
	//Idle when looking right
	Animation* IdleRight = new Animation(*fAnimData.fTexture, sf::Vector2i(fAnimData.fTextureRects[BossAnim::IdleRight].left, fAnimData.fTextureRects[BossAnim::IdleRight].top),
		fAnimData.fReadDir[ReadDir::Right]);
	IdleRight->Repeat(true);
	IdleRight->setDuration(fAnimData.fDurations[BossAnim::Idle]);
	IdleRight->setFrameSize(sf::Vector2i(fAnimData.fTextureRects[BossAnim::Idle].width, fAnimData.fTextureRects[BossAnim::Idle].height));
	IdleRight->setNumFrames(fAnimData.fNumFrames[BossAnim::Idle]);
	IdleRight->Restart();

	fAnimations[AnimType::Idle] = IdleRight;

	//Idle when looking left
	Animation* IdleLeft = new Animation(*fAnimData.fTexture, sf::Vector2i(fAnimData.fTextureRects[BossAnim::IdleLeft].left, fAnimData.fTextureRects[BossAnim::IdleLeft].top),
		fAnimData.fReadDir[ReadDir::Left]);
	IdleLeft->Repeat(true);
	IdleLeft->setDuration(fAnimData.fDurations[BossAnim::Idle]);
	IdleLeft->setFrameSize(sf::Vector2i(fAnimData.fTextureRects[BossAnim::Idle].width, fAnimData.fTextureRects[BossAnim::Idle].height));
	IdleLeft->setNumFrames(fAnimData.fNumFrames[BossAnim::Idle]);
	IdleLeft->Restart();

	fAnimations[AnimType::IdleLeft] = IdleLeft;

	//Walk when looking right
	Animation* WalkRight = new Animation(*fAnimData.fTexture, sf::Vector2i(fAnimData.fTextureRects[BossAnim::WalkRight].left, fAnimData.fTextureRects[BossAnim::WalkRight].top),
		fAnimData.fReadDir[ReadDir::Right]);
	WalkRight->Repeat(true);
	WalkRight->setDuration(fAnimData.fDurations[BossAnim::Walk]);
	WalkRight->setFrameSize(sf::Vector2i(fAnimData.fTextureRects[BossAnim::Walk].width, fAnimData.fTextureRects[BossAnim::Walk].height));
	WalkRight->setNumFrames(fAnimData.fNumFrames[BossAnim::Walk]);
	WalkRight->Restart();

	fAnimations[AnimType::Run] = WalkRight;

	//Walk when looking left
	Animation* WalkLeft = new Animation(*fAnimData.fTexture, sf::Vector2i(fAnimData.fTextureRects[BossAnim::WalkLeft].left, fAnimData.fTextureRects[BossAnim::WalkLeft].top),
		fAnimData.fReadDir[ReadDir::Left]);
	WalkLeft->Repeat(true);
	WalkLeft->setDuration(fAnimData.fDurations[BossAnim::Walk]);
	WalkLeft->setFrameSize(sf::Vector2i(fAnimData.fTextureRects[BossAnim::Walk].width, fAnimData.fTextureRects[BossAnim::Walk].height));
	WalkLeft->setNumFrames(fAnimData.fNumFrames[BossAnim::Walk]);
	WalkLeft->Restart();

	fAnimations[RunLeft] = WalkLeft;

	//Hit when looking right
	Animation* HitRight = new Animation(*fAnimData.fTexture, sf::Vector2i(fAnimData.fTextureRects[BossAnim::beingHitRight].left, fAnimData.fTextureRects[BossAnim::beingHitRight].top),
		fAnimData.fReadDir[ReadDir::Right]);
	HitRight->Repeat(false);
	HitRight->setDuration(fAnimData.fDurations[BossAnim::beingHit]);
	HitRight->setFrameSize(sf::Vector2i(fAnimData.fTextureRects[BossAnim::beingHit].width, fAnimData.fTextureRects[BossAnim::beingHit].height));
	HitRight->setNumFrames(fAnimData.fNumFrames[BossAnim::beingHit]);
	HitRight->Restart();

	fAnimations[AnimType::beingHit] = HitRight;

	//Hit when looking left
	Animation* HitLeft = new Animation(*fAnimData.fTexture, sf::Vector2i(fAnimData.fTextureRects[BossAnim::beingHitLeft].left, fAnimData.fTextureRects[BossAnim::beingHitLeft].top),
		fAnimData.fReadDir[ReadDir::Left]);
	HitLeft->Repeat(false);
	HitLeft->setDuration(fAnimData.fDurations[BossAnim::beingHit]);
	HitLeft->setFrameSize(sf::Vector2i(fAnimData.fTextureRects[BossAnim::beingHit].width, fAnimData.fTextureRects[BossAnim::beingHit].height));
	HitLeft->setNumFrames(fAnimData.fNumFrames[BossAnim::beingHit]);
	HitLeft->Restart();

	fAnimations[AnimType::beingHitLeft] = HitLeft;

	//Attack_1 when looking right (NOTE: not sure if it should be on repeat mode)
	Animation* Attack_1Right = new Animation(*fAnimData.fTexture, sf::Vector2i(fAnimData.fTextureRects[BossAnim::Attack_1Right].left, fAnimData.fTextureRects[BossAnim::Attack_1Right].top),
		fAnimData.fReadDir[ReadDir::Right]);
	Attack_1Right->Repeat(true);
	Attack_1Right->setDuration(fAnimData.fDurations[BossAnim::Attack_1]);
	Attack_1Right->setFrameSize(sf::Vector2i(fAnimData.fTextureRects[BossAnim::Attack_1].width, fAnimData.fTextureRects[BossAnim::Attack_1].height));
	Attack_1Right->setNumFrames(fAnimData.fNumFrames[BossAnim::Attack_1]);
	Attack_1Right->Restart();

	fAnimations[AnimType::Attack1] = Attack_1Right;

	//Attack_1 when looking left (NOTE: not sure if it should be on repeat mode)
	Animation* Attack_1Left = new Animation(*fAnimData.fTexture, sf::Vector2i(fAnimData.fTextureRects[BossAnim::Attack_1Left].left, fAnimData.fTextureRects[BossAnim::Attack_1Left].top),
		fAnimData.fReadDir[ReadDir::Left]);
	Attack_1Left->Repeat(true);
	Attack_1Left->setDuration(fAnimData.fDurations[BossAnim::Attack_1]);
	Attack_1Left->setFrameSize(sf::Vector2i(fAnimData.fTextureRects[BossAnim::Attack_1].width, fAnimData.fTextureRects[BossAnim::Attack_1].height));
	Attack_1Left->setNumFrames(fAnimData.fNumFrames[BossAnim::Attack_1]);
	Attack_1Left->Restart();

	fAnimations[AnimType::Attack1_left] = Attack_1Left;

	//Attack_2 when looking right (NOTE: not sure if it should be on repeat mode)
	Animation* Attack_2Right = new Animation(*fAnimData.fTexture, sf::Vector2i(fAnimData.fTextureRects[BossAnim::Attack_2Right].left, fAnimData.fTextureRects[BossAnim::Attack_2Right].top),
		fAnimData.fReadDir[ReadDir::Right]);
	Attack_2Right->Repeat(true);
	Attack_2Right->setDuration(fAnimData.fDurations[BossAnim::Attack_2]);
	Attack_2Right->setFrameSize(sf::Vector2i(fAnimData.fTextureRects[BossAnim::Attack_2].width, fAnimData.fTextureRects[BossAnim::Attack_2].height));
	Attack_2Right->setNumFrames(fAnimData.fNumFrames[BossAnim::Attack_2]);
	Attack_2Right->Restart();

	fAnimations[AnimType::Attack2] = Attack_2Right;

	//Attack_2 when looking left (NOTE: not sure if it should be on repeat mode)
	Animation* Attack_2Left = new Animation(*fAnimData.fTexture, sf::Vector2i(fAnimData.fTextureRects[BossAnim::Attack_2Left].left, fAnimData.fTextureRects[BossAnim::Attack_2Left].top),
		fAnimData.fReadDir[ReadDir::Left]);
	Attack_2Left->Repeat(true);
	Attack_2Left->setDuration(fAnimData.fDurations[BossAnim::Attack_2]);
	Attack_2Left->setFrameSize(sf::Vector2i(fAnimData.fTextureRects[BossAnim::Attack_2].width, fAnimData.fTextureRects[BossAnim::Attack_2].height));
	Attack_2Left->setNumFrames(fAnimData.fNumFrames[BossAnim::Attack_2]);
	Attack_2Left->Restart();

	fAnimations[AnimType::Attack2_Left] = Attack_2Left;

	//Attack_3 when looking right (NOTE: not sure if it should be on repeat mode)
	Animation* Attack_3Right = new Animation(*fAnimData.fTexture, sf::Vector2i(fAnimData.fTextureRects[BossAnim::Attack_3Right].left, fAnimData.fTextureRects[BossAnim::Attack_3Right].top),
		fAnimData.fReadDir[ReadDir::Right]);
	Attack_3Right->Repeat(true);
	Attack_3Right->setDuration(fAnimData.fDurations[BossAnim::Attack_3]);
	Attack_3Right->setFrameSize(sf::Vector2i(fAnimData.fTextureRects[BossAnim::Attack_3].width, fAnimData.fTextureRects[BossAnim::Attack_3].height));
	Attack_3Right->setNumFrames(fAnimData.fNumFrames[BossAnim::Attack_3]);
	Attack_3Right->Restart();

	fAnimations[AnimType::Attack3] = Attack_3Right;

	//Attack_3 when looking left (NOTE: not sure if it should be on repeat mode)
	Animation* Attack_3Left = new Animation(*fAnimData.fTexture, sf::Vector2i(fAnimData.fTextureRects[BossAnim::Attack_3Left].left, fAnimData.fTextureRects[BossAnim::Attack_3Left].top),
		fAnimData.fReadDir[ReadDir::Left]);
	Attack_3Left->Repeat(true);
	Attack_3Left->setDuration(fAnimData.fDurations[BossAnim::Attack_3]);
	Attack_3Left->setFrameSize(sf::Vector2i(fAnimData.fTextureRects[BossAnim::Attack_3].width, fAnimData.fTextureRects[BossAnim::Attack_3].height));
	Attack_3Left->setNumFrames(fAnimData.fNumFrames[BossAnim::Attack_3]);
	Attack_3Left->Restart();

	fAnimations[AnimType::Attack3_Left] = Attack_3Left;

	//JumpReady when looking Right
	Animation* Jump_readyRight = new Animation(*fAnimData.fTexture, sf::Vector2i(fAnimData.fTextureRects[BossAnim::Jump_readyRight].left, fAnimData.fTextureRects[BossAnim::Jump_readyRight].top),
		fAnimData.fReadDir[ReadDir::Right]);
	Jump_readyRight->Repeat(true);
	Jump_readyRight->setDuration(fAnimData.fDurations[BossAnim::JumpReady]);
	Jump_readyRight->setFrameSize(sf::Vector2i(fAnimData.fTextureRects[BossAnim::JumpReady].width, fAnimData.fTextureRects[BossAnim::JumpReady].height));
	Jump_readyRight->setNumFrames(fAnimData.fNumFrames[BossAnim::JumpReady]);
	Jump_readyRight->Restart();

	fAnimations[AnimType::JumpReady] = Jump_readyRight;

	//JumpReady when looking Left
	Animation* Jump_readyLeft = new Animation(*fAnimData.fTexture, sf::Vector2i(fAnimData.fTextureRects[BossAnim::Jump_readyLeft].left, fAnimData.fTextureRects[BossAnim::Jump_readyLeft].top),
		fAnimData.fReadDir[ReadDir::Left]);
	Jump_readyLeft->Repeat(true);
	Jump_readyLeft->setDuration(fAnimData.fDurations[BossAnim::JumpReady]);
	Jump_readyLeft->setFrameSize(sf::Vector2i(fAnimData.fTextureRects[BossAnim::JumpReady].width, fAnimData.fTextureRects[BossAnim::JumpReady].height));
	Jump_readyLeft->setNumFrames(fAnimData.fNumFrames[BossAnim::JumpReady]);
	Jump_readyLeft->Restart();

	fAnimations[AnimType::JumpReadyLeft] = Jump_readyLeft;

	//JumpLoop when looking Right
	Animation* Jump_loopRight = new Animation(*fAnimData.fTexture, sf::Vector2i(fAnimData.fTextureRects[BossAnim::Jump_loopRight].left, fAnimData.fTextureRects[BossAnim::Jump_loopRight].top),
		fAnimData.fReadDir[ReadDir::Right]);
	Jump_loopRight->Repeat(true);
	Jump_loopRight->setDuration(fAnimData.fDurations[BossAnim::JumpLoop]);
	Jump_loopRight->setFrameSize(sf::Vector2i(fAnimData.fTextureRects[BossAnim::JumpLoop].width, fAnimData.fTextureRects[BossAnim::JumpLoop].height));
	Jump_loopRight->setNumFrames(fAnimData.fNumFrames[BossAnim::JumpLoop]);
	Jump_loopRight->Restart();

	fAnimations[AnimType::Jump] = Jump_loopRight;

	//JumpLoop when looking Left
	Animation* Jump_loopLeft = new Animation(*fAnimData.fTexture, sf::Vector2i(fAnimData.fTextureRects[BossAnim::Jump_loopLeft].left, fAnimData.fTextureRects[BossAnim::Jump_loopLeft].top),
		fAnimData.fReadDir[ReadDir::Left]);
	Jump_loopLeft->Repeat(true);
	Jump_loopLeft->setDuration(fAnimData.fDurations[BossAnim::JumpLoop]);
	Jump_loopLeft->setFrameSize(sf::Vector2i(fAnimData.fTextureRects[BossAnim::JumpLoop].width, fAnimData.fTextureRects[BossAnim::JumpLoop].height));
	Jump_loopLeft->setNumFrames(fAnimData.fNumFrames[BossAnim::JumpLoop]);
	Jump_loopLeft->Restart();

	fAnimations[AnimType::JumpLeft] = Jump_loopLeft;

	//Fall when looking Right
	Animation* FallRight = new Animation(*fAnimData.fTexture, sf::Vector2i(fAnimData.fTextureRects[BossAnim::FallRight].left, fAnimData.fTextureRects[BossAnim::FallRight].top),
		fAnimData.fReadDir[ReadDir::Right]);
	FallRight->Repeat(true);
	FallRight->setDuration(fAnimData.fDurations[BossAnim::Fall]);
	FallRight->setFrameSize(sf::Vector2i(fAnimData.fTextureRects[BossAnim::Fall].width, fAnimData.fTextureRects[BossAnim::Fall].height));
	FallRight->setNumFrames(fAnimData.fNumFrames[BossAnim::Fall]);
	FallRight->Restart();

	fAnimations[AnimType::Fall] = FallRight;

	//Fall when looking Left
	Animation* FallLeft = new Animation(*fAnimData.fTexture, sf::Vector2i(fAnimData.fTextureRects[BossAnim::FallLeft].left, fAnimData.fTextureRects[BossAnim::FallLeft].top),
		fAnimData.fReadDir[ReadDir::Left]);
	FallLeft->Repeat(true);
	FallLeft->setDuration(fAnimData.fDurations[BossAnim::Fall]);
	FallLeft->setFrameSize(sf::Vector2i(fAnimData.fTextureRects[BossAnim::Fall].width, fAnimData.fTextureRects[BossAnim::Fall].height));
	FallLeft->setNumFrames(fAnimData.fNumFrames[BossAnim::Fall]);
	FallLeft->Restart();

	fAnimations[AnimType::FallLeft] = FallLeft;


	//Death when looking right
	Animation* DeathRight = new Animation(*fAnimData.fTexture, sf::Vector2i(fAnimData.fTextureRects[BossAnim::DeathRight].left, fAnimData.fTextureRects[BossAnim::DeathRight].top),
		fAnimData.fReadDir[ReadDir::Right]);
	DeathRight->Repeat(false);
	DeathRight->setDuration(fAnimData.fDurations[BossAnim::Death]);
	DeathRight->setFrameSize(sf::Vector2i(fAnimData.fTextureRects[BossAnim::Death].width, fAnimData.fTextureRects[BossAnim::Death].height));
	DeathRight->setNumFrames(fAnimData.fNumFrames[BossAnim::Death]);
	DeathRight->Restart();

	fAnimations[AnimType::Death] = DeathRight;

	//Death when looking left
	Animation* DeathLeft = new Animation(*fAnimData.fTexture, sf::Vector2i(fAnimData.fTextureRects[BossAnim::DeathLeft].left, fAnimData.fTextureRects[BossAnim::DeathLeft].top),
		fAnimData.fReadDir[ReadDir::Left]);
	DeathLeft->Repeat(false);
	DeathLeft->setDuration(fAnimData.fDurations[BossAnim::Death]);
	DeathLeft->setFrameSize(sf::Vector2i(fAnimData.fTextureRects[BossAnim::Death].width, fAnimData.fTextureRects[BossAnim::Death].height));
	DeathLeft->setNumFrames(fAnimData.fNumFrames[BossAnim::Death]);
	DeathLeft->Restart();

	fAnimations[AnimType::DeathLeft] = DeathLeft;

}