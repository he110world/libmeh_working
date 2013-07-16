
/*
  Horizontal gaussian blur, 5-taps

 */
varying vec2 g_texcoord;

#ifdef VERTEX_SHADER
attribute vec2 co, texco;

void main(void)
{
	g_texcoord=texco;
	gl_Position=gl_ModelViewProjectionMatrix*vec4(co,0.0,1.0);
}
#endif



#ifdef FRAGMENT_SHADER

uniform sampler2D color_coc_tex;
uniform vec2 scrnsz;

vec4 gaussblur(vec2 tc, vec2 d)
{
	vec4 color=0.0;
	
	float c1=texture2D(color_coc_tex, tc-2.0*d).a;
	float c2=texture2D(color_coc_tex, tc-d).a;
	float c3=texture2D(color_coc_tex, tc).a;	
	float c4=texture2D(color_coc_tex, tc+d).a;
	float c5=texture2D(color_coc_tex, tc+2.0*d).a;

	color+=texture2D(color_coc_tex, tc-2.0*d)*vec4(c1,c1,c1,0.0539909665);
	color+=texture2D(color_coc_tex, tc-d)*vec4(c2,c2,c2,0.2419707245);
	color+=texture2D(color_coc_tex, tc)*vec4(c3,c3,c3,0.3989422804);
	color+=texture2D(color_coc_tex, tc+d)*vec4(c4,c4,c4,0.2419707245);
	color+=texture2D(color_coc_tex, tc+2.0*d)*vec4(c5,c5,c5,0.0539909665);

	return color;
}


#endif
