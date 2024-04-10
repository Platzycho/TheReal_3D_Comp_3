#pragma once
#include "Includes.h"
#include "NPCPath.h"


class NPCPath;

class Shader;

class Sphere
{
public:
	Sphere(float radius, unsigned int sectCount, unsigned int stkCount, float r, float g, float b, float posX, float posY, float posZ);
	~Sphere();
	void Draw(Shader& shader);
	void UpdatePosition(const NPCPath& path, float deltaTime);
	void GenerateSphere(float radius, unsigned int sectCount, unsigned int stkCount, float r, float g, float b);
	void SetupMesh();

	static std::vector<Collision> spheres;

private:
	unsigned int VAO, VBO, EBO;
	std::vector<Vertex> vertices;
	glm::vec3 Position;
	glm::vec3 Scale;
	glm::mat4 modelMatrix;
	glm::vec3 rotationAxis;
	std::vector<unsigned int> indices;

	float rotationAngle;

	float pathParameter = 0.0f;
	bool movingForward = true;
	float speed = 0.1f;

	void updateModelMatrix();

};


