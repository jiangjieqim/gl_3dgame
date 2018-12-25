varying vec4 position;  
void main(void)  
{  
    position = gl_ModelViewMatrix * gl_Vertex;  
    gl_Position = ftransform();  
}
