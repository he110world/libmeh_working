
varying vec4 hpos;
varying vec3 eye;
varying vec3 light;
varying vec2 texcoord;

#ifdef VERTEX_SHADER

attribute vec3 co;
attribute vec2 texco;
attribute vec3 normal;
attribute vec3 tangent;
attribute vec3 binormal;

uniform vec3 lightpos;


// perform basic transformation for per-pixel displacement mapping
void main(void)
{
	// vertex position in object space
	vec4 pos = vec4(co,1.0);

	// vertex position in clip space
	hpos = gl_ModelViewProjectionMatrix*pos;

	// copy color and texture coordinates  		
	texcoord = texco; // * tile;

	// compute modelview rotation only part   
	mat3 modelviewrot;
	modelviewrot[0] = gl_ModelViewMatrix[0].xyz;
	modelviewrot[1] = gl_ModelViewMatrix[1].xyz;
	modelviewrot[2] = gl_ModelViewMatrix[2].xyz;

	// tangent vectors in view space
	vec3 tangent = modelviewrot*tangent;
	vec3 binormal = modelviewrot*binormal;
	vec3 normal = modelviewrot*normal;
	mat3 tangentspace = mat3(tangent,binormal,normal);

	// vertex position in view space ( with model transformations )
	vec3 vpos = (gl_ModelViewMatrix*pos).xyz;

	// view in tangent space
	eye = tangentspace*vpos;


	// light position in tangent space
	vec4 L = vec4(lightpos.xyz,1.0);
	L = gl_ModelViewMatrix*L;
//	L.z*=-1.0;
	L.xyz = tangentspace*(L.xyz-vpos);
	light = L.xyz;


	gl_Position=hpos;
}


#endif // VERTEX_SHADER


#ifdef FRAGMENT_SHADER

// setup ray pos and dir based on view vector
// and apply d bias and d factor
void setup_ray(out vec3 p, out vec3 v)
{
	p = vec3(texcoord,0.0);
	v = normalize(eye);

	v.z = abs(v.z);

	float db = 1.0-v.z; 
	db *= db; 
	db = 1.0 - db * db;
	v.xy *= db;

	v.xy *= .1;//d_factor;
	v /= v.z;	
}


uniform sampler2D color_map;
uniform sampler2D relief_map;
uniform sampler2D pyramid_map;

uniform sampler2D depth_map;

// do normal mapping using given texture coordinate
// tangent space phong lighting with optional border clamp
// normal X and Y stored in red and green channels
vec4 normal_mapping(vec2 texcoord, out vec4 clr, out vec3 nml )
{
	// derivative texcoord from input texcoord
	vec2 dx = dFdx(texcoord);
	vec2 dy = dFdy(texcoord);

	// color map
	vec4 color = texture2DGrad(color_map, texcoord, dx, dy);

	clr=color;

//	vec4 color = texture2D(color_map, texcoord);

	// normal map
	vec3 normal = texture2DGrad(relief_map, texcoord, dx, dy);

	nml=normal;

//	vec4 normal = texture2D(relief_map, texcoord);

#if 0
	normal.xy = 2.0 * normal.xy - 1.0;
	normal.y = -normal.y;
	normal.z = sqrt( 1.0 - dot( normal.xy, normal.xy ) );
#endif

	normal = 2.0*normal-1.0;

	// light and view in tangent space

	vec3 l=normalize(light);
	vec3 v=normalize(eye);

	// compute diffuse and specular terms
	float diff = clamp( dot( l, normal.xyz ),0.0,1.0 );
	float spec = clamp( dot( normalize( l - v ), normal.xyz ),0.0,1.0 );

	// attenuation factor
	float att = 1.0 - max( 0.0, l.z ); 
	att = 1.0 - att * att;

	// border clamp
//	if ( border_clamp )
	{
#if 0
		if ( texcoord.x < 0.0 ) discard;
		if ( texcoord.y < 0.0 ) discard;
		if ( texcoord.x > 1.0 ) discard;
		if ( texcoord.y > 1.0 ) discard;
#endif
	}

	// compute final color
	vec4 finalcolor;
	const float shine=32.0;
	const vec3 ambient=vec3(0.2,0.2,0.2);
	const vec3 diffuse=vec3(0.9,0.9,0.9);
	const vec3 specular=vec3(0.2,0.2,0.2);

	finalcolor.xyz = ambient * color.xyz +
		att * ( color.xyz * diffuse*diff +
		specular * pow( spec, shine ) );
	finalcolor.w = 1.0;
	return finalcolor;
}


int count=0;
void ray_intersect_pdm( inout vec3 p, inout vec3 v )
{	
	const float EPSILON = 0.001;
	const int MAX_LEVEL = 8;
	const float NODE_COUNT = 256.0;	// displacement map resolution
	const float TEXEL_SPAN_HALF = 1.0 / NODE_COUNT / 2.0;	

	float fDeltaNC = TEXEL_SPAN_HALF * .1; //d_factor;	// offset for node crossing

	vec3 p2 = p;		// current position	
	int level = MAX_LEVEL;	// current level
	while ( level >= 0 )
	{
//		vec4 uv = vec4( p2.xyz, level );
//		float d = texture2DLod( pyramid_map, uv.xy, (float)level).w;

		float d = texture2DLod(pyramid_map, p2.xy, float(level)).w;
		if ( d > p2.z + EPSILON )
		{
			vec3 tmpP2 = p + v * d;	// new point ( predictive )

			// test node IDs
			float nodeCount = exp2(float(MAX_LEVEL - level));
			vec4 nodeID = vec4( p2.xy, tmpP2.xy );	
			nodeID = floor( nodeID * nodeCount );
			vec2 test = abs( nodeID.xy - nodeID.zw );

			if ( test.x + test.y > EPSILON )
			{
				float texelSpan = 1.0 / nodeCount;

				vec2 dirSign = ( sign(v.xy ) + 1.0 ) * 0.5;

				// distance to the next node's boundary
				vec2 a = p2.xy - p.xy;
				vec2 p3 = ( nodeID.xy + dirSign ) * texelSpan;
				vec2 b = p3.xy - p.xy;

				// node crossing
				vec2 dNC = ( b.xy * p2.z ) / a.xy;                
				d = min( d, min( dNC.x, dNC.y ) ) + fDeltaNC;

				++level;
			}

			p2 = p + v * d;
		}


		
#if 1

//TESTING

		// refinement with linearly filtered depth
		if ( level <= 0 )
		{
			vec2 ray2D =  p2.xy - p.xy;
			float rayLength = length( ray2D );
			
			float depthb = p2.z * ( rayLength + TEXEL_SPAN_HALF ) / rayLength;
			vec4 p2b = vec4( p + v * depthb, level );
			float depth2b = texture2DLod(depth_map, p2b.xy, float(level)).w;		// linearly filtered sampler
			if ( depth2b > p2b.z + EPSILON )
			{
				p2 = p2b.xyz;
				level++;
			}	
		}

//~TESTING
#endif
		count++;
		if (count>100) break;  // DEBUGGED: without this, couple of pixels may result in INFINITE LOOP =>fps=0.01

		--level;
	} // end loop
	p = p2;
}


const int search_step=100;
void ray_intersect_linear( inout vec3 p, inout vec3 v )
{
	v /= v.z * search_step;

	for( int i = 0; i < search_step; i++  )
	{
		float tex = texture2D(pyramid_map, p.xy ).w;
		if ( p.z < tex )
		{
			p += v;
		}
	}
}

void ray_intersect_binary( inout vec3 p, inout vec3 v )
{
	const int num_steps_bin = 7;

	for( int i = 0; i < num_steps_bin; i++  )
	{
		v *= 0.5;
		float tex = texture2D(pyramid_map, p.xy).w;
		if ( p.z < tex )
			p += v;
		else
			p -= v;
	}
}

void ray_intersect_rm( inout vec3 p, inout vec3 v )
{
	ray_intersect_linear( p, v );
	ray_intersect_binary( p, v );
}

#if 0
void main(void)
{
	vec3 p, v;

	setup_ray(p.xyz, v);
	ray_intersect_rm(p.xyz,v);

	gl_FragColor = normal_mapping(p.xy);
}

#else
void main(void)
{
	vec3 p, v;

	setup_ray(p, v);
	ray_intersect_pdm(p, v);

#if 0
	vec4 result;	
	result = normal_mapping(p.xy);
#endif

	vec4 clr;
	vec3 nml;
	vec4 result;

	result=normal_mapping(p.xy, clr,nml);
	
	
//	if(p.z > 0.5) result=vec4(1.0);	



	if (count>100) result=vec4(1.0);
//	gl_FragColor=result*1.5;


	gl_FragData[0]=clr;
	gl_FragData[1]=vec4(nml,1.0);
}
#endif

#endif //FRAGMENT_SHADER
