
varying vec2 g_texco;

#ifdef VERTEX_SHADER

attribute vec2 co;
attribute vec2 texco;

void main(void)
{
	gl_Position=gl_ModelViewProjectionMatrix*vec4(co,0.0,1.0);
	g_texco=texco;
}

#endif


#ifdef FRAGMENT_SHADER

uniform sampler2D depth_tex;

void main(void)
{
	gl_FragDepth = texture2D(depth_tex, g_texco).x;
}

#endif
