#pragma once
#include "Component.h"
namespace GUI {
	
	class Container : public Component
	{
	public:
		Container();
		~Container() override;

		void pack(Component* component);
		void clearContainer();

		virtual bool isSelectable() const override;
		virtual void handleEvent(const sf::Event& e) override;
		virtual void update(sf::Time dt) override;

	private:
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

		bool hasSelection() const;
		void select(std::size_t index);
		void selectNext();
		void selectPrevious();

	private:
		std::vector<Component*> fChildren;
		int fSelectedChild;
	};

}