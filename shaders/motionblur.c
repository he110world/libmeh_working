
/*
  Motion blur shader: read in depth
 */
varying vec2 texcoord;

#ifdef VERTEX_SHADER

attribute vec2 co;
attribute vec2 texco;

void main(void)
{
//	texcoord=texco;
	gl_TexCoord[0].st=texco;
	gl_Position=gl_ModelViewProjectionMatrix*vec4(co,0.0,1.0);
}


#endif

#ifdef FRAGMENT_SHADER

uniform sampler2D depth_tex;
uniform sampler2D color_tex;
uniform mat4 prevmodelview;
uniform mat4 projection;
uniform mat4 invproj, invmodelview;

void main(void)
{

	vec2 texcoord=gl_TexCoord[0];

#if 1
	float z=texture2D(depth_tex, texcoord)*2.0-1.0;
	vec4 ndc=vec4(2.0*texcoord-1.0, z, 1.0);

	vec4 pndc=invproj*ndc;

	vec4 eye=pndc/pndc.w;

	vec4 obj=invmodelview*eye; // world space position

	vec4 oldeye = prevmodelview*obj;
	float oldw=-oldeye.z;

	vec4 oldndc=(projection*oldeye)/oldw;

	vec2 v=(0.5*(ndc-oldndc)/1.0).xy;
#endif

	vec4 color=texture2D(color_tex, texcoord);
	vec4 color0=color;
	for (int i=1; i<8; ++i, texcoord+=v) {
		color += texture2D(color_tex, texcoord);
	}
	
	gl_FragColor=color/8.0;
}

#endif
