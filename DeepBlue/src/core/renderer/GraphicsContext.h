#pragma once
#include "Camera.h"
#include "../component/DirectionalLight.h"
#include "../component/PointLight.h"

class GraphicsContext {
public:
	static GraphicsContext& getInstance() {
		static GraphicsContext context;
		return context;
	}

	std::shared_ptr<Camera> getCamera() {
		return m_camera;
	}

	void setCamera(std::shared_ptr<Camera> camera) {
		m_camera = camera;
	}

	void registerLight(std::shared_ptr<PointLight> light) {
		m_pointlights.push_back(light);
	}

	void registerLight(std::shared_ptr<DirectionalLight> light) {
		m_directionallights.push_back(light);
	}


	std::vector<std::shared_ptr<PointLight> > getPointLights()
	{
		return m_pointlights;
	}

	std::vector<std::shared_ptr<DirectionalLight> > getDirectionalLights()
	{
		return m_directionallights;
	}

private:
	std::shared_ptr<Camera> m_camera;
	std::vector<std::shared_ptr<PointLight> > m_pointlights;
	std::vector<std::shared_ptr<DirectionalLight> > m_directionallights;
	GraphicsContext() {}
	GraphicsContext(const GraphicsContext&);
};