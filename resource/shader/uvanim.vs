uniform mat4 _mat1;//此矩阵作为一个模型变换矩阵,外部传递
void main(){
	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_Position = gl_ModelViewProjectionMatrix*_mat1*gl_Vertex;
}
