#include "CubePlayer.h"
#include "Cube.h"
#include "Shader.h"

std::vector<PlayerCollision> CubePlayer::players = {};

CubePlayer::CubePlayer(float width, float height, float depth, float r, float g, float b, float posX, float posY, float posZ) : position(glm::vec3(posX, posY, posZ)), scale(glm::vec3(1.0f, 1.0f, 1.0f))
{
    GeneratePlayer(width, height, depth, r, g, b);
    setupMesh();
    rotationAngle = 0;
    rotationAxis = glm::vec3(1.0f, 1.0f, 1.0f);
    updateModelMatrix();
    players.emplace_back(glm::vec3(position), glm::vec3(scale));
}

CubePlayer::~CubePlayer()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void CubePlayer::Draw(Shader& shader)
{
    shader.use();
    shader.setMat4("model", modelMatrix);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, vertices.size());
    glBindVertexArray(0);


}

void CubePlayer::PlayerInput(GLFWwindow* window, float deltaTime)
{

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        UpdatePosition(glm::vec3(0.0f, 0.0f, -10.f) * deltaTime);
        /*rotationAngle-=1; */
        updateModelMatrix();
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        UpdatePosition(glm::vec3(0.0f, 0.0f, 10.f) * deltaTime);
        /*rotationAngle+=1;*/
        updateModelMatrix();
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        UpdatePosition(glm::vec3(-10.f, 0.0f, 0.0f) * deltaTime);
        //rotationAngle+=1;
        updateModelMatrix();
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        UpdatePosition(glm::vec3(10.f, 0.0f, 0.0f) * deltaTime);
        //rotationAngle-=1;
        updateModelMatrix();
    }

    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        if (speedControl <= 0.1f) {
            return;
        }
        else {
            speedControl -= 0.1f;
        }
    }

    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    {
        if (speedControl >= 1.5f) {
            return;
        }
        else {
            speedControl += 0.1f;
        }
        
    }
}

void CubePlayer::UpdatePosition(glm::vec3 direction)
{
    position = position + direction * speedControl;
    updateModelMatrix();
}

void CubePlayer::SetRotation(float angle, glm::vec3 axis)
{
    rotationAngle = angle;
    rotationAxis = axis;
    updateModelMatrix();
}

bool CubePlayer::collisionDetection(const PlayerCollision& cube1, const PlayerCollision& cube2)
{
    glm::vec3 minA = cube1.position - cube1.size * 0.5f;
    glm::vec3 maxA = cube1.position + cube1.size * 0.5f;

    glm::vec3 minB = cube2.position - cube2.size * 0.5f;
    glm::vec3 maxB = cube2.position + cube2.size * 0.5f;

    bool colX = minA.x <= maxB.x && maxA.x >= minB.x;
    bool colY = minA.y <= maxB.y && maxA.y >= minB.y;
    bool colZ = minA.z <= maxB.z && maxA.z >= minB.z;

    return colX && colY && colZ;
}

void CubePlayer::GeneratePlayer(float w, float h, float d, float r, float g, float b) {
    // Define the half extents of the cube
    float hw = w * 0.5f;
    float hh = h * 0.5f;
    float hd = d * 0.5f;

    glm::vec3 color = { r, g, b };

    glm::vec3 baryCoords[] = {

        {1.f, 0.f, 0.f},
        {0.f, 1.f, 0.f},
        {0.f, 0.f, 1.f}
    };

    vertices = {
        // Front face
        {{ hw, -hh, hd}, color, baryCoords[0]}, // Bottom Right
        {{-hw, -hh, hd}, color, baryCoords[1]}, // Bottom Left
        {{ hw, hh, hd}, color, baryCoords[2]}, // Top Right

        {{ hw, hh, hd}, color, baryCoords[0]},//Top Right
        {{-hw, hh, hd}, color, baryCoords[1]},// Top Left
        {{-hw, -hh, hd}, color, baryCoords[2]}, //Bottom Left

        // Back face
        {{-hw, -hh, -hd}, color, baryCoords[0]}, // Bottom Left
        {{ hw, -hh, -hd}, color, baryCoords[1]}, // Bottom Right
        {{ hw,  hh, -hd}, color, baryCoords[2]}, // Top Right

        {{ hw,  hh, -hd}, color, baryCoords[0]}, // Top Right
        {{-hw,  hh, -hd}, color, baryCoords[1]}, // Top Left
        {{-hw, -hh, -hd}, color, baryCoords[2]}, // Bottom Left

        // Left face
        {{-hw, -hh, -hd}, color, baryCoords[0]}, // Bottom Left
        {{-hw, -hh,  hd}, color, baryCoords[1]}, // Bottom Right
        {{-hw,  hh,  hd}, color, baryCoords[2]}, // Top Right

        {{-hw,  hh,  hd}, color, baryCoords[0]}, // Top Right
        {{-hw,  hh, -hd}, color, baryCoords[1]}, // Top Left
        {{-hw, -hh, -hd}, color, baryCoords[2]}, // Bottom Left
        // Right face

        {{ hw, -hh,  hd}, color, baryCoords[0]}, // Bottom Left
        {{ hw, -hh, -hd}, color, baryCoords[1]}, // Bottom Right
        {{ hw,  hh, -hd}, color, baryCoords[2]}, // Top Right

        {{ hw,  hh, -hd}, color, baryCoords[0]}, // Top Right
        {{ hw,  hh,  hd}, color, baryCoords[1]}, // Top Left
        {{ hw, -hh,  hd}, color, baryCoords[2]}, // Bottom Left

        // Top face
        {{-hw,  hh,  hd}, color, baryCoords[0]}, // Bottom Left
        {{ hw,  hh,  hd}, color, baryCoords[1]}, // Bottom Right
        {{ hw,  hh, -hd}, color, baryCoords[2]}, // Top Right

        {{ hw,  hh, -hd}, color, baryCoords[0]}, // Top Right
        {{-hw,  hh, -hd}, color, baryCoords[1]}, // Top Left
        {{-hw,  hh,  hd}, color, baryCoords[2]}, // Bottom Left

        // Bottom face
        {{-hw, -hh, -hd}, color, baryCoords[0]}, // Bottom Left
        {{ hw, -hh, -hd}, color, baryCoords[1]}, // Bottom Right
        {{ hw, -hh,  hd}, color, baryCoords[2]}, // Top Right

        {{ hw, -hh,  hd}, color, baryCoords[0]}, // Top Right
        {{-hw, -hh,  hd}, color, baryCoords[1]}, // Top Left
        {{-hw, -hh, -hd}, color, baryCoords[2]}, // Bottom Left
    };

}


void CubePlayer::setupMesh()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Position));
    glEnableVertexAttribArray(0);

    // Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Color));
    glEnableVertexAttribArray(1);

    // Barycentric coord
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Barycentric));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void CubePlayer::updateModelMatrix()
{
    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, position);
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotationAngle), rotationAxis);
    modelMatrix = glm::scale(modelMatrix, scale);
}

