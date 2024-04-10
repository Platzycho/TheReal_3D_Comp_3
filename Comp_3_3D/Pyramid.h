#pragma once
#include "Includes.h"

class Pyramid
{
public:
	Pyramid(float baseSide, float height, float r, float g, float b, float posX, float posY, float posZ);
	~Pyramid();
	void Draw(Shader& shader);
	void GeneratePyramid(float baseSide, float height, float r, float g, float b);
	void SetupMesh();

	static std::vector<Collision> pyroMids;

private:
	unsigned int VAO, VBO;
	std::vector<Vertex> vertices;
	glm::vec3 Position;
	glm::vec3 Scale;
	glm::mat4 modelMatrix;
	glm::vec3 rotationAxis;

	float rotationAngle;

	void updateModelMatrix();

};

