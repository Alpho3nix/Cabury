#include "ShoppingNPC.h"
#include "Button.h"
#include "Player.h"

ShoppingNPC::ShoppingNPC(int hp, NonPlayerChar::Type type, State::Context context)
	:NonPlayerChar(hp, type, context), fShopMenu()
{
	setupAnims();
}

void ShoppingNPC::handleEvent(const sf::Event& e, CommandQueue& commands)
{
	if (!fMenuCreated && fBeginInteraction)
	{
		if (e.type == sf::Event::KeyReleased && e.key.code == sf::Keyboard::Enter)
		{
			createMenu(commands);
			fMenuCreated = true;
			setCurrentAnimation(AnimType::Transition);
		}
	}
	else if(fMenuCreated && fBeginInteraction)
	{
		fShopMenu.handleEvent(e);
	}
}

void ShoppingNPC::createMenu(CommandQueue& commands)
{
	//buttons size : 130 x 64
	const sf::Texture& tex = fContext.textures->get(Textures::ID::ShopButtons);
	sf::Vector2i buttonSize(130, 64);

	int buttonNumber = tex.getSize().y / buttonSize.y;
	if (buttonNumber > 6)
	{
		throw std::runtime_error("failed to create shop menu, number of buttons on one page is too big");
		return;
	}
	
	for (int i = 0; i < buttonNumber; ++i)
	{
		GUI::Button* offer = new GUI::Button(sf::IntRect(0, buttonSize.y * i, buttonSize.x, buttonSize.y),
			sf::IntRect(buttonSize.x, buttonSize.y * i, buttonSize.x, buttonSize.y),
			sf::IntRect(2 * buttonSize.x, buttonSize.y * i, buttonSize.x, buttonSize.y), fContext.fonts->get(Fonts::ID::Sansation), tex);
		offer->setPosition(buttonSize.x * i - 150.f, -100.f + buttonSize.y * int(i / 3));
		switch (i)
		{
		case AddingResistance:
			offer->setCallback([&commands]() {
				commands.addCommand(Category::PlayerChar, derivedAction<Player>([](Player& player, sf::Time)
					{
						if (player.getMoney() >= 50)
						{
							player.addMoney(-50);
							player.addResistance(10);
						}
					}));
				});
			break;
		case AddingStrength:
			offer->setCallback([&commands]() {
				commands.addCommand(Category::PlayerChar, derivedAction<Player>([](Player& player, sf::Time)
					{
						if (player.getMoney() >= 50)
						{
							player.addMoney(-50);
							player.addStrength(10);
						}
					}));
				});
			break;
		default:
			break;
		}

		fShopMenu.pack(offer);
		

	}
}

void ShoppingNPC::reset()
{
	NonPlayerChar::reset();
	fMenuCreated = false;
	fStopShowing = false;
	fShopMenu.clearContainer();
}

void ShoppingNPC::updateNode(sf::Time dt, CommandQueue& commands)
{
	NonPlayerChar::updateNode(dt, commands);

	if (fInteractBuffer && !fMenuCreated)
	{
		
		if (!fMenuCreated) {
			fAnimations[AnimType::Transition]->setReadingDirection(-fAnimData.fReadDir[Right]);
			fAnimations[AnimType::Transition]->setStartPoint(sf::Vector2i(fAnimData.fTextureRects[NPCAnim::BeginInteractIdle].left - fAnimData.fTextureRects[NPCAnim::BeginInteractTransition].width,
				fAnimData.fTextureRects[NPCAnim::BeginInteractTransition].top));

			setCurrentAnimation(AnimType::Transition);
		}
	}
	else if (!fMenuCreated && fAnimations[AnimType::Transition]->isFinished())
	{
		fAnimations[AnimType::Transition]->setReadingDirection(fAnimData.fReadDir[Right]);
		fAnimations[AnimType::Transition]->setStartPoint(sf::Vector2i(fAnimData.fTextureRects[NPCAnim::BeginInteractTransition].left, fAnimData.fTextureRects[NPCAnim::BeginInteractTransition].top));

		setCurrentAnimation(AnimType::Idle);
	}

	if (fMenuCreated)
	{
		fStopShowing = true;
		if (fAnimations[AnimType::Transition]->isFinished())
		{
			setCurrentAnimation(AnimType::Idle2);
		}
	}

	fInteractBuffer = fMenuCreated;
}

void ShoppingNPC::drawSthBetween(sf::RenderTarget& target, sf::RenderStates states) const
{
	if(fBeginInteraction && fMenuCreated)
		target.draw(fShopMenu, states);
}

void ShoppingNPC::setupAnims()
{
	//Idle when the shop's opened
	Animation* idleShop = new Animation(*fAnimData.fTexture, sf::Vector2i(fAnimData.fTextureRects[NPCAnim::BeginInteractIdle].left, fAnimData.fTextureRects[NPCAnim::BeginInteractIdle].top), fAnimData.fReadDir[Right]);
	idleShop->Repeat(true);
	idleShop->setDuration(fAnimData.fDurations[NPCAnim::BeginInteractIdle]);
	idleShop->setFrameSize(sf::Vector2i(fAnimData.fTextureRects[NPCAnim::BeginInteractIdle].width, fAnimData.fTextureRects[NPCAnim::BeginInteractIdle].height));
	idleShop->setNumFrames(fAnimData.fNumFrames[NPCAnim::BeginInteractIdle]);
	idleShop->Restart();

	fAnimations[AnimType::Idle2] = idleShop;

	//Transition from idle to idle2
	Animation* transitionShop = new Animation(*fAnimData.fTexture, sf::Vector2i(fAnimData.fTextureRects[NPCAnim::BeginInteractTransition].left, fAnimData.fTextureRects[NPCAnim::BeginInteractTransition].top), fAnimData.fReadDir[Right]);
	transitionShop->Repeat(false);
	transitionShop->setDuration(fAnimData.fDurations[NPCAnim::BeginInteractTransition]);
	transitionShop->setFrameSize(sf::Vector2i(fAnimData.fTextureRects[NPCAnim::BeginInteractTransition].width, fAnimData.fTextureRects[NPCAnim::BeginInteractTransition].height));
	transitionShop->setNumFrames(fAnimData.fNumFrames[NPCAnim::BeginInteractTransition]);
	transitionShop->Restart();

	fAnimations[AnimType::Transition] = transitionShop;
}
