#pragma once
#include <vector>
#include <string>

#include "../math/Transform.h"
#include "component/IComponent.h"

class SceneNode {
public:
	SceneNode() : m_name("Nameless"), m_transform(std::make_shared<Transform>()) {}

	SceneNode(std::string name) : m_name(name), m_transform(std::make_shared<Transform>()) {}

	std::shared_ptr<Transform> getTransform() {
		return m_transform;
	}

	void setTransform(std::shared_ptr<Transform> t) {
		m_transform = t;
	}

	void addComponent(std::shared_ptr<IComponent> comp) {
		comp->setParent(this);
		m_components.push_back(comp);
	}

	void addChild(std::shared_ptr<SceneNode> child) {
		//Calculate overall transformation matrix from parent
		std::shared_ptr<Transform> trans = std::make_shared<Transform>();
		trans->t = m_transform->t + child->getTransform()->t;
		trans->r = m_transform->r + child->getTransform()->r;
		trans->s = m_transform->s + child->getTransform()->s - Vec3f(1.0, 1.0, 1.0);

		child->setTransform(trans);
		m_children.push_back(child);
	}

	std::vector<std::shared_ptr<SceneNode> > getChildren() {
		return m_children;
	}

	std::string getName() {
		return m_name;
	}

	void input() {

		for (std::shared_ptr<IComponent> component : m_components) {
			component->input();
		}

		for (std::shared_ptr<SceneNode> node : m_children) {
			node->input();
		}
	}

	void update() {

		for (std::shared_ptr<IComponent> component : m_components) {
			component->update();
		}

		for (std::shared_ptr<SceneNode> node : m_children) {
			node->update();
		}
	}

	void render() {

		for (std::shared_ptr<IComponent> component : m_components) {
			component->render();
		}

		for (std::shared_ptr<SceneNode> node : m_children) {
			node->render();
		}
	}

private:
	std::vector<std::shared_ptr<SceneNode> > m_children;
	std::vector<std::shared_ptr<IComponent> > m_components;

	std::string m_name;
	std::shared_ptr<Transform> m_transform;
};