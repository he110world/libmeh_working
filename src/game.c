#define USE_MEH_TYPE 1

#include "backend.h"
#include <assert.h>
#include <alloca.h>
#include <stdio.h>
#include <stdlib.h>

void c_test_bgnforward()
{
	cvar_setf("forwardspeed", -.2);
}

void c_test_bgnback()
{
	cvar_setf("forwardspeed", .2);
}

void c_test_bgnstrafeleft()
{
	cvar_setf("strafespeed", -.2);
}

void c_test_bgnstraferight()
{
	cvar_setf("strafespeed", .2);
}

void c_test_endforward()
{
	cvar_setf("forwardspeed", 0);
}

void c_test_endback()
{
	cvar_setf("forwardspeed", 0);
}

void c_test_endstrafeleft()
{
	cvar_setf("strafespeed", 0);
}

void c_test_endstraferight()
{
	cvar_setf("strafespeed", 0);
}


void c_test_bgnspinleft()
{
	cvar_setf("spinspeed", -1);
}

void c_test_endspinleft()
{
	cvar_setf("spinspeed", 0);
}


void c_test_bgnspinright()
{
	cvar_setf("spinspeed", 1);
}

void c_test_endspinright()
{
	cvar_setf("spinspeed", 0);
}

void c_test_bgnmoveup()
{
	cvar_setf("verticalspeed", .5);
}

void c_test_endmoveup()
{
	cvar_setf("verticalspeed", 0);
}

void c_test_bgnmovedown()
{
	cvar_setf("verticalspeed", -.5);
}

void c_test_endmovedown()
{
	cvar_setf("verticalspeed", 0);
}

typedef struct {
	v3_t co;
	float texco[2];
	v3_t normal;
	v3_t tangent;
	v3_t binormal;
} test_vbfmt_t;


uint texcolor;
uint texrelief;
uint texpyramid;
uint texdepth;
uint shaderpdm, shaderpdm_mrt;

uint gbuf_color;
uint gbuf_normal;
uint gbuf_depth;

#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL/SDL.h>

void test_initgbuf()
{

#if 1

#define MRT 0

#if MRT
	if (!shaderpdm_mrt) shaderpdm_mrt=vfs_load("shaders/pdm_mrt.c");
	rnd_tg("ttd", gbuf_color, gbuf_normal, gbuf_depth);
#else
	rnd_tg("td", gbuf_color, gbuf_depth);
//	rnd_tg(0);
#endif

	rnd_clearall();
	
	////////////////////////////////
	test_vbfmt_t verts[4];
	verts[0].co=vec(-10, -10, 30);
	verts[1].co=vec(10, -10, 30);
	verts[2].co=vec(10, 10, 30);
	verts[3].co=vec(-10, 10, 30);
	for (int i=0; i<4; i++) {
		verts[i].normal=vec(0,0,-1);
		verts[i].tangent=vec(1,0,0);
		verts[i].binormal=vec(0,1,0);
	}

	verts[0].texco[0]=0;
	verts[0].texco[1]=0;
	verts[1].texco[0]=.5;
	verts[1].texco[1]=0;
	verts[2].texco[0]=.5;
	verts[2].texco[1]=.5;
	verts[3].texco[0]=0;
	verts[3].texco[1]=.5;

#if MRT
	vfs_use(shaderpdm_mrt);
#else
	vfs_use(shaderpdm);
#endif


	samp2d("color_map", texcolor);
	samp2d("relief_map", texrelief);
	samp2d("pyramid_map", texpyramid);
	samp2d("depth_map", texdepth);
	uni3f("lightpos", 5.36f, 10.0f, 1.8f);
	int vb=vb_alloc(4, sizeof(test_vbfmt_t), verts);
	rendq("{co texco normal tangent binormal}", -1, vb);
	vb_free(vb);

#endif //0

	vfs_use(-1);
#if 1
	float tcbnd[4]={0,0,1,1};
	float qbnd[4]={-10,-10,10,10};

	glColor4f(1,1,1,1);
//	glEnable(GL_TEXTURE_2D);
//	glBindTexture(GL_TEXTURE_2D, texcolor); //texpyramid);//texrelief); //texlena);

	float z=50;
	glBegin(GL_QUADS);
	glTexCoord2f(tcbnd[0], tcbnd[1]);	glVertex3f(qbnd[0], qbnd[1], z);
	glTexCoord2f(tcbnd[2], tcbnd[1]);	glVertex3f(qbnd[2], qbnd[1], z);
	glTexCoord2f(tcbnd[2], tcbnd[3]);	glVertex3f(qbnd[2], qbnd[3], z);
	glTexCoord2f(tcbnd[0], tcbnd[3]);	glVertex3f(qbnd[0], qbnd[3], z);
	glEnd();

//	glDisable(GL_TEXTURE_2D); //TODO

#endif


	
}


cvec_bgn(game);
cvec(prevmodelview);
cvec(invmodelview);
cvec(invproj);
cvec(projection);
cvec(color_tex);
cvec(depth_tex);
cvec_end(game);



void test_motionblur()
{
	static uint shadermotionblur=0;

	if (!shadermotionblur) shadermotionblur=vfs_load("shaders/motionblur.c");
	vfs_use(shadermotionblur);

	unifv("prevmodelview", prevmodelview);
	unifv("invmodelview", invmodelview);
	unifv("invproj", invproj);
	unifv("projection", projection);

	samp2d("depth_tex", gbuf_depth);
	samp2d("color_tex", gbuf_color);

	float co[4]={0,0,800,600};
	float texco[4]={0,0,1,1};

	rnd_tg(0);
	rnd_scrncoord();
	rendrect("co texco", co, texco);
}


#if 0
void test_motionblurfilter()
{
	uint in[]={gbuf_depth, gbuf_color};
	uint out[1];
	rt_lock(gbuf_depth); // so gbuf_depth won't be released even if it's unreferenced in the filter
	filter_use("motionblur", in, out);
}
#endif

void c_testeditor()
{
    glClearColor(0,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
}

void c_testdraw()
{
	gbuf_color=rt_alloc(RT_4B, 0);
	gbuf_normal=rt_alloc(RT_4B, 0);
	gbuf_depth=rt_alloc(RT_DEPTH, 0);	

	cs_printf("color=%i, normal=%i, depth=%i\n", gbuf_color,gbuf_normal,gbuf_depth);

	float dx,dy=0,dz;
	dx=cvar_getf("strafespeed", NULL);
	dy=cvar_getf("verticalspeed", NULL);
	dz=cvar_getf("forwardspeed", NULL);
	float du=cvar_getf("spinspeed", NULL);
	cam3_movetg("testcam3", dx,dy,dz);
	cam3_update("testcam3", du, 1.0/60.0);
	cam3_use("testcam3");

	float modelview[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, modelview);
	glGetFloatv(GL_PROJECTION_MATRIX, projection);
	invmat4(invproj, projection);
	invmat4(invmodelview, modelview);

	glEnable(GL_DEPTH_TEST);
	rnd_clearall();

	test_initgbuf();

	glDisable(GL_DEPTH_TEST);

//	test_motionblur();

//	test_dof2();
//	test_gaussianblur();

	color_tex[0]=gbuf_color;
	depth_tex[0]=gbuf_depth;
	filter_use("test.fil");

	vfs_use(-1);

	memcpy(prevmodelview, modelview, 16*sizeof(float));

	rt_free(gbuf_color);
	rt_free(gbuf_normal);
	rt_free(gbuf_depth);

}




#include "stb_image.h"

void dllInit()
{
	cmd_add("+forward", c_test_bgnforward);
	cmd_add("-forward", c_test_endforward);
	cmd_add("+back", c_test_bgnback);
	cmd_add("-back", c_test_endback);
	cmd_add("+strafeleft", c_test_bgnstrafeleft);
	cmd_add("-strafeleft", c_test_endstrafeleft);
	cmd_add("+straferight", c_test_bgnstraferight);
	cmd_add("-straferight", c_test_endstraferight);
	cmd_add("+moveup", c_test_bgnmoveup);
	cmd_add("-moveup", c_test_endmoveup);
	cmd_add("+movedown", c_test_bgnmovedown);
	cmd_add("-movedown", c_test_endmovedown);
	cmd_add("+spinleft", c_test_bgnspinleft);
	cmd_add("-spinleft", c_test_endspinleft);
	cmd_add("+spinright", c_test_bgnspinright);
	cmd_add("-spinright", c_test_endspinright);	
	cmd_add("testdraw", c_testdraw);

	vu_bgn("hello");
	{
		vu_cmd("testdraw");

		vu_kcmd('a', "strafeleft");
		vu_kcmd('d', "straferight");
		vu_kcmd('w', "forward");
		vu_kcmd('s', "back");
		vu_kcmd(SDLK_UP, "moveup");
		vu_kcmd(SDLK_DOWN, "movedown");
		vu_kcmd(SDLK_LEFT, "spinleft");
		vu_kcmd(SDLK_RIGHT, "spinright");
	}
	vu_end();

	v3_t target={-10,5,15};

	cam3_add("testcam3", target, add(target,vec(0,2.5,-10)), 7, 1);

	shaderpdm=vfs_load("shaders/pdm.c");

	
	int w,h,comp;

	uchar *normal_data=stbi_load("wall_n.tga", &w, &h, &comp, 3);
	texrelief=tex3b(w,h, 011, normal_data);
	free(normal_data);
	
	uchar *color_data=stbi_load("wall_d.tga", &w, &h, &comp, 3);
	texcolor=tex3b(w,h, 011, color_data);
	free(color_data);

	uchar *depth_data=stbi_load("wall_h.tga", &w,&h,&comp,1);
	texpyramid=tex1b(w,h, 100, depth_data);
	
#if 1
	uchar *mem=alloca(w*h/2);
	uchar *level[10];
	level[0]=depth_data;
	level[1]=mem;
	int L=1;
	for (int sz=w/2; sz; sz>>=1) {
		uchar *p=level[L];

		for (int i=0; i<sz; i++) {
			for (int j=0; j<sz; j++) {
				uchar d[4];
				int w=2*sz;
				int x,y;

				x=2*i; y=2*j;
				d[0]=level[L-1][y*w+x];
				x=2*i+1; y=2*j;
				d[1]=level[L-1][y*w+x];
				x=2*i; y=2*j+1;
				d[2]=level[L-1][y*w+x];
				x=2*i+1; y=2*j+1;
				d[3]=level[L-1][y*w+x];

				level[L][j*sz+i] = min(min(d[0],d[1]),min(d[2],d[3]));
			}
		}

/*
		image_t img;
		img.width=img.height=sz;
		img.type=1;
		img.depth=8;
		img.data=level[L];

		char nam[32];
		sprintf(nam, "pyramid%i.tga", L);
		writetga(&img, nam);
*/

		glTexImage2D(GL_TEXTURE_2D, L/*level*/, GL_ALPHA8, sz,sz, 0/*border*/,
			     GL_ALPHA, GL_UNSIGNED_BYTE, level[L]);

		level[L+1]=level[L]+sz*sz;
		++L;
	}
#endif


#if 1
	texdepth=tex1b(w,h, 211, depth_data);
//	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE); /* hardware mipmap generation */	
	assert(!gluBuild2DMipmaps(GL_TEXTURE_2D, GL_ALPHA8, w, h, GL_ALPHA, GL_UNSIGNED_BYTE, depth_data));

#endif
	free(depth_data);
	
}
