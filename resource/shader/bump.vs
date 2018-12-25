varying vec3 LightDir;
varying vec3 EyeDir;
varying vec2 TexCoord;
uniform mat4 _mat1;//此矩阵作为一个模型变换矩阵,外部传递
uniform mat4 _PerspectiveMatrix4x4;
uniform mat4 _ModelViewMatrix4x4;
void main()
{
	//灯光坐标
	vec3 LightPosition = vec3(0,20,100);

	vec3 tangent; 
	vec3 binormal; 
	vec3 c1 = cross(gl_Normal, vec3(0.0, 0.0, 1.0)); 
	vec3 c2 = cross(gl_Normal, vec3(0.0, 1.0, 0.0)); 
   
	if(length(c1)>length(c2))
	{
		tangent = c1;   
	}
	else
	{
		tangent = c2;   
	}

	EyeDir = vec3(gl_ModelViewMatrix * gl_Vertex);
	TexCoord = gl_MultiTexCoord0.st;
	vec3 n = normalize(gl_NormalMatrix * gl_Normal);
	vec3 t = normalize(gl_NormalMatrix * tangent);
	vec3 b = cross(n, t);
	vec3 v;
	v.x = dot(LightPosition, t);
	v.y = dot(LightPosition, b);
	v.z = dot(LightPosition, n);
	LightDir = normalize(v);
	v.x = dot(EyeDir, t);
	v.y = dot(EyeDir, b);
	v.z = dot(EyeDir, n);
	EyeDir = normalize(v);
	
	//gl_Position = gl_ModelViewProjectionMatrix*_mat1*gl_Vertex;
	gl_Position = _PerspectiveMatrix4x4*_ModelViewMatrix4x4*_mat1*vec4(gl_Vertex.xyz, 1.0);
}