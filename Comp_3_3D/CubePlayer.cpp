#include "CubePlayer.h"
#include "Cube.h"
#include "Shader.h"

CubePlayer::CubePlayer(float width, float height, float depth, float r, float g, float b, float posX, float posY, float posZ) : position(glm::vec3(posX, posY, posZ)), scale(glm::vec3(1.0f, 1.0f, 1.0f))
{
    GeneratePlayer(width, height, depth, r, g, b);
    setupMesh();
    rotationAngle = 0;
    rotationAxis = glm::vec3(1.0f, 1.0f, 1.0f);
    updateModelMatrix();
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

    //glm::vec3 tentativePosition = position + direction * speedControl;
    //AABB playerBox = CalculateAABB({ tentativePosition, scale }); // Assuming scale can be used to represent size here

    //// Assume cubes and pyramids are stored in std::vector<Cube> and std::vector<Pyramid>
    //bool collisionDetected = false;
    //for (const auto& cube : Cube::cubes) { // Assuming cubes is accessible here
    //    if (CheckAABBCollision(playerBox, CalculateAABB(cube.GetCollisionData()))) {
    //        collisionDetected = true;
    //        break;
    //    }
    //}

    //// Repeat for pyramids if necessary

    //if (!collisionDetected) {
    //    position = tentativePosition; // Update position only if no collision
    //    updateModelMatrix(); // Update the model matrix with the new position
    //}
}

void CubePlayer::SetRotation(float angle, glm::vec3 axis)
{
    rotationAngle = angle;
    rotationAxis = axis;
    updateModelMatrix();
}

bool CubePlayer::collisionDetection(const Collision& cube1, const Collision& cube2)
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

//bool CubePlayer::CheckAABBCollision(const AABB& a, const AABB& b)
//{
//    return false;
//}

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
    {{ hw, -hh, hd}, baryCoords[0], {1.0f, 0.0f}}, // Bottom Right
    {{-hw, -hh, hd}, baryCoords[1], {0.0f, 0.0f}}, // Bottom Left
    {{ hw, hh, hd}, baryCoords[2] , {1.0f, 1.0f}}, // Top Right
    
    {{ hw, hh, hd}, baryCoords[0] , {1.0f, 1.0f}},//Top Right
    {{-hw, hh, hd}, baryCoords[1] , {0.0f, 1.0f}},// Top Left
    {{-hw, -hh, hd}, baryCoords[2], {0.0f, 0.0f}}, //Bottom Left
    
    // Back face
    {{-hw, -hh, -hd}, baryCoords[0], {1.0f, 0.0f}}, // Bottom Left
    {{ hw, -hh, -hd}, baryCoords[1], {0.0f, 0.0f}}, // Bottom Right
    {{ hw,  hh, -hd}, baryCoords[2], {1.0f, 1.0f}}, // Top Right
    
    {{ hw,  hh, -hd}, baryCoords[0], {1.0f, 1.0f}}, // Top Right
    {{-hw,  hh, -hd}, baryCoords[1], {0.0f, 1.0f}}, // Top Left
    {{-hw, -hh, -hd}, baryCoords[2], {0.0f, 0.0f}}, // Bottom Left
    
    // Left face     
    {{-hw, -hh, -hd}, baryCoords[0], {1.0f, 0.0f}}, // Bottom Left
    {{-hw, -hh,  hd}, baryCoords[1], {0.0f, 0.0f}}, // Bottom Right
    {{-hw,  hh,  hd}, baryCoords[2], {1.0f, 1.0f}}, // Top Right
    
    {{-hw,  hh,  hd}, baryCoords[0], {1.0f, 1.0f}}, // Top Right
    {{-hw,  hh, -hd}, baryCoords[1], {0.0f, 1.0f}}, // Top Left
    {{-hw, -hh, -hd}, baryCoords[2], {0.0f, 0.0f}}, // Bottom Left
    // Right face    
    
    {{ hw, -hh,  hd}, baryCoords[0], {1.0f, 0.0f}}, // Bottom Left
    {{ hw, -hh, -hd}, baryCoords[1], {0.0f, 0.0f}}, // Bottom Right
    {{ hw,  hh, -hd}, baryCoords[2], {1.0f, 1.0f}}, // Top Right
    
    {{ hw,  hh, -hd}, baryCoords[0], {1.0f, 1.0f}}, // Top Right
    {{ hw,  hh,  hd}, baryCoords[1], {0.0f, 1.0f}}, // Top Left
    {{ hw, -hh,  hd}, baryCoords[2], {0.0f, 0.0f}}, // Bottom Left
    
    // Top face      
    {{-hw,  hh,  hd}, baryCoords[0], {1.0f, 0.0f}}, // Bottom Left
    {{ hw,  hh,  hd}, baryCoords[1], {0.0f, 0.0f}}, // Bottom Right
    {{ hw,  hh, -hd}, baryCoords[2], {1.0f, 1.0f}}, // Top Right
    
    {{ hw,  hh, -hd}, baryCoords[0], {1.0f, 1.0f}}, // Top Right
    {{-hw,  hh, -hd}, baryCoords[1], {0.0f, 1.0f}}, // Top Left
    {{-hw,  hh,  hd}, baryCoords[2], {0.0f, 0.0f}}, // Bottom Left
    
    // Bottom face   
    {{-hw, -hh, -hd}, baryCoords[0], {1.0f, 0.0f}}, // Bottom Left
    {{ hw, -hh, -hd}, baryCoords[1], {0.0f, 0.0f}}, // Bottom Right
    {{ hw, -hh,  hd}, baryCoords[2], {1.0f, 1.0f}}, // Top Right
    
    {{ hw, -hh,  hd}, baryCoords[0], {1.0f, 1.0f}}, // Top Right
    {{-hw, -hh,  hd}, baryCoords[1], {0.0f, 1.0f}}, // Top Left
    {{-hw, -hh, -hd}, baryCoords[2], {0.0f, 0.0f}}, // Bottom Left
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
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Barycentric));
    glEnableVertexAttribArray(1);

    // Barycentric coord
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexMex));
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

