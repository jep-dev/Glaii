#version 330

layout (std140) uniform mat4 mvp;

in vec4 arg0;

void main(){
	gl_Position = mvp * arg0;
}
