
#include "blurbycoc.c"

#ifdef FRAGMENT_SHADER
void main(void)
{
	vec2 d=vec2(1.0/scrnsz.x, 0.0)
	gl_FragColor=vec4(gaussblur(g_texcoord).rgb,1.0);
}

#endif
