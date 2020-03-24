#pragma once
#include <string>
#include <vector>
#include <memory>
#include "../core/SceneNode.h"
#include "../core/renderer/Camera.h"

class IScene {
public:

	virtual void input() = 0;

	virtual void update() = 0;

	virtual void render() = 0;

	std::shared_ptr<Camera> getCamera() {
		return m_camera;
	}

	std::vector<std::shared_ptr<SceneNode> > getAllEntities() {
		return m_root->getChildren();
	}

	virtual ~IScene() {}

protected:
	std::string m_name;
	std::unique_ptr<SceneNode> m_root = nullptr;
	std::shared_ptr<Camera> m_camera = nullptr;
};