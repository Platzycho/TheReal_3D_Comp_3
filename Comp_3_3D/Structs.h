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

struct AABB{
	glm::vec3 min;
	glm::vec3 max;
};

inline AABB CalculateAABB(const Collision& object) {
	AABB box;
	box.min = object.position - object.size * 0.5f;
	box.max = object.position + object.size * 0.5f;
	return box;
}

inline bool CheckAABBCollision(const AABB& a, const AABB& b) {
	return (a.min.x <= b.max.x && a.max.x >= b.min.x) &&
			(a.min.y <= b.max.y && a.max.y >= b.min.y) &&
			(a.min.z <= b.max.z && a.max.z >= b.min.z);
}