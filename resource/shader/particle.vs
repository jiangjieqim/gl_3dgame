uniform mat4 _mat1;
uniform mat4 _PerspectiveMatrix4x4;
uniform mat4 _ModelViewMatrix4x4;

attribute vec2 _TexCoord;
varying vec2 out_texcoord;

uniform vec3 _camPos;
void main()
{
	out_texcoord = _TexCoord;
	float size = 2.0f;
	
	vec3 vAt = _camPos - gl_Vertex.xyz;
    vAt      = normalize( vAt );
    vec3 vRight = cross( vec3( 0.0, 1.0, 0.0 ), vAt );
    vec3 vUp = cross( vAt, vRight );
    vRight   = normalize( vRight );
    vUp = normalize( vUp );
	
	vec2 s  = gl_MultiTexCoord0.zw * vec2( size/2.0, size );
	vec3 vR = s.xxx * vRight;
	vec3 vU = s.yyy * vUp;
		
	vec4 dir = vec4( vR + vU, 0.0 );
	
	gl_Position = _PerspectiveMatrix4x4*_ModelViewMatrix4x4  * _mat1 * (gl_Vertex + dir);
	
	
	//gl_Position = _PerspectiveMatrix4x4 * _ModelViewMatrix4x4 * _mat1 * vec4(gl_Vertex.xyz, 1.0);
}