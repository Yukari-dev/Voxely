#version 330

in vec2 fragUV;
in vec3 fragColor;
in vec3 fragNormal;

out vec4 outColor;

uniform sampler2D textureSampler;

uniform vec3 sunDirection;
uniform vec3 sunColor;
uniform float sunIntensity;

uniform vec3 ambientColor;
uniform vec3 ambientIntensity;

void main(){
    vec4 texColor = texture(textureSampler, fragUV);
    vec3 ambient = ambientColor * ambientIntensity;

    vec3 norm = normalize(fragNormal);
    vec3 lightDir = normalize(sunDirection);
    float diffImpact = max(dot(norm, lightDir), 0.0);

    vec3 diffuse = diffImpact * sunColor * sunIntensity;

    vec3 finalLightning = ambient + diffuse;
    outColor = vec4(texColor.rgb*finalLightning, 1.0);
}