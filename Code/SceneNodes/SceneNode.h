#pragma once
#include "SFML/Graphics.hpp"
#include "Categories.h"
#include "CommandQueue.h"
#include <set>

//a scene node is something that can be drawn to the screen
class SceneNode : public sf::Drawable, public sf::Transformable
{
public:
	typedef std::pair<SceneNode*, SceneNode*> Pair;
	SceneNode(Category category = Category::None);

	void attachChild(SceneNode* child);
	SceneNode* detachChild(const SceneNode& node);
	virtual unsigned int getCategory() const;
	sf::Transform getWorldTransform() const;
	sf::Vector2f getWorldPosition() const;
	void setWorldPosition(float x, float y);
	void setWorldPosition(sf::Vector2f pos);

	virtual sf::FloatRect getBoundingRect() const;
	
	void updateGraph(sf::Time dt, CommandQueue& commands);
	void OnCommand(const Command& command, sf::Time dt);

	void checkNodeCollision(SceneNode& node, std::set<Pair>& colPairs);
	void checkSceneCollision(SceneNode& graph, std::set<Pair>& colPairs);
	virtual bool isRemovable() const;
	virtual bool isDestroyed() const;
	void removeDeadSceneNodes();

	virtual ~SceneNode();

private:

	void draw(sf::RenderTarget& target, sf::RenderStates states) const final override;
	void drawBoundingRect(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
	void drawChildren(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual void updateNode(sf::Time dt, CommandQueue& commands);

protected:
	std::vector<SceneNode*> fChildren;
	SceneNode* fParent;
	bool fDrawChildren = true;
	bool fUpdateChildren = true;
private:

	Category fCategory;

};

bool collide(SceneNode* rhs, SceneNode* lhs);