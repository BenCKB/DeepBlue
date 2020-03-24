#pragma once
#include "IScene.h"
#include "../math/Vector.h"
#include "../core/renderer/Mesh.h"
#include "../../res/Materials/PBRMaterial.h"
#include "../../res/Materials/LambertMaterial.h"
#include "../core/component/CMeshRenderer.h"
#include "../core/renderer/GraphicsContext.h"
#include "../util/ObjLoader.h"

class HighScene : public IScene {
public:
	HighScene() {
		m_root = std::make_unique<SceneNode>("root");

		m_camera = std::make_shared<Camera>(Vec3f(0.0, 0.0, 0.0), Vec3f(0.0, 1.0, 0.0), Vec3f(0.0, 0.0, -1.0));

		//Set up current camera
		GraphicsContext::getInstance().setCamera(m_camera);

		ObjLoader loader(".\\res\\Models\\sponza_g.obj", ".\\res\\Models\\sponzaPBR.mtl"); //sponza_g

		//m_camera->rotateY(-1.2f);
		m_camera->translate(Vec3f(-1.0, 0.0, 0.0), 100);


		/* Real point Light 1 */
		std::shared_ptr<SceneNode> pointLight = std::make_shared<SceneNode>("REAL POINT LIGHT");
		std::shared_ptr<PointLight> lightComponent = std::make_shared<PointLight>(Vec3f(0.8f, 0.5f, 0.5f), 10000.0f, Vec3f(0.0f, 10.0f, 0.0f), 1.0f, 0.7f, 1.8f);

		pointLight->setTransform(std::make_shared<Transform>(lightComponent->getTransform()));
		pointLight->addComponent(lightComponent);

		GraphicsContext::getInstance().registerLight(lightComponent);
		m_root->addChild(pointLight);

		/* Real point Light  2*/
		std::shared_ptr<SceneNode> pointLight2 = std::make_shared<SceneNode>("REAL POINT LIGHT 2");
		std::shared_ptr<PointLight> lightComponent2 = std::make_shared<PointLight>(Vec3f(0.0f, 1.0f, 0.0f), 1000.0f, Vec3f(0.0f, 20.0f, 0.0f), 1.0f, 0.7f, 1.8f);

		pointLight2->setTransform(std::make_shared<Transform>(lightComponent2->getTransform()));
		pointLight2->addComponent(lightComponent2);

		GraphicsContext::getInstance().registerLight(lightComponent2);
		m_root->addChild(pointLight2);

		/* Real point Light 3 */
		std::shared_ptr<SceneNode> pointLight3 = std::make_shared<SceneNode>("REAL POINT LIGHT 3");
		std::shared_ptr<PointLight> lightComponent3 = std::make_shared<PointLight>(Vec3f(0.0f, 0.0f, 1.0f), 1000.0f, Vec3f(0.0f, 30.0f, 0.0f), 1.0f, 0.7f, 1.8f);

		pointLight3->setTransform(std::make_shared<Transform>(lightComponent3->getTransform()));
		pointLight3->addComponent(lightComponent3);

		GraphicsContext::getInstance().registerLight(lightComponent3);
		m_root->addChild(pointLight3);

		for (auto m : loader.meshes) {
			std::shared_ptr<SceneNode> model = std::make_shared<SceneNode>(m->NAME);

			std::shared_ptr<Shader> shader;
			shader = std::make_shared<Shader>();
			shader->addShader(ShaderType::VERTEXSHADER, ".\\res\\Shaders\\PBRShader.vert");
			shader->addShader(ShaderType::FRAGMENTSHADER, ".\\res\\Shaders\\PBRShader.frag");
			shader->bind();
			shader->getUniforms();

			///* MESH SETUP */ 
			std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(&m->VERTS, &m->NORMS, &m->TEXVERTS, &m->IND);

			mesh->scale(0.1f);
			//mesh->scale(1.0f);

			model->setTransform(std::make_shared<Transform>(mesh->modelTransform));

			///* MATERIAL SETUP */
			std::shared_ptr<PBRMaterial> material;
			material = std::make_shared<PBRMaterial>(shader);

			std::string base = ".\\res\\Textures\\";
			std::string albedo;
			std::string metallic;
			std::string roughness;
			std::string normal;

			for (auto mat : loader.materials) {
				if (mat->NAME.compare(m->MAT) == 0) {
					albedo = base + mat->DIFFUSE;
					metallic = base + mat->AMBIENT;
					roughness = base + mat->SPECULAR;
					normal = base + mat->NORMAL;
					break;
				}
			}

			material->addTextures(albedo, roughness, metallic, normal);

			mesh->setMaterial(material);

			std::shared_ptr<CMeshRenderer> meshRenderer = std::make_shared<CMeshRenderer>(mesh);

			model->addComponent(meshRenderer);

			m_root->addChild(model);
		}


		std::shared_ptr<SceneNode> light = std::make_shared<SceneNode>("Light Model");

		std::shared_ptr<Shader> shaderLight;
		shaderLight = std::make_shared<Shader>();
		shaderLight->addShader(ShaderType::VERTEXSHADER, ".\\res\\Shaders\\LambertShader.vert");
		shaderLight->addShader(ShaderType::FRAGMENTSHADER, ".\\res\\Shaders\\LambertShader.frag");
		shaderLight->bind();
		shaderLight->getUniforms();

		/* MESH SETUP */
		std::shared_ptr<Mesh> meshLight = std::make_shared<Mesh>(".\\res\\Models\\cube.obj");

		meshLight->move(0.0, 25.0, 0.0);
		meshLight->scale(0.3f);

		light->setTransform(std::make_shared<Transform>(meshLight->modelTransform));

		///* MATERIAL SETUP */
		std::shared_ptr<LambertMaterial> materialLight;
		materialLight = std::make_shared<LambertMaterial>(shaderLight);
		//materialLight->addTextures();
		meshLight->setMaterial(materialLight);

		std::shared_ptr<CMeshRenderer> meshRendererLight = std::make_shared<CMeshRenderer>(meshLight);

		light->addComponent(meshRendererLight);

		m_root->addChild(light);
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

	~HighScene() {
		
	}
};