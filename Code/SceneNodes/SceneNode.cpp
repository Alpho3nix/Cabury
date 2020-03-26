#include "SceneNode.h"
#include <iostream>

SceneNode::SceneNode(Category category)
	:fCategory(category), fChildren(), fParent(nullptr)
{
}

void SceneNode::attachChild(SceneNode * child)
{
	child->fParent = this;
	fChildren.push_back(child);
}

SceneNode* SceneNode::detachChild(const SceneNode & node)
{
	auto found = std::find_if(fChildren.begin(), fChildren.end(), [&](SceneNode* child) {return child == &node; });
	assert(found != fChildren.end());
	SceneNode* result = *found;
	result->fParent = nullptr;
	fChildren.erase(found);
	return result;
}

unsigned int SceneNode::getCategory() const
{
	return fCategory;
}

sf::Transform SceneNode::getWorldTransform() const
{
	sf::Transform transform = sf::Transform::Identity;
	for (const SceneNode* node = this; node != nullptr; node = node->fParent)
	{
		transform = node->getTransform() * transform;
	}
	return transform;
}

sf::Vector2f SceneNode::getWorldPosition() const
{
	return getWorldTransform() * sf::Vector2f();
}

void SceneNode::setWorldPosition(float x, float y)
{
	sf::Vector2f pos(x, y);
	if(pos.y != getWorldPosition().y)
		setPosition(getPosition().x + getWorldPosition().x - pos.x, getPosition().y + getWorldPosition().y - pos.y);
}

void SceneNode::setWorldPosition(sf::Vector2f pos)
{
	if (pos.y != getWorldPosition().y)
		setPosition(getPosition().x + getWorldPosition().x - pos.x, getPosition().y + getWorldPosition().y - pos.y);
}

sf::FloatRect SceneNode::getBoundingRect() const
{
	return sf::FloatRect();
}

void SceneNode::updateNode(sf::Time dt, CommandQueue& commands)
{
}

void SceneNode::updateGraph(sf::Time dt, CommandQueue& commands)
{
	updateNode(dt, commands);
	if (fUpdateChildren) {
		for (auto child : fChildren)
		{
			child->updateGraph(dt, commands);
		}
	}
}

void SceneNode::OnCommand(const Command & command, sf::Time dt)
{
	if (command.category & getCategory())
	{
		command.action(*this, dt);
	}

	if (fUpdateChildren) {
		for (SceneNode* child : fChildren)
		{
			child->OnCommand(command, dt);
		}
	}
}

void SceneNode::checkNodeCollision(SceneNode & node, std::set<Pair>& colPairs)	//using a set because of its "no duplicates" property
{
	if (this != &node && collide(this, &node) && !isRemovable() && !node.isRemovable())
	{
		colPairs.insert(std::minmax(this, &node));	//so that there's no collision pairs that are the same just in reverse order
	}

	for (SceneNode* child : fChildren)
	{
		child->checkNodeCollision(node, colPairs);
	}
}

void SceneNode::checkSceneCollision(SceneNode & graph, std::set<Pair>& colPairs)
{
	checkNodeCollision(graph, colPairs);

	for (SceneNode* child : graph.fChildren)
	{
		checkSceneCollision(*child, colPairs);
	}
}

bool SceneNode::isRemovable() const
{
	return false;
}

bool SceneNode::isDestroyed() const
{
	return false;
}

void SceneNode::removeDeadSceneNodes()
{
	//Remove all children that are removable
	auto deadNodesBegin = std::remove_if(fChildren.begin(), fChildren.end(), std::mem_fn(&SceneNode::isRemovable));
	for (auto itr = deadNodesBegin; itr != fChildren.end(); ++itr)
	{
		if ((*itr)->isRemovable()) {
			delete (*itr);
		}
	}
	fChildren.erase(deadNodesBegin, fChildren.end());

	//call function recursively for all remaining children
	std::for_each(fChildren.begin(), fChildren.end(), std::mem_fn(&SceneNode::removeDeadSceneNodes));
}


SceneNode::~SceneNode()
{
	for (SceneNode* child : fChildren)
	{
		delete child;
	}
}

void SceneNode::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	drawCurrent(target, states);
	if(fDrawChildren)
		drawChildren(target, states);

	//optional
	//drawBoundingRect(target, states);
}

void SceneNode::drawBoundingRect(sf::RenderTarget & target, sf::RenderStates states) const
{
	sf::FloatRect boundingRect = getBoundingRect();
	sf::RectangleShape Box;
	Box.setPosition(boundingRect.left, boundingRect.top);
	Box.setSize(sf::Vector2f(boundingRect.width, boundingRect.height));
	Box.setFillColor(sf::Color::Transparent);
	Box.setOutlineColor(sf::Color::Green);
	Box.setOutlineThickness(1.f);
	target.draw(Box);
}

void SceneNode::drawCurrent(sf::RenderTarget & target, sf::RenderStates states) const
{

}

void SceneNode::drawChildren(sf::RenderTarget & target, sf::RenderStates states) const
{
	for (const SceneNode* child : fChildren)
	{
		child->draw(target, states);
	}
}

bool collide(SceneNode * rhs, SceneNode * lhs)
{
	return rhs->getBoundingRect().intersects(lhs->getBoundingRect());
}
