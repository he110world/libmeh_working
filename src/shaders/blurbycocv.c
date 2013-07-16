
#include "blurbycoc.c"

#ifdef FRAGMENT_SHADER
void main(void)
{
	vec2 d=vec2(0.0, 1.0/scrnsz.y);
	gl_FragColor=vec4(gaussblur(g_texcoord).rgb,1.0);
}

#endif
