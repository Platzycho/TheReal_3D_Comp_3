#pragma once

#include <Camera.h>
#include <SceneGraph.h>
#include <memory>
#include <Controller.h>
#include <Mesh.h>
#include <ActorController.h>
#include <CameraController.h>
#include <Lights/PointLight.h>
#include <Lights/DirectionalLight.h>

class Scene
{
public:
	Scene(const std::stringbuf& name);
	virtual ~Scene() = default;
	
	Scene(const Scene&) = delete;
	Scene& operator = (const Scene&) = delete;
	Scene(Scene&&) = delete;
	Scene& operator = (Scene&&) = delete;

	virtual void LoadContent();
	virtual void UnloadContent();

	void UpdateInputController(float dt);
	void UpdateSceneGraph(Actor* actor, float dt, Transform globalTransform = Transform{});

	void Update(float dt);
	void HandleCollision();

	void RenderSceneGraph(Actor* actor, float dt, Transform globalTransform = Transform{});
	void BindDirectionalLight();
	void BindPointLights();
	void BindCamera();
	void RenderUI();
	void Render(float dt);

	void FramebufferSizeCallback(class Window* window, int width, int height);
	void MouseMoveCallback(class Window* window, double xpos, double ypos);
	void MouseButtonCallback(class Window* window, int button, int action, int mods);
	void MouseScrollCallback(class Window* window, double xoffset, double yoffset);
	void KeyCallback(class Window* window, int key, int scancode, int action, int mods);

	void SetController(const std::shared_ptr::ptr<IController>& controller) { mActiveController = controller; }
	std::shared_ptr<IController> GetController() const { return mActiveController; }

	SceneGraph mSceneGraph;
	CameraActor mSceneCamera{ "SceneCamera" };

private:
	MeshActor* mCubeActor0{ nullptr };
	StaticMeshActor* mStaticActor0{ nullptr };

	class Skybox* mSkybox{ nullptr };
	class Shader* mShader{ nullptr };

	PointLightActor* mPointLightActor{ nullptr };
	DirectionalLightActor* mDirectionalLightActor{ nullptr };

	std::shared_ptr<ActorController> mActorController;
	std::shared_ptr<CameraController> mCameraController;
protected:
	std::shared_ptr<IController> mActiveController{ nullptr };
};

