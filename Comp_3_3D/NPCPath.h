#pragma once

#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct PathVertex {
	glm::vec3 Position, Color, Barycentric;

};

class Shader;

class NPCPath
{
public:
	NPCPath(float t, float tMax, float step, float radius, float r, float g, float b, float posX, float posY, float posZ);
	~NPCPath();
	void Draw(Shader& shader);
	void GeneratePath(float t, float tMax, float step, float radius);

	std::vector<glm::vec2> curvePoints;
	
	glm::vec3 position;

private:
	
	unsigned int VBO, VAO;

	glm::vec3 scale;
	glm::mat4 modelMatrix;
	glm::vec3 rotationAxis;
	std::vector<PathVertex> vertices;

	float rotationAngle;

	void SetupMesh();
	void updateModelMatrix();

};

