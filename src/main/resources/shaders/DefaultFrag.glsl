#version 330

in vec2 fragUV;
in vec3 fragColor;

out vec4 outColor;

uniform sampler2D textureSampler;

void main(){
    outColor = texture(textureSampler, fragUV);
}