#version 330

uniform mat4 mvp[3];

in vec4 arg0;

void main(){
	gl_Position = mvp[0] * mvp[1] * mvp[2] * arg0;
}
