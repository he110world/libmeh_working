
/* depth of field */

varying vec2 g_texco;

#ifdef VERTEX_SHADER
attribute vec2 co,texco;

void main(void)
{
	g_texco=texco;
	gl_Position=gl_ModelViewProjectionMatrix*vec4(co,0.0,1.0);
}

#endif

#ifdef FRAGMENT_SHADER

uniform sampler2D color_tex, depth_tex;
uniform float focus, noblur, maxblur;
uniform float near,far;

float eyez;
float computecoc(sampler2D s, vec2 tc)
{
	float z=2.0*texture2D(s,tc).x-1.0;
	
	// convert depth to eye space

	eyez=2.0*near*far/((far-near)*z-(far+near));

	return clamp(max(0.0,abs(eyez-focus)-noblur)/(maxblur-noblur), 0.0,1.0);
}

void main(void)
{
	float coc=computecoc(depth_tex, g_texco);
	gl_FragColor=vec4(coc,coc,coc,1.0); //*0.00001 + abs(eyez-focus)*0.01;
}

#endif
