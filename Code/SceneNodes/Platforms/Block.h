#include "ResourceIdentifiers.h"

class Block : public sf::Drawable, public sf::Transformable
{
public:

	enum class Type
	{
		BeginBlock, BetweenBlock, EndBlock, SoloBlock
	};

	Block(TexturesHolder& textures, Type type, int width = 1, int height = 1);
	Block();

	sf::IntRect getSize() const;
	
private:
	 
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	sf::Sprite fVisualBlock;
};