#pragma once
#include "IScene.h"
#include "../math/Vector.h"
#include "../core/renderer/Mesh.h"
#include "../../res/Materials/PBRMaterial.h"
#include "../../res/Materials/LambertMaterial.h"
#include "../core/component/CMeshRenderer.h"
#include "../core/renderer/GraphicsContext.h"

class DemoScene : public IScene {
public:
	DemoScene() {
		m_root = std::make_unique<SceneNode>("root");
		m_camera = std::make_shared<Camera>(Vec3f(0.0, 1.0, 4.0), Vec3f(0.0, 1.0, 0.0), Vec3f(0.0, 0.0, -1.0));
	

		//Set up current camera
		GraphicsContext::getInstance().setCamera(m_camera);

		/* ========== BEGIN SPHERE MODEL ========== */
		std::shared_ptr<SceneNode> gun = std::make_shared<SceneNode>("Test Model");
		
		std::shared_ptr<Shader> shader;
		shader = std::make_shared<Shader>();
		shader->addShader(ShaderType::VERTEXSHADER, ".\\res\\Shaders\\LambertShader.vert");
		shader->addShader(ShaderType::FRAGMENTSHADER, ".\\res\\Shaders\\LambertShader.frag");
		shader->bind();
		shader->getUniforms();

		/* MESH SETUP */
		std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(".\\res\\Models\\cube.obj");

		//mesh->scale(0.001f);

		gun->setTransform(std::make_shared<Transform>(mesh->modelTransform));

		/* MATERIAL SETUP */
		std::shared_ptr<LambertMaterial> material = std::make_shared<LambertMaterial>(shader);
		mesh->setMaterial(material);

		std::shared_ptr<CMeshRenderer> meshRenderer = std::make_shared<CMeshRenderer>(mesh);

		gun->addComponent(meshRenderer);

		m_root->addChild(gun);
		/* ========== END SPHERE MODEL ========== */

		std::shared_ptr<SceneNode> directLight = std::make_shared<SceneNode>("REAL DIRECTIONAL LIGHT");
		std::shared_ptr<DirectionalLight> lightComponent = std::make_shared<DirectionalLight>(Vec3f(0.8f, 0.5f, 0.5f), 10000.0f, Vec3f(0.0f, 10.0f, 0.0f));

		directLight->setTransform(std::make_shared<Transform>(lightComponent->getTransform()));
		directLight->addComponent(lightComponent);

		GraphicsContext::getInstance().registerLight(lightComponent);
		m_root->addChild(directLight);

	}

	void input() {
		m_root->input();
	}

	void update() {
		m_root->update();
	}

	void render() {
		m_root->render();
	}

	~DemoScene() {}
};