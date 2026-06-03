#version 450

layout(push_constant) uniform PushConstants{
    mat4 model;
} push;

layout(binding = 0) uniform UBO {
    mat4 projection;
    mat4 view;
} ubo;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;


layout(location = 0) out vec3 fragColor;

void main(){
    gl_Position = ubo.projection * ubo.view * push.model * vec4(inPosition, 1.0);
    fragColor = inColor;
}