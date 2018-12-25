/*
	沿着法线方向偏移几个单位

	使用fixed function pipeline，一种有效的方法是先绘制一遍整个模型到stencil buffer，
	然后使用粗的线宽绘制一遍模型的wireframe，只画stencil buffer上非模型部分，这样外轮廓线就绘制出来了。最后再画一遍模型就好了。参见Generating Outlines in OpenGL

*/
uniform mat4 _mat1;
uniform mat4 _PerspectiveMatrix4x4;
uniform mat4 _ModelViewMatrix4x4;

uniform float _LineWidth;//边缘线宽度
void main()
{
	float offsetLine = _LineWidth;
	
	vec3 n = normalize (gl_Normal) * offsetLine;
	
	vec4 _pos = gl_Vertex ;

	_pos.xyz+=n.xyz;
	
	
	gl_Position = _PerspectiveMatrix4x4	*	_ModelViewMatrix4x4	*	_mat1	*	vec4(_pos.xyz, 1.0);
}

/*
	float offsetLine = _LineWidth;
	
	vec3 n = normalize (gl_Normal) * offsetLine;
	
	vec4 _pos = gl_Vertex ;

	_pos.xyz+=n.xyz;
	
	gl_Position = _PerspectiveMatrix4x4	*	_ModelViewMatrix4x4	*	_mat1	*	vec4(_pos.xyz, 1.0);

*/