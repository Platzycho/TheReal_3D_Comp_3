#pragma once

#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Structs.h"

class Shader;

class Cube
{
public:
	Cube(float width, float height, float depth,
		float r, float g, float b,
		float posX, float posY, float posZ);
	~Cube();
	void Draw(Shader& shader);
	void SetRotation(float angle, glm::vec3 axis);
	void GenerateCube(float w, float h, float d, float r, float g, float b);
	static bool collisionDetection(const Collision& cube1, const Collision& cube2);

	static std::vector<Collision> cubes;


private:


	unsigned int VAO, VBO, EBO;
	glm::vec3 position;
	glm::vec3 scale;
	glm::mat4 modelMatrix;
	glm::vec3 rotationAxis;
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	float rotationAngle;

	void setupMesh();
	void updateModelMatrix();
};
