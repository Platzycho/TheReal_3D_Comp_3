#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec3 BaryCoord;

uniform float time; 

void main() {
    // Simple movement effect based on barycentric coordinates and time
    float movement = sin(time + BaryCoord.x * 2.0 + BaryCoord.y * 2.0 + BaryCoord.z * 2.0) * 0.5 + 0.5;
    FragColor = vec4(ourColor * movement, 1.0);

    //FragColor = vec4(BaryCoord, 1.0);

}