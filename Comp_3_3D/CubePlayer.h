#pragma once
#include "Includes.h"

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
	//bool CheckAABBCollision(const AABB& a, const AABB& b);

private:
	unsigned int VAO, VBO, EBO;
	glm::vec3 position;
	glm::vec3 newPosition;
	glm::vec3 scale;
	glm::mat4 modelMatrix;
	glm::vec3 rotationAxis;
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	float speedControl = 0.5f;

	float rotationAngle;

	void setupMesh();
	void updateModelMatrix();
};


