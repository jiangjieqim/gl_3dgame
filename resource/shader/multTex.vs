//∂‡÷ÿÃ˘Õº
uniform mat4 _PerspectiveMatrix4x4;
uniform mat4 _ModelViewMatrix4x4;
uniform mat4 _mat1;
void main(void)
{
	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_TexCoord[1] = gl_MultiTexCoord1;	
	//gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	gl_Position = _PerspectiveMatrix4x4	*	_ModelViewMatrix4x4	*	_mat1	*	vec4(gl_Vertex.xyz, 1.0);
}