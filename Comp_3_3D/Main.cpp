#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Shader.h"
#include "Camera.h"
#include "Cube.h"
#include "Pyramid.h"
#include "Sphere.h"
#include "CubePlayer.h"
#include "NPCPath.h"
#include "TexMex.h"
#include "Object.h"
#include <iostream>
#include <algorithm>
#include <cmath>


const double M_PI = 3.141592653589793;

struct CallbackData {
    Shader* myShader;
    Camera* myCamera;
};

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void processInput(GLFWwindow* window);

void mouse_callback(GLFWwindow* window, double xpos, double ypos);

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

Camera camera(glm::vec3(-0.0589525f, 11.9224f, 12.7897f));

glm::vec3 camPosition = glm::vec3(-0.0589525f, 11.9224f, 12.7897f);

glm::vec3 cameraDirection = glm::vec3(0.0f, -1.0f, -1.0f);

glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

float deltaTime = 0.0f;
float lastFrame = 0.0f;
float rotationAngle = 0.0f;

int main() {
	
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to init GLAD" << std::endl;
		return -1;
	}

	glViewport(0, 0, 800, 600);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
   
    TexMex myTex("C:/Users/Nesli/Documents/3D Prog/Compulsory_3/3d_Comp_3/Comp_3_3D/Assets/PZ.jpg", "diffuse", false);

    TexMex myTexTwo("C:/Users/Nesli/Documents/3D Prog/Compulsory_3/3d_Comp_3/Comp_3_3D/Assets/images.jpg", "diffuse", false);

    Shader myShader("shader.vs", "shader.fs");

    Shader myOtherShader("shader.vs", "shader.fs");

    Object surface(0, 20.f, 0.5f, 20.f, 0.5f, 0.5f, 0.5f, 0.f, 0.f, 0.f);

    CubePlayer player(1.f, 1.f, 1.f, 0.5f, 0.f, 1.f, 3.f, 1.f, 0.f);

    Object pyraOne(1, 2.f, 1.f, 2.f, 0.6f, 0.3f, 0.0f, 5.f, 0.5f, 1.f);

    Object pyraTwo(1, 2.f, 2.f, -1.f, 0.6f, 0.3f, 0.0f, 0.f, 3.5f, 1.f);

    Object spherOne(2, 1.f, 2.f, 2.f, 1.f, 0.3f, 0.3f, 0.f, 0.f, 0.f);

    NPCPath path(0.0f, 20.0f, 0.01f, 7.0f, 1.f, 0.3f, 0.5f, 0.f, 0.f, 0.f);

    cameraDirection = glm::normalize(cameraDirection);

	glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)800 / (float)600, 0.1f, 100.0f);
    glm::mat4 view = glm::lookAt(camPosition, camPosition + cameraDirection, up);

    CallbackData callbackData;
    callbackData.myShader = &myShader;
    callbackData.myCamera = &camera;

    glfwSetScrollCallback(window, scroll_callback);

    glfwSetWindowUserPointer(window, &callbackData);

    glfwSetCursorPosCallback(window, mouse_callback);

    glEnable(GL_DEPTH_TEST);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

   

	while(!glfwWindowShouldClose(window))
	{
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		processInput(window);
        player.PlayerInput(window, deltaTime);

       // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)800 / (float)600, 0.1f, 100.0f);
        glm::vec3 lightDir = glm::normalize(glm::vec3(-1.0f, -1.0f, -1.0f));

        // Create transformations and pass them to the shaders
        glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first

        // Rotate the cube over time  
        rotationAngle += 0.0f;
        
        player.SetRotation(rotationAngle, glm::vec3(0.5f, 0.3f, 0.6f));
        myShader.use();
        myTex.UseTexture(GL_TEXTURE0);
        myShader.setInt("textureSampler", 0);
        myShader.setFloat("time", currentFrame);
        myShader.setMat4("projection", projection);
        myShader.setMat4("view", view);
        myShader.setMat4("model", model);

        myOtherShader.use();
        myTexTwo.UseTexture(GL_TEXTURE1);
        myOtherShader.setInt("textureSampler", 1);
        myOtherShader.setFloat("time", currentFrame);
        myOtherShader.setMat4("projection", projection);
        myOtherShader.setMat4("view", view);
        myOtherShader.setMat4("model", model);

        spherOne.UpdatePosition(path, deltaTime);
        // Render the cube
       
        spherOne.DrawElement(myShader);
        path.Draw(myShader);
        player.Draw(myOtherShader);
        pyraOne.DrawArray(myOtherShader);
        pyraTwo.DrawArray(myShader);
        
        surface.DrawArray(myShader);
        
        
        // Swap buffers and poll IO events
        glfwSwapBuffers(window);
        glfwPollEvents();
		
	}

    surface.~Object();
    player.~CubePlayer();
    pyraOne.~Object();
    pyraTwo.~Object();
    spherOne.~Object();
    path.~NPCPath();
	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
    
    CallbackData* data = static_cast<CallbackData*>(glfwGetWindowUserPointer(window));

    // Now you can use data->myShader and data->camera as needed
     if (data != nullptr && data->myShader != nullptr) {
        glm::mat4 projection = glm::perspective(glm::radians(data->myCamera->Zoom), (float)width / (float)height, 0.1f, 100.0f);
        data->myShader->use(); // Make sure the shader is being used before setting uniforms
        glUniformMatrix4fv(glGetUniformLocation(data->myShader->ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    }
}

void processInput(GLFWwindow* window)
{	

	if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    static float lastX = 800.0f / 2.0;
    static float lastY = 600.0f / 2.0;
    static bool firstMouse = true;

    CallbackData* data = static_cast<CallbackData*>(glfwGetWindowUserPointer(window));
    if(firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    if (data && data->myCamera) {
        data->myCamera->ProcessMouseMovement(xoffset, yoffset);
    }
    

}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    CallbackData* data = static_cast<CallbackData*>(glfwGetWindowUserPointer(window));
    if(data && data->myCamera) {
        data->myCamera->ProcessMouseScroll(yoffset);
    }
}
