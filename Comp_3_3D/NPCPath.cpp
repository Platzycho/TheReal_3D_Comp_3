#include "NPCPath.h"
#include "Shader.h"
#include <iostream>

NPCPath::NPCPath(float t, float tMax, float step, float radius, float r, float g, float b, float posX, float posY, float posZ) : position(glm::vec3(posX, posY, posZ)), scale(glm::vec3(1.0f, 1.0f, 1.0f))
{
	GeneratePath(t, tMax, step, radius); 
	for (const auto& point : curvePoints) {
		// Assuming a default color (e.g., white) and barycentric (you can adjust these as needed)
		vertices.push_back({ glm::vec3(point, posZ), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f) });
	}
	SetupMesh();
	rotationAngle = 0;
	rotationAxis = glm::vec3(1.0f, 1.0f, 1.0f);
	updateModelMatrix();
}

NPCPath::~NPCPath()
{

}

void NPCPath::Draw(Shader& shader)
{
	shader.use();
	shader.setMat4("model", modelMatrix); 
	glBindVertexArray(VAO);
	glDrawArrays(GL_POINTS, 0, vertices.size()); 
	glBindVertexArray(0);
}

void NPCPath::GeneratePath(float t, float tMax, float step, float radius)
{
	curvePoints.clear();
	for (t = 0.0f; t <= tMax; t += step) {
		float x = radius * cos(t);
		float y = radius * sin(t);
		curvePoints.push_back(glm::vec2(x, y));
	}
}

void NPCPath::SetRotation(float angle, glm::vec3 axis)
{
	rotationAngle = angle;
	rotationAxis = axis;
	updateModelMatrix();
}

void NPCPath::SetupMesh()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Correctly calculate the size based on the entire vertex structure
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Position));
	glEnableVertexAttribArray(0);

	// Color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Barycentric));
	glEnableVertexAttribArray(1);

	// Barycentric attribute 
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexMex));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void NPCPath::updateModelMatrix()
{
	modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::translate(modelMatrix, position);
	modelMatrix = glm::rotate(modelMatrix, glm::radians(rotationAngle), rotationAxis);
	modelMatrix = glm::scale(modelMatrix, scale);
}

