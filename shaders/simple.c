varying vec3 L, E;
varying vec2 texcoord;
varying vec3 cubecoord;
attribute vec3 tangent;
uniform vec4 light;

void main(void){
	gl_Position = ftransform();

	texcoord=gl_MultiTexCoord0.xy;
	cubecoord=gl_MultiTexCoord1.xyz;

	vec3 n = normalize(gl_NormalMatrix*gl_Normal);
	vec3 t = normalize(gl_NormalMatrix*tangent);
	vec3 b = cross(n,t);

	vec3 vert = vec3(gl_ModelViewMatrix * gl_Vertex);
	vec3 tmp = vec3(gl_ModelViewMatrix*light)-vert;
//	vec3 tmp = light-vert;

//	vec3 tmp = light-gl_Vertex.xyz;
	L.x = dot(tmp,t);
	L.y = dot(tmp,b);
	L.z = dot(tmp,n);
	tmp = -vert;
	E.x = dot(tmp,t);
	E.y = dot(tmp,b);
	E.z = dot(tmp,n);
}
varying vec3 L,E;
varying vec2 texcoord;
varying vec3 cubecoord;
uniform sampler2D colortex, normaltex;
uniform samplerCube stenciltex;	

float appr_pow(float t)
{
	return max(0.0, 8.0*t-7.0);
}

void main()
{
	float invradius=1.0/100.0;
	float distsqr = dot(L,L);
	float att = clamp(1.0 - invradius * sqrt(distsqr), 0.0, 1.0);
	vec3 newL = L*inversesqrt(distsqr);
	vec3 newE = normalize(E);
	vec4 color=texture2D(colortex, texcoord);
	float opaque=textureCube(stenciltex, cubecoord).a;
	vec3 N=normalize(texture2D(normaltex, texcoord).xyz*2.0 - 1.0);
	float diffuse = max(dot(N,newL),0.0);
//	float spec = pow(clamp(dot(reflect(-newL,N),newE),0.0,1.0), 16.0);

//	float spec = clamp(dot(reflect(-newL,N),newE),0.0,1.0);

	float spec = dot(reflect(-newL,N),newE);


//	vec3 R=normalize(2*N-newL);
//	float spec = appr_pow(clamp(dot(N,R),0.0,1.0));
//	float spec = pow(clamp(dot(N,R),0.0,1.0),1.0);

//	gl_FragColor = att*(vec4(diffuse)*color+vec4(.5*spec));

	gl_FragColor = att*(vec4(diffuse+spec)*color);
	gl_FragColor.a = opaque;
}