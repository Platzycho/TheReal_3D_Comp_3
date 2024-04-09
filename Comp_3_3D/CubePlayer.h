#pragma once

#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct PlayerVertex {
	glm::vec3 Position, Color, Barycentric;
};

struct PlayerCollision {
	glm::vec3 position, size;

	PlayerCollision(glm::vec3 pos, glm::vec3 sz) : position(pos), size(sz) {}
};

class Shader;

class CubePlayer
{
public:
	CubePlayer(float width, float height, float depth,
		float r, float g, float b,
		float posX, float posY, float posZ);
	~CubePlayer();
	void Draw(Shader& shader);
	void PlayerInput(GLFWwindow* window, float deltaTime);
	void UpdatePosition(glm::vec3 direction);
	void SetRotation(float angle, glm::vec3 axis);
	void GeneratePlayer(float w, float h, float d, float r, float g, float b);
	static bool collisionDetection(const PlayerCollision& cube1, const PlayerCollision& cube2);

	static std::vector<PlayerCollision> players;


private:


	unsigned int VAO, VBO, EBO;
	glm::vec3 position;
	glm::vec3 newPosition;
	glm::vec3 scale;
	glm::mat4 modelMatrix;
	glm::vec3 rotationAxis;
	std::vector<PlayerVertex> vertices;
	std::vector<unsigned int> indices;

	float speedControl = 0.5f;

	float rotationAngle;

	void setupMesh();
	void updateModelMatrix();
};


