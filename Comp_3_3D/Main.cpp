#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Shader.h"
#include "Camera.h"
#include "Cube.h"
#include <iostream>
#include <algorithm>
#include <cmath>

struct CallbackData {
    Shader* myShader;
    Camera* myCamera;
};

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void processInput(GLFWwindow* window);

void mouse_callback(GLFWwindow* window, double xpos, double ypos);

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));



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

    
    Shader myShader("shader.vs", "shader.fs");

    Cube cube(2.f, 2.f, 2.f, 0.f, 0.f, 1.f, 1.f, 1.f, 0.f);

    Cube cubeTwo(2.f, 2.f, 2.f, 0.f, 1.f, 0.f, 5.f, 1.f, 0.f);

    Cube cubeThree(2.f, 2.f, 2.f, 1.f, 0.f, 0.f, 3.f, 3.f, 0.f);

	glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)800 / (float)600, 0.1f, 100.0f);
	glm::mat4 view = camera.GetViewMatrix();

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

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		myShader.use();
        
	    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)800 / (float)600, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();/*
        glUniformMatrix4fv(glGetUniformLocation(myShader.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));*/
        glm::vec3 lightDir = glm::normalize(glm::vec3(-1.0f, -1.0f, -1.0f));

        // Create transformations and pass them to the shaders
        glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first

        // Rotate the cube over time  
        rotationAngle += 0.1f;
        cube.SetRotation(rotationAngle, glm::vec3(0.5f, 0.3f, 0.6f));
        cubeTwo.SetRotation(rotationAngle, glm::vec3(-0.5f, 0.3f,- 0.6f));
        cubeThree.SetRotation(rotationAngle, glm::vec3(0.0f, -1.0f, 0.6f));
        glUniformMatrix4fv(glGetUniformLocation(myShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));

        myShader.setMat4("projection", projection);
        myShader.setMat4("view", view);

        // Render the cube
        cube.Draw(myShader);
        cubeTwo.Draw(myShader);
        cubeThree.Draw(myShader);


        // Swap buffers and poll IO events
        glfwSwapBuffers(window);
        glfwPollEvents();
		
	}

    cube.CleanUp();
    cubeTwo.CleanUp();
    cubeThree.CleanUp();
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
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        camera.ProcessKeyboard(UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        camera.ProcessKeyboard(DOWN, deltaTime);

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
