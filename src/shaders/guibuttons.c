
varying vec4 vertcolor;

#ifdef VERTEX_SHADER
attribute vec3 co;
attribute vec2 texco;
attribute vec4 color;

void main(void)
{
	gl_TexCoord[0].st=texco.st;
	gl_Position=gl_ModelViewProjectionMatrix*vec4(co,1.0);
	vertcolor=color;
}

#endif


#ifdef FRAGMENT_SHADER
uniform sampler2D fnt_tex;

void main(void)
{
	gl_FragColor=texture2D(fnt_tex, gl_TexCoord[0].st).a*vertcolor+vec4(0.5);
}

#endif
