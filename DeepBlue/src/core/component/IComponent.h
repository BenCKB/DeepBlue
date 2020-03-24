#pragma once

//Forward declaring SceneNodes (Entities)
class SceneNode;

class IComponent {
public:

	virtual void input() = 0;
	
	virtual void update() = 0;

	virtual void render() = 0;

	void setParent(SceneNode* parent) {
		m_parent = parent;
	}

	virtual ~IComponent() {}

protected:
	SceneNode* m_parent = nullptr;
};