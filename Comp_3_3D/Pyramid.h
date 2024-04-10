#pragma once

#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct PyraVertex {
	glm::vec3 Position, Barycentric;
	glm::vec2 TexMex;

};

struct PyraCollision {
	glm::vec3 Position, Scale;

	PyraCollision(glm::vec3 pos, glm::vec3 sz) : Position(pos), Scale(sz) {}
};

class Pyramid
{
public:
	Pyramid(float baseSide, float height, float r, float g, float b, float posX, float posY, float posZ);
	~Pyramid();
	void Draw(Shader& shader);
	void GeneratePyramid(float baseSide, float height, float r, float g, float b);
	void SetupMesh();

	static std::vector<PyraCollision> pyroMids;

private:
	unsigned int VAO, VBO;
	std::vector<PyraVertex> vertices;
	glm::vec3 Position;
	glm::vec3 Scale;
	glm::mat4 modelMatrix;
	glm::vec3 rotationAxis;

	float rotationAngle;

	void updateModelMatrix();

};

