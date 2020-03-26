#pragma once
#include "SFML/Graphics.hpp"

namespace GUI
{
	class Component : public sf::Drawable, public sf::Transformable, private sf::NonCopyable
	{
	public:
		Component();
		virtual ~Component();

		virtual bool isSelectable() const = 0;
		bool isSelected() const;
		virtual void select();
		virtual void deselect();
		virtual bool isActive() const;
		virtual void activate();
		virtual void deactivate();
		virtual void handleEvent(const sf::Event& e) = 0;
		virtual void update(sf::Time dt);

	private:
		bool fSelected;
		bool fActive;


	};
}