#version 330 core

layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec3 vColor;


uniform vec3 displacement;
uniform vec3 anchorPosition;
out vec3 color;

void main()
{
	if((abs(vPosition.x - anchorPosition.x) < 0.00001) && (abs(vPosition.y - anchorPosition.y) < 0.00001) && (abs(vPosition.z - anchorPosition.z) < 0.00001)){
		gl_Position = vec4(vPosition, 1.0);
	}else{
		gl_Position = vec4(vPosition.x, vPosition.y + (0.35 * displacement.y), vPosition.z, 1.0);
	}
	color = vColor;
}