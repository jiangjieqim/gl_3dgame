#version 130
varying vec4 outColor;
varying vec2 TextCoord;
uniform sampler2D tex1;
uniform sampler2D tex2;

void main()
{
 	vec4 clr_tex1=texture2D(tex1, TextCoord);
 	vec4 clr_mask=texture2D(tex2, TextCoord);
 	
 	if(clr_mask.a<=0)
 		discard;
 	else
		gl_FragColor =clr_tex1*clr_mask.a;
}