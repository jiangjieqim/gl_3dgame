varying vec3 normal;  
uniform mat4 _PerspectiveMatrix4x4;
uniform mat4 _ModelViewMatrix4x4;
uniform mat4 _mat1;
void main()  
{  
    normal = gl_NormalMatrix * gl_Normal; 
    //gl_Position = ftransform();  
	gl_Position = _PerspectiveMatrix4x4	*	_ModelViewMatrix4x4	*	_mat1	*	vec4(gl_Vertex.xyz, 1.0);
}
