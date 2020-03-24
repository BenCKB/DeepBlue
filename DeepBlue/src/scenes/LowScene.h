#pragma once
#include "IScene.h"
#include "../math/Vector.h"
#include "../core/renderer/Mesh.h"
#include "../../res/Materials/PBRMaterial.h"
#include "../../res/Materials/LambertMaterial.h"
#include "../core/component/CMeshRenderer.h"
#include "../core/renderer/GraphicsContext.h"
#include "../core/component/PointLight.h"

class LowScene : public IScene {
public:
	LowScene() {
		/* ===== Initialize Scene Variables ===== */

		//Create root node for scene graph
		m_root = std::make_unique<SceneNode>("root");
		
		//Set up main camera and update GraphicsContext
		m_camera = std::make_shared<Camera>(Vec3f(0.0f, 0.0f, 0.0f), Vec3f(0.0f, 1.0f, 0.0f), Vec3f(0.0f, 0.0f, 1.0f));
		GraphicsContext::getInstance().setCamera(m_camera);

		//Transform camera
		m_camera->translate(Vec3f(0.f, 0.f, -1.f), 5.0f);

		/* ===== Load Model ===== */
		//1. Create SceneNode
		std::shared_ptr<SceneNode> sphere = std::make_shared<SceneNode>("Sphere Model");

		//2. Create shader object using vertex and fragment shader
		std::shared_ptr<Shader> shader = std::make_shared<Shader>();
		shader->addShader(ShaderType::VERTEXSHADER, ".\\res\\Shaders\\PBRShader.vert");
		shader->addShader(ShaderType::FRAGMENTSHADER, ".\\res\\Shaders\\PBRShader.frag");
		shader->bind();
		shader->getUniforms();

		//3. Set up model and geometry data
		std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(".\\res\\Models\\sphere.obj");
		//Set node transform
		sphere->setTransform(std::make_shared<Transform>(mesh->modelTransform));

		//4. Set up material
		std::shared_ptr<PBRMaterial> material = std::make_shared<PBRMaterial>(shader);

		mesh->setMaterial(material);

		//5. Add MeshRenderer component to enable rendering
		std::shared_ptr<CMeshRenderer> meshRenderer = std::make_shared<CMeshRenderer>(mesh);
		sphere->addComponent(meshRenderer);

		//6. Add node to scene graph
		m_root->addChild(sphere);

		/* ===== Add Light ===== */
		//Create light node
		std::shared_ptr<SceneNode> pointLight = std::make_shared<SceneNode>("REAL POINT LIGHT");
		
		//Create PointLight component
		std::shared_ptr<PointLight> lightComponent = std::make_shared<PointLight>(Vec3f(1.0f, 1.0f, 1.0f), 100.0f, Vec3f(0.0f, 10.0f, 0.0f), 1.0f, 0.7f, 1.8f);
		
		//Set nodes' component
		pointLight->setTransform(std::make_shared<Transform>(lightComponent->getTransform()));
		
		//Add component to node
		pointLight->addComponent(lightComponent);

		//Register light, to be added during rendering
		GraphicsContext::getInstance().registerLight(lightComponent);

		//Add node to scene graph
		m_root->addChild(pointLight);
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

	~LowScene() {}
};