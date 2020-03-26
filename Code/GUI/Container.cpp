#include "Container.h"

GUI::Container::Container()
	:fChildren(), fSelectedChild(-1)
{
}

GUI::Container::~Container()
{
	for (Component* comp : fChildren)
	{
		delete comp;
	}
}

void GUI::Container::pack(Component * component)
{
	fChildren.push_back(component);
	if (!hasSelection() && component->isSelectable())
	{
		select(fChildren.size() - 1);
	}
}

void GUI::Container::clearContainer()
{
	fSelectedChild = -1;
	for (Component* comp : fChildren)
	{
		delete comp;
	}
	fChildren.clear();
}

bool GUI::Container::isSelectable() const
{
	return false;
}

void GUI::Container::handleEvent(const sf::Event & e)
{
	if (hasSelection() && fChildren[fSelectedChild]->isActive())
	{
		fChildren[fSelectedChild]->handleEvent(e);
	}
	else if (e.type == sf::Event::KeyReleased)
	{
		if (e.key.code == sf::Keyboard::Up || e.key.code == sf::Keyboard::Left)	//it's from top to bottom (or right to left) so up(or Left) is previous and down(or Right) is next
		{
			selectPrevious();
		}
		else if (e.key.code == sf::Keyboard::Down || e.key.code == sf::Keyboard::Right)
		{
			selectNext();
		}
		else if (e.key.code == sf::Keyboard::Return)	
		{
			if (hasSelection())
			{
				fChildren[fSelectedChild]->activate();
			}
		}
	}
}

void GUI::Container::update(sf::Time dt)
{
	for (Component* comp : fChildren)
	{
		comp->update(dt);
	}
}

void GUI::Container::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	states.transform *= getTransform();

	for (Component* child : fChildren)
	{
		target.draw(*child, states);
	}
}

bool GUI::Container::hasSelection() const
{
	return (fSelectedChild >= 0);
}

void GUI::Container::select(std::size_t index)
{
	if (fChildren[index]->isSelectable()) {
		
		if (hasSelection())
		{
			fChildren[fSelectedChild]->deselect();
		}
		fChildren[index]->select();
		fSelectedChild = index;
	}
}

void GUI::Container::selectNext()
{
	if (!hasSelection())
		return;

	int next = fSelectedChild;
	do
	{
		next = (next + 1) % fChildren.size();

	} while (!fChildren[next]->isSelectable());
	
	select(next);
}

void GUI::Container::selectPrevious()
{
	if (!hasSelection())
		return;

	int previous = fSelectedChild;
	do
	{
		previous = (previous + fChildren.size() - 1) % fChildren.size();
	} while (!fChildren[previous]->isSelectable());

	select(previous);
}
