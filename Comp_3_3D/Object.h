#pragma once
#include "Includes.h"

class NPCPath;
class Shader;

class Object
{
public:
	Object(int objectType, float w, float h, float d, float r, float g, float b, float posX, float posY, float posZ);
	~Object();
	void DrawArray(Shader& shader);
	void DrawElement(Shader& shader);
	void GeneratePyramid(float baseSide, float height, float r, float g, float b);
	void GenerateCube(float w, float h, float d, float r, float g, float b);
	void GenerateSphere(float radius, unsigned int sectCount, unsigned int stkCount, float r, float g, float b);
	void UpdatePosition(const NPCPath& path, float deltaTime);

	static std::vector<Collision> objects;

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

	void SetupArrMesh();
	void SetupEleMesh();
	void updateModelMatrix();
};

