#include "Button.h"
#include "UtilityFunc.h"

GUI::Button::Button(State::Context context, sf::IntRect texRectNorm, sf::IntRect texRectSelect, sf::IntRect texRectPress, sf::Font& font, unsigned int charSize)
	:fText("", font, charSize)
	, fSprite(context.textures->get(Textures::ID::Buttons))
	, fCallback(), fIsToggle(false)
	, textureRectNormal(texRectNorm), textureRectSelected(texRectSelect), textureRectPressed(texRectPress)
{
	changeTexture(Normal);
	Utility::CenterOrigin(fSprite);

	sf::FloatRect bounds = fSprite.getGlobalBounds();
	fText.setPosition(bounds.left + bounds.width / 2.f, bounds.top + bounds.height / 2.f);
}

GUI::Button::Button(sf::IntRect texRectNorm, sf::IntRect texRectSelect, sf::IntRect texRectPress, sf::Font & font, const sf::Texture & ButtonTex)
	:fText("", font, 16),
	fSprite(ButtonTex),
	fCallback(), fIsToggle(false)
	, textureRectNormal(texRectNorm), textureRectSelected(texRectSelect), textureRectPressed(texRectPress)
{
	changeTexture(Normal);
	Utility::CenterOrigin(fSprite);

	sf::FloatRect bounds = fSprite.getGlobalBounds();
	fText.setPosition(bounds.left + bounds.width / 2.f, bounds.top + bounds.height / 2.f);
}

void GUI::Button::setCallback(Callback cb)
{
	fCallback = cb;
}

void GUI::Button::setText(const std::string & text)
{
	fText.setString(text);
	Utility::CenterOrigin(fText);
}

void GUI::Button::setToggle(bool flag)
{
	fIsToggle = flag;
}

bool GUI::Button::isSelectable() const
{
	return true;
}

void GUI::Button::select()
{
	Component::select();
	changeTexture(Selected);
}

void GUI::Button::deselect()
{
	Component::deselect();
	changeTexture(Normal);
}

void GUI::Button::activate()
{
	Component::activate();
	if (fIsToggle) {
		changeTexture(Type::Pressed);
	}

	if (fCallback)
		fCallback();

	if (!fIsToggle)
		deactivate();
}

void GUI::Button::deactivate()
{
	Component::deactivate();

	if (fIsToggle)
	{
		if (isSelected())
			changeTexture(Selected);
		else
			changeTexture(Normal);
	}
}

void GUI::Button::handleEvent(const sf::Event & e)
{
}

void GUI::Button::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(fSprite, states);
	if(fText.getString() != "")
		target.draw(fText, states);
}

void GUI::Button::changeTexture(Type buttonType)
{
	sf::IntRect textureRect;
	switch (buttonType)
	{
	case Type::Normal:
		textureRect = textureRectNormal;
		break;
	case Type::Selected:
		textureRect = textureRectSelected;
		break;
	case Type::Pressed:
		textureRect = textureRectPressed;
		break;
	default:
		textureRect = sf::IntRect(0, 0, 0, 0);
		break;
	}
	fSprite.setTextureRect(textureRect);
}
