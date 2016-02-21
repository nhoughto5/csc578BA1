#version 330 core

layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec3 vColour;
layout(location = 2) in vec3 normal;
out vec3 fColour;
uniform mat4 mvpMat;
//uniform vec3 ambientLight;
//uniform vec3 diffuseLightPosition;

void main(){
    gl_Position = mvpMat * vec4(vPosition, 1);
	//vec3 lightVector = normalize(diffuseLightPosition - vPosition);
	//fColour = vColour * dot(lightVector, normal);
	fColour = vColour;
}