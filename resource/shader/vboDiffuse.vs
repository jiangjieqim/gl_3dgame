#version 110
uniform mat4 _mat1;
uniform mat4 _PerspectiveMatrix4x4;//Õ∏ ”æÿ’Û

attribute vec3 _Position;
attribute vec3 _Normal;
attribute vec2 _TexCoord;
varying vec2 out_texcoord;
uniform mat4 _ModelViewMatrix4x4;

void main()
{
	out_texcoord = _TexCoord;
	gl_Position = _PerspectiveMatrix4x4 * _ModelViewMatrix4x4 * _mat1*vec4(_Position, 1.0);
}