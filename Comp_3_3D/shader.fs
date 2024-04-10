#version 330 core
out vec4 FragColor;

in vec2 TexCoord; 
in vec3 BaryCoord; 

uniform sampler2D textureSampler; 
uniform float time; 

void main() {
    vec4 texColor = texture(textureSampler, TexCoord); 
    // Apply the simple movement effect based on barycentric coordinates and time
    float movement = sin(time + BaryCoord.x * 2.0 + BaryCoord.y * 2.0 + BaryCoord.z * 2.0) * 0.5 + 0.5;
    FragColor = vec4(texColor.rgb * movement, texColor.a); // Apply the dynamic effect to the texture color
}