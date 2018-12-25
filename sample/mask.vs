#version 130
  
 attribute vec2 position;
 attribute vec4 inColor;
 attribute vec2 textcoord;
 

varying vec4 outColor;
 varying vec2 TextCoord;
 void main()
 {
    gl_Position = vec4(position, 0.0, 1.0);
 	outColor = inColor;
 	TextCoord=textcoord;
 }