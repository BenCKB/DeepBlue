#pragma once
#include "IScene.h"
#include "../math/Vector.h"
#include "../core/renderer/Mesh.h"
#include "../../res/Materials/PBRMaterial.h"
#include "../core/component/CMeshRenderer.h"
#include "../core/renderer/GraphicsContext.h"
#include "../util/ObjLoader.h"

//TODO: Check includes
class MediumScene : public IScene {
public:
	MediumScene() {
		
		/* ===== Initialize Scene Variables ===== */
		
		//Create root node for scene graph
		m_root = std::make_unique<SceneNode>("root");

		//Set up main camera and update GraphicsContext
		m_camera = std::make_shared<Camera>(Vec3f(0.0, 0.0, -12.0), Vec3f(0.0, 1.0, 0.0), Vec3f(0.0, 0.0, 1.0));
		GraphicsContext::getInstance().setCamera(m_camera);

		//Transform camera
		m_camera->rotateY(-0.6f);
		m_camera->rotateX(0.2f);
		m_camera->translate(Vec3f(1.0, 0.0, 0.0), 5);
		
		/* ===== Load Models ===== */

		//Load .obj model with corresponding material file 
		ObjLoader* loader = new ObjLoader(".\\res\\Models\\helmet.obj", ".\\res\\Models\\helmet.mtl");
		
		//Obj files may contain multiple groups/objects
		for (auto m : loader->meshes) {

			//1. Create SceneNode
			std::shared_ptr<SceneNode> model = std::make_shared<SceneNode>(m->NAME);

			//2. Create shader object using vertex and fragment shader
			std::shared_ptr<Shader> shader = std::make_shared<Shader>();
			shader->addShader(ShaderType::VERTEXSHADER, ".\\res\\Shaders\\PBRShader.vert");
			shader->addShader(ShaderType::FRAGMENTSHADER, ".\\res\\Shaders\\PBRShader.frag");
			shader->bind();
			shader->getUniforms();
			
			//3. Set up model and load geometry data
			std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(&m->VERTS, &m->NORMS, &m->TEXVERTS, &m->IND);
			//Model transformations
			mesh->scale(1.0f);
			//Set node transform
			model->setTransform(std::make_shared<Transform>(mesh->modelTransform));

			//4. Set up material
			std::shared_ptr<PBRMaterial> material = std::make_shared<PBRMaterial>(shader);
			
			//Get paths to textures
			std::string base = ".\\res\\Textures\\helmet\\";
			std::string albedo;
			std::string metallic;
			std::string roughness;
			std::string normal;

			for (auto mat : loader->materials) {
				if (mat->NAME.compare(m->MAT) == 0) {
					albedo = base + mat->DIFFUSE;
					metallic = base + mat->AMBIENT;
					roughness = base + mat->SPECULAR;
					normal = base + mat->NORMAL;
					LOG_INFO(albedo);
					LOG_INFO(metallic);
					LOG_INFO(roughness);
					LOG_INFO(normal);
					break;
				}
			}

			material->addTextures(albedo, roughness, metallic, normal);
			mesh->setMaterial(material);

			//5. Add MeshRenderer component to node to enable rendering
			std::shared_ptr<CMeshRenderer> meshRenderer = std::make_shared<CMeshRenderer>(mesh);
			model->addComponent(meshRenderer);
			
			//6. Add node to scene graph 
			m_root->addChild(model);
		}
		
		delete loader;

		/* ===== Add Lights ===== */
		//Create first light node
		std::shared_ptr<SceneNode> pointLight = std::make_shared<SceneNode>("Point Light 1");
		
		//Create PointLight component
		std::shared_ptr<PointLight> lightComponent = std::make_shared<PointLight>(Vec3f(1.0f, 1.0f, 1.0f), 3.0f, Vec3f(0.0f, 200.0f, 10.0f), 1.0f, 0.007f, 0.0002f);
		
		//Set nodes' transform
		pointLight->setTransform(std::make_shared<Transform>(lightComponent->getTransform()));
		
		//Add component to node
		pointLight->addComponent(lightComponent);

		//Register light, to be added during rendering
		GraphicsContext::getInstance().registerLight(lightComponent);
		
		//Add node to scene graph
		m_root->addChild(pointLight);

		/* Create second light node */
		std::shared_ptr<SceneNode> pointLight2 = std::make_shared<SceneNode>("Point Light 2");
		
		std::shared_ptr<PointLight> lightComponent2 = std::make_shared<PointLight>(Vec3f(1.0f, 1.0f, 1.0f), 10.0f, Vec3f(-5.0f, 5.0f, 5.0f), 1.0f, 0.7f, 1.8f);
		
		pointLight2->setTransform(std::make_shared<Transform>(lightComponent2->getTransform()));
		
		pointLight2->addComponent(lightComponent2);
		
		GraphicsContext::getInstance().registerLight(lightComponent2);
		
		m_root->addChild(pointLight2);
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

	//TODO: Check items that need to be destroyed
	~MediumScene() {}
};