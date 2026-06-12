#version 330

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inUV;
layout(location = 2) in vec3 inColor;

out vec2 fragUV;
out vec3 fragColor;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main(){
    gl_Position = projection * view * model * vec4(inPosition, 1.0);
    fragUV = inUV;
    fragColor = inColor;
}