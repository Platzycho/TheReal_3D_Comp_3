#pragma once
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct Vertex {
	glm::vec3 Position, Barycentric;
	glm::vec2 TexMex;
};

struct Collision {
	glm::vec3 position, size;

	Collision(glm::vec3 pos, glm::vec3 sz) : position(pos), size(sz) {}
};