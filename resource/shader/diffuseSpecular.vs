/*
	漫射 + 高光 + 贴图
*/
uniform mat4 _mat1;//此矩阵作为一个模型变换矩阵,外部传递
uniform float bSpecular;//是否启用高光
varying vec4 color;//向片段着色其传递的参数 
uniform mat4 _PerspectiveMatrix4x4;
uniform mat4 _ModelViewMatrix4x4;
void main()
{
	
	vec3 lightposition = vec3(100,100,100);	//光源位置
	vec3 eyeposition = vec3(0,10,30);	//相机坐标
	vec4 ambient=vec4(1.0,0.0,0.0,0.0);	//环境光颜色
	vec4 lightcolor=vec4(1.0,1.0,1.0,1.0);	//光源颜色
	float Ns = 8.5;//高光系数 该值需要>1.0正常才正常
	float attenuation=0.8;//光线的衰减系数	,该值越大,则高光光圈越大
	
	//======================================================

	vec3 ECPosition = vec3(gl_ModelViewMatrix * gl_Vertex);

	vec3 N = normalize(gl_NormalMatrix * gl_Normal);
 	vec3 L = normalize(lightposition - ECPosition);
 	vec3 V = normalize(eyeposition - ECPosition);
 	vec3 H = normalize(V + L); 
	
	vec4 diffuse = lightcolor*max(dot(N , L),0);
 	
	if(bSpecular>0){
		vec4 specular = lightcolor * pow(max(dot(N , H) , 0) , Ns) * attenuation;
		
		//注:diffuse + specular可能>0,所以用clamp取0~1.0之间的值,防止曝光
		color=clamp((diffuse + specular),0.0,1.0);
	}else{
		color=diffuse;//只取漫射值
	}
	
	gl_TexCoord[0] = gl_MultiTexCoord0;
	//gl_Position = gl_ModelViewProjectionMatrix*_mat1*gl_Vertex;

	gl_Position = _PerspectiveMatrix4x4*_ModelViewMatrix4x4*_mat1*vec4(gl_Vertex.xyz, 1.0);
}
