#ifndef BACKEND__H
#define BACKEND__H

extern unsigned nextp2(unsigned);
extern int	nlz(unsigned);
#define lg2(x) (31-nlz(x))
#define nfact2(x) (31-nlz((x) & -(x)))
// nfact2(x) -- 800=25*32=25*2^5 => nfact2(800)=5
// 600=75*8=75*2^3 => nfact2(600)=3

#define ARRAY_INIT_CAP 16
#define ARR_CKSUM 0x4da3a98b
typedef struct {
	int	nbytes, cap;
//	unsigned	cksum;
} arr__t;
extern void *arr__new(int);
extern int arr__addnb(arr__t**,void*,int);
extern int arr__popnb(arr__t*,int);

#define arr_push(head,val) \
	((head) \
	 ? arr__addnb(&(head), NULL, sizeof(*(head))), ((head)[((arr__t*)(head)-1)->nbytes/sizeof(*(head))-1]=(val)) \
	 : ((head)=arr__new(sizeof(*(head))), *(head)=(val)))

#define arr_pushn(head,ptr,n)			\
	((head) \
	 ? arr__addnb(&(head), (ptr), (n)*sizeof(*(head)))		\
	 : ((head)=arr__new((n)*sizeof(*(head))), ((ptr) ? mymemcpy((head), (ptr), (n)*sizeof(*(head))) : 0)  ))
			       
#define arr_len(head) \
	((head) \
	 ? ((arr__t*)(head)-1)->nbytes/sizeof(*head) \
	 : 0)
/*
#define arr_pop(head) \
	((head) \
	 ? (head)[arr__popnb((head), sizeof(*(head))) / sizeof(*(head))] \
	 : NULL)
*/

#define arr_popall(head) \
	((head) \
	 ? ((arr__t*)(head)-1)->nbytes=0 \
	 : 0)

#define arr_popn(head,n) \
	((head) \
	 ? ((arr__t*)(head)-1)->nbytes-= (n)*sizeof(*head) \
	 : 0)

#define arr_pop(head) arr_popn(head,1)

#define arr_kill(head) \
	((head) \
	 ? free((arr__t*)(head)-1), head=NULL \
	 : 0)

extern char *readtextfile(const char*,char*);
#define loadtext_alloca(nam)  readtextfile((nam), alloca(flenbyname((nam))+1))
#define loadtext_malloc(nam)  readtextfile((nam), malloc(flenbyname((nam))+1))


// Memory management routines

extern void id1k_gen(int *m);
extern int id1k_del(int *m);
extern int id1k_alloc(int m);
extern int id1k_free(int m, int id);
extern int id1k_allocated(int m, int id);
extern void id1k_killall();

extern void pool_gen(int *p, int blk_ne, int elemsz);
extern int pool_del(int *p);
extern void *pool_alloc(int p);
extern int pool_free(int p, char *dat);
extern void pool_killall();

extern void bd_gen(int *id, int blksz, int nblks);
extern int bd_del(int *id);
extern int bd_alloc(int id, int sz, int *finalsz);
extern int bd_free(int id, int addr);
extern void bd_killall();

extern void cvar_set(const char *nam, const char *str);
extern char *cvar_get(const char *nam, int *ok);
extern void cvar_seti(const char *nam, int i);
extern int cvar_geti(const char *nam, int *ok);
extern void cvar_setf(const char *nam, float f);
extern float cvar_getf(const char *nam, int *ok);
extern void cvar_del(char *nam);

extern float *cvec_get(char *nam);  // don't use sizeof() over cvec!

extern int vfs_load(const char *nam);
extern void vfs_use(int id);
extern void vfs_reload(const char *nam);

extern void uni1f(const char *nam, float v0);
extern void uni2f(const char *nam, float v0, float v1);
extern void uni3f(const char *nam, float v0, float v1, float v2);
extern void uni4f(const char *nam, float v0, float v1, float v2, float v3);
extern void unifv(const char *nam, const float *dat);
extern void samp2d(const char *nam, unsigned tex);

extern int vb_alloc(int n, int sz, void *dat);
extern int eb_alloc(int n, int elemsz, void *dat);
extern void vb_free(int vb);
extern void eb_free(int eb);
extern void vb_update(int vb, int n, int sz, void *dat);
extern void eb_update(int eb, int nbytes, void *dat);
extern int vb_getcap(int vb);

extern void rend(const char *fmt, int eb, ...);
extern void rendq(const char *fmt, int eb, ...);
extern void rendrect(const char *fmt, ...);
extern void rnd_tg(const char *fmt, ...);
extern void rnd_scrncoord();
extern void rnd_scrncoordsz(int w, int h);
extern void rnd_quad(float rgba[4], float qbnd[4]);
extern void rnd_quadsz(int w, int h, float rgba[4], float qbnd[4]);
extern void rnd_texquad(unsigned tex, float tcbnd[4], float qbnd[4]);
extern void rnd_texquadsz(unsigned tex, int w, int h, float tcbnd[4], float qbnd[4]);

enum {RT_4B, RT_4H, RT_DEPTH, RT_MAX};
extern unsigned rt_alloc(int type, int level);
extern void rt_free(unsigned tex);

extern unsigned tex1b(int w, int h, int filter, void *data);
extern unsigned tex3b(int w, int h, int filter, void *data);
extern unsigned tex4b(int w, int h, int filter, void *data);
extern unsigned tex1h(int w, int h, int filter, void *data);
extern unsigned tex3h(int w, int h, int filter, void *data);
extern unsigned tex4h(int w, int h, int filter, void *data);
extern unsigned tex1f(int w, int h, int filter, void *data);
extern unsigned tex3f(int w, int h, int filter, void *data);
extern unsigned tex4f(int w, int h, int filter, void *data);
extern unsigned tex3bdxt1(int w, int h, int filter, void *data);
extern unsigned dtex(int w, int h, int filter, void *data);
extern unsigned dtex2b(int w, int h, int filter, void *data);
extern unsigned dtex3b(int w, int h, int filter, void *data);
extern unsigned dstex(int w, int h, int filter, void *data);

extern int fnt_load(const char *nam);
extern void fnt_loadslot(const char *nam, int i);
extern void fnt_use(const char *nam);
extern void fnt_useslot(int i);
extern void fnt_color3f(float r, float g, float b);
extern void fnt_color4f(float r, float g, float b, float a);
extern void fnt_color3fv(float c[3]);
extern void fnt_color4fv(float c[4]);
extern void fnt_getcolor(float c[4]);
extern void fnt_scale(float s);
extern void fnt_printf(int posx, int posy, int alignment, const char *fmt, ...);

extern void cs_printf(const char *fmt, ...);

extern void cmd_addtxt(char *t);
extern int cmd_geti(int *i);
extern int cmd_getf(float *f);
extern int cmd_gets(char *s);
extern void cmd_execnow(char *cmd);
extern void cmd_exec();
extern void cmd_add(const char *nam, void (*func)());

extern void vu_use(const char *nam);
extern void vu_force_bgn(char *nam);
extern void vu_end();

extern void vu_bgn(char *nam);
extern void vu_kcmd(int k, char *cmd);
extern void vu_cmd(char *cmd);



#define PI 3.1415926535897932384626

typedef struct{
	float x,y;
} v2_t;

typedef struct {
	float x;
	float y;
	float z;
} v3_t;

typedef struct {
	float x;
	float y;
	float z;
	float w;
} v4_t;


typedef float **mat_t;

enum {X, Y, Z};
typedef struct {
	float _[6];
} v3__t;

typedef struct {
	float w;
	v3_t v;
} q_t;


typedef struct {
	float pitch;
	float yaw;
	float roll;
} euler_t;


typedef struct {
	v3_t n;
	float d;
} plane_t;

#if 0
typedef union{
	v3_t row[3];
	float _[3][3];
} mat3x3_t;
#endif

typedef struct{
	float			data[3][3];
} mat3x3_t;


typedef float m3_t[9];
typedef float m4_t[16];

extern v3_t add (v3_t v0, v3_t v1);
extern v3_t sub (v3_t v0, v3_t v1);
extern float dot (v3_t v0, v3_t v1);
extern v3_t cross (v3_t v0, v3_t v1);
extern v3_t normalize (v3_t v);
extern v3_t scale (v3_t v, float s);
extern v3_t vec (float x, float y, float z);
extern v3_t matvec (m3_t m, v3_t v);
extern void cpmat (m3_t dest, m3_t src);
extern void matt(float dest[3][3], float src[3][3]);
extern void matmat(float dest[3][3], float m[3][3], float r[3][3]);
extern float length (v3_t v);
extern int ludecomp (float A[], float LU[], int P[], int N);
extern int qrdecomp (float A[], float Q[], float R[], int N);
extern int lusolve (float A0[], float x[], float b0[], int N);
extern int luinv (float A0[], float Ainv[], int N);
extern int eigenvalue (float A0[], float lambda[], int N);
extern int eigensym (float A0[], float lambda[], float U[], int N);
extern void printmat (char *name, float A[], int N);
extern void printvec (char *name, float v[], int N);
extern void orthonormalization3x3(float m[3][3]);
extern v3_t mat3x3_v3(float m[3][3], v3_t v);
extern v3_t matv3(float m[3][3], v3_t v);
extern v3_t mattv3(float m[3][3], v3_t v);
extern q_t addq (q_t q0, q_t q1);
extern q_t subq (q_t q0, q_t q1);
extern q_t crossq (q_t q0, q_t q1);
extern float dotq (q_t q0, q_t q1);
extern q_t scaleq(q_t q0, float k);
extern q_t normalizeq (q_t q0);
extern q_t conjq (q_t q);
extern void q2mat(q_t q, float m[3][3]);
extern q_t mat2q(float m[3][3]);
extern v3_t rotbyq(q_t q, v3_t v);
extern void euler2axes (euler_t euler, v3_t axis[3]);
extern euler_t euler (float p, float y, float r);
extern int invmat4(float i[16], const float m[16]);

#define radsin(rad) sinf (rad)
#define radcos(rad) cosf (rad)

#define ANG_TO_RAD(ang) (ang) * 0.01745329252
#define RAD_TO_ANG(rad) (rad) * 57.29577951

#define DEG2RAD( d ) ( ( d ) *PI / 180.0f )
#define RAD2DEG( r ) ( ( r ) * 180.0f / PI )
#define SIND( d ) sinf( DEG2RAD( (d) ) )
#define COSD( d ) cosf( DEG2RAD( (d) ) )
#define TAND( d ) tanf( DEG2RAD( (d) ) )

#define MATRIX(m, w, h, i, j) m[(j)*(h) + (i)]
#define MATRIX_SAFE(m, w, h, i, j) m[(j)%(h)*(h) + (i)%(w)]

#define EPSILON 0.00001

#define max(a, b) ((a)>(b) ? (a) : (b))
#define min(a, b) ((a)<(b) ? (a) : (b))

#define WRAP(f, mini, maxi) ((f) >= (mini) ? ((f) < (maxi) ? (f) : (f) - (maxi) + (mini)) : (f) + (maxi) - (mini))
#define CLAMP(f, mini, maxi) ((f) < (mini) ? (mini) : ((f) > (maxi) ? (maxi) : (f))

#define v3_to_array (v) ((float *) & (v))
#define ARRAY(v) ((float *)&(v))

#define matv matv3
#define proj dot


typedef struct cvec_s{
	char *nam;
	float v[16];
	int id;
} cvec_t;

#define cvec(aa) \
	static cvec_t cvec__N_a_M_e_##aa __attribute__((aligned (4) )) ={ .nam= #aa }; \
	static float *aa __attribute__((aligned (4) ))=cvec__N_a_M_e_##aa.v;

#define cvec_end(filename) \
	static cvec_t cvec_end_##filename __attribute__((aligned (4) ))= {.nam= 0 }; \
 	void cvec_init_##filename(cvec_t **first, int *n) {	\
		cvec_t *begin=&cvec_bgn_##filename; \
		cvec_t *end=&cvec_end_##filename;\
		*first=(float**)(begin+1)+1;				\
		*n = ((int)end- (int)(*first))/(sizeof(cvec_t)+sizeof(float*))   ; \
	}

#define cvec_bgn(filename) \
	static cvec_t cvec_bgn_##filename __attribute__((aligned (4) ))={ .nam= 0, .id=__LINE__} ; \
	static float *cvec_placeholder_##filename __attribute__((aligned (4) ))=cvec_bgn_##filename.v;


enum {TOK_IF = 256, TOK_ELSE, TOK_WHILE, TOK_BREAK, TOK_CONTINUE, TOK_RETURN, TOK_INT,
      TOK_FLOAT, TOK_FOR, TOK_VOID, TOK_GE, TOK_LE, TOK_EQ,
      TOK_NEQ, TOK_INT_CONST, TOK_FLOAT_CONST, TOK_STRING_CONST, TOK_END, TOK_ID,
      TOK_L_OR, TOK_L_AND, TOK_MAX};

#ifdef USE_MEH_TYPE
typedef unsigned int uint;
typedef unsigned short ushort;
typedef unsigned char uchar;
#endif

extern void cam3_update(char *nam, float du, float dt);
extern void cam3_movetg(char *nam, float x, float y, float z);
extern void cam3_use(char *nam);
extern void cam3_add(char *nam, v3_t target, v3_t pos, float Kd, float angKd);

#endif
