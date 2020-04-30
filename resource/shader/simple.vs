#version 110
attribute vec3 _Position;
attribute vec2 _TexCoord;
varying vec2 out_texcoord;
uniform mat4 _mat1;

void main(){
	out_texcoord = _TexCoord;
	gl_Position = _mat1*vec4(_Position, 1.0);
}