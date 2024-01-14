#version 330 core

in vec4 position;
in vec3 vertexColor;
out vec3 color;

void main(){
	gl_Position = position;
	color = vertexColor;
}