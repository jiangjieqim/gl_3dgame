
uniform mat4 _mat1;//此矩阵作为一个模型变换矩阵,外部传递
const vec3 tangent = vec3(1.0, 0.0, 0.0);

varying vec3 lightVec;
varying vec3 halfVec;
varying vec3 eyeVec;
const vec3 lightPos = vec3(100,100,100);//光照坐标

uniform mat4 _PerspectiveMatrix4x4;
uniform mat4 _ModelViewMatrix4x4;


void main()
{

	gl_TexCoord[0] =  gl_MultiTexCoord0;

	// Building the matrix Eye Space -> Tangent Space
	vec3 n = normalize (gl_NormalMatrix * gl_Normal);
	vec3 t = normalize (gl_NormalMatrix * tangent);
	vec3 b = cross (n, t);

	vec3 vertexPosition = vec3(gl_ModelViewMatrix *  gl_Vertex);
	//vec3 lightDir = normalize(gl_LightSource[0].position.xyz - vertexPosition);
	vec3 lightDir = normalize(lightPos.xyz - vertexPosition);
		
	// transform light and half angle vectors by tangent basis
	vec3 v;
	v.x = dot (lightDir, t);
	v.y = dot (lightDir, b);
	v.z = dot (lightDir, n);
	lightVec = normalize (v);

	v.x = dot (vertexPosition, t);
	v.y = dot (vertexPosition, b);
	v.z = dot (vertexPosition, n);
	eyeVec = normalize (v);


	vertexPosition = normalize(vertexPosition);

	/* Normalize the halfVector to pass it to the fragment shader */

	// No need to divide by two, the result is normalized anyway.
	// vec3 halfVector = normalize((vertexPosition + lightDir) / 2.0); 
	vec3 halfVector = normalize(vertexPosition + lightDir);
	v.x = dot (halfVector, t);
	v.y = dot (halfVector, b);
	v.z = dot (halfVector, n);

	// No need to normalize, t,b,n and halfVector are normal vectors.
	//normalize (v);
	halfVec = v ; 
	  
	  
	//gl_Position = gl_ModelViewProjectionMatrix*_mat1*gl_Vertex;
	gl_Position = _PerspectiveMatrix4x4	*	_ModelViewMatrix4x4	*	_mat1	*	vec4(gl_Vertex.xyz, 1.0);
}