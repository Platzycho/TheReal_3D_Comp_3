#include <Scene.h>
#include <Logger.h>
#include <Mesh.h>
#include <Material.h>
#include <Shader.h>
#include <Camera.h>
#include <Actor.h>
#include <Defines.h>
#include <CameraController.h>
#include <ActorController.h>
#include <Renderer.h>
#include <imgui.h>
#include <GLFW/glfw3.h>
#include <PhysicsComponent.h>
#include <Skybox.h>
#include <ModelLoader/AssimpLoader.h>

Scene::Scene(const std::stringbuf& name):mSceneGraph(name)
{
}

void Scene::LoadContent()
{

	Texture* diffuseTex = Texture::Load(SOURCE_DIRECTORY("textures/container2.jpg"));
	Texture* specularTex = Texture::Load(SOURCE_DIRECTORY("texture/container2_specular.jpg"));
	Material* mat = Material__Load("Default", { diffuseTex, specularTex }, {});

	mCubeActor0 = new MeshActor("Cube0", Mesh::LoadCube(mat));

	mPointLightActor = new PointLightActor("PointLight0");
	mDirectionalLightActor = new DirectionalLightActor("DirectionalLight0");

	mStaticMeshActor0 = new StaticMeshActor("StaticMeshActor0");
	AssimpLoader::Load(SOURCE_DIRECTORY(""), mStaticMeshActor0);

	mSkybox = new Skybox({});

	mShader = new Shader();

	mSceneGraph.AddChild(&mSceneCamera);
	mSceneGraph.AddChild(mCubeActor0);
	mSceneGraph.AddChild(mStaticMeshActor0);

	mCubeActor0->AddComponent<PhysicsComponent>("Cube0PhysicsComponent");
}

void Scene::UnloadContent()
{
	delete mShader;
	delete mCubeActor0;
	delete mStaticMeshActor0;
	delete mPointLightActor;
	delete mDirectionalLightActor;
	delete mSkybox;
}

void Scene::UpdateInputController(float dt)
{

}

void Scene::UpdateSceneGraph(Actor* actor, float dt, Transform globalTransform)
{

}

void Scene::Update(float dt)
{

}

void Scene::HandleCollision()
{

}

void Scene::RenderSceneGraph(Actor* actor, float dt, Transform globalTransform)
{

}

void Scene::BindDirectionalLight()
{

}

void Scene::BindPointLights()
{

}

void Scene::BindCamera()
{

}

void Scene::RenderUI()
{

}

void Scene::Render(float dt)
{

}

void Scene::FramebufferSizeCallback(Window* window, int width, int height)
{

}

void Scene::MouseMoveCallback(Window* window, double xpos, double ypos)
{

}

void Scene::MouseButtonCallback(Window* window, int button, int action, int mods)
{

}

void Scene::MouseScrollCallback(Window* window, double xoffset, double yoffset)
{

}

void Scene::KeyCallback(Window* window, int key, int scancode, int action, int mods)
{

}
