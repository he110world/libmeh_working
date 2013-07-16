
varying vec4 vertcolor;

#ifdef VERTEX_SHADER
attribute vec2 co, texco;
attribute vec4 color;

void main(void)
{
	gl_TexCoord[0].st=texco.st;
	gl_Position=gl_ModelViewProjectionMatrix*vec4(co,0.0,1.0);
	vertcolor=color;
}

#endif


#ifdef FRAGMENT_SHADER
uniform sampler2D fnt_tex;

void main(void)
{
	gl_FragColor=texture2D(fnt_tex, gl_TexCoord[0].st).a*vertcolor;
}

#endif
