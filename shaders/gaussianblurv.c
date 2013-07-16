
/*
  Vertical gaussian blur, 5-taps

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

uniform sampler2D color_tex;
uniform vec2 scrnsz;

vec4 gaussblur(vec2 tc)
{
	vec2 d=vec2(0.0, 2.0/scrnsz.y);
	vec4 color=0.0;

	color+=texture2D(color_tex, tc-2.0*d)*0.0539909665;
	color+=texture2D(color_tex, tc-d)*0.2419707245;
	color+=texture2D(color_tex, tc)*0.3989422804;
	color+=texture2D(color_tex, tc+d)*0.2419707245;
	color+=texture2D(color_tex, tc+2.0*d)*0.0539909665;

	return color;
}

void main(void)
{
	gl_FragColor=gaussblur(g_texcoord);
}

#endif
