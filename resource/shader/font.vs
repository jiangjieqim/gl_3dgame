#version 110

uniform mat4 _mat1;//此矩阵作为一个模型变换矩阵,外部传递
uniform mat4 _PerspectiveMatrix4x4;
uniform mat4 _ModelViewMatrix4x4;

void main()
{
	gl_TexCoord[0].xy = gl_MultiTexCoord0.xy;
	
	gl_Position = _PerspectiveMatrix4x4 * _ModelViewMatrix4x4 * _mat1 * vec4(gl_Vertex.xyz, 1.0);
}
