#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec3 aBaryCoords;

out vec3 ourColor; // Pass color to the fragment shader
out vec3 BaryCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    ourColor = aColor; // Set the output color to the input color from the VBO
    BaryCoord = aBaryCoords;
    
}