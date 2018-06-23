
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "oldskool.h"

#include "drawchar.h"
#include "3de.h"
#include "font.h"
#define PI 3.14159265f

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define SCREEN_SIZE (640*480)

#define ffabs fabs
#ifndef WIN32
#define fsin sinf
#define fcos cosf
#define fsqrt sqrtf
#else
#define fsin sin
#define fcos cos
#define fsqrt sqrt
#endif

#define fistshl(__a,__b) (*__a)=((__b)*65536.f)
#define fist(__a,__b) (*__a)=(__b)

#include "data/adntitle.i"
#include "data/pill.i"
#include "data/cajp.i"

#define ZCONS 200.0f

static float	rot[200][3];	//102

float	ymax=479.0f,ymin=0.0f;
float	xmax=639.0f,xmin=0.0f;
static float	maxyADN=0.0f;

typedef	struct vector VECTOR;
typedef	struct s_struct sStruct;

struct vector{
	float	v1;
	float	v2;
	float	v3;
};

struct s_struct{
	int		zval;
	short int	nface;
	short int	color;
};

static sStruct	sorted[204*4];

extern	int p_minx;
extern	int p_maxx;

#define ScreenBytesPerLine 640
#define ScreenBytesPerPixel 1

static void sort_pol(sStruct *min,sStruct *max) {
	sStruct t;
	int	a,b;
	int maxi = max - min;
	for(a=0;a<maxi;a++){
		for(b=0;b<maxi;b++){
			if(min[a].zval>=min[b].zval && a!=b){
				t = min[a];
				min[a] = min[b];
				min[b] = t;
			}
		}
	}
}


void LineAA_2(unsigned char *dst,float x1f,float y1f,float x2f,float y2f,unsigned char color){
	int 	pos,inc,dex,dey,xp,yp,offs;
	int 	alias,antialias;
	int 	x1,x2,y1,y2;
	int 	minx,miny,maxx,maxy;
	unsigned int	aliase;
	unsigned int	aaliase;
	unsigned int	f00,f01,f10,f11;
	int	count,c1,c2;
        float   dx,dy;
	float	m1,m2;

	c1=((x1f<xmin)<<3) | ((x1f>xmax)<<2) | ((y1f<ymin)<<1) | ((y1f>ymax)<<0);
	c2=((x2f<xmin)<<3) | ((x2f>xmax)<<2) | ((y2f<ymin)<<1) | ((y2f>ymax)<<0);
	while (c1 | c2){
		if(c1 & c2) return;
		dx=x2f-x1f;
		dy=y2f-y1f;
		m1=dy/dx;
		m2=dx/dy;

		if(c1){
			if(c1&8) {y1f+=(xmin-x1f)*m1;x1f=xmin;} else 
			if(c1&4) {y1f+=(xmax-x1f)*m1;x1f=xmax;} else 
			if(c1&2) {x1f+=(ymin-y1f)*m2;y1f=ymin;} else 
			if(c1&1) {x1f+=(ymax-y1f)*m2;y1f=ymax;}
			c1=((x1f<xmin)<<3)|((x1f>xmax)<<2)|((y1f<ymin)<<1)|((y1f>ymax));
		} else {
			if(c2&8) {y2f+=(xmin-x2f)*m1;x2f=xmin;} else 
			if(c2&4) {y2f+=(xmax-x2f)*m1;x2f=xmax;} else 
			if(c2&2) {x2f+=(ymin-y2f)*m2;y2f=ymin;} else 
			if(c2&1) {x2f+=(ymax-y2f)*m2;y2f=ymax;}
			c2=((x2f<xmin)<<3)|((x2f>xmax)<<2)|((y2f<ymin)<<1)|((y2f>ymax));
		}
	}
	if(x2f-x1f==0.0 && y2f-x1f==0.0) return;
	if(ffabs(x2f-x1f)>ffabs(y2f-y1f)){
		x1=((int) (65536.0f*x1f));
		x2=((int) (65536.0f*x2f));
		y1=((int) (65536.0f*y1f));
		y2=((int) (65536.0f*y2f));
	
		minx=x1;maxx=x2;miny=y1;maxy=y2;
		if(maxx<minx){
			minx=x2;maxx=x1;miny=y2;maxy=y1;
		}

		
		pos=miny;

		dex=(maxx-minx);
		dey=(maxy-miny);
		if((dex>>16)==0) return;

		inc=dey/(dex>>16);
		aaliase=(minx>>8)&0xff;
		aliase=0x100-aaliase;

		xp=(minx)>>16;

		count=(maxx-minx+0x7fff)>>16;
		while(count){
			yp=pos>>16;
			antialias=(pos>>8)&0xff;
			alias=0x100-antialias;
		
			f00=(alias*aliase);
			f01=(aaliase*alias);
			f10=(aliase*antialias);
			f11=(aaliase*antialias);

			offs=(yp<<9)+(yp<<7)+xp;

			dst[offs]=((f00*(color-dst[offs]))>>16)+dst[offs];
			offs++;
			dst[offs]=((f01*(color-dst[offs]))>>16)+dst[offs];
			offs+=639;
			dst[offs]=((f10*(color-dst[offs]))>>16)+dst[offs];
			offs++;
			dst[offs]=((f11*(color-dst[offs]))>>16)+dst[offs];
			pos+=inc;
			xp++;
			count--;
		}
	} else {
		x1=((int) (65536.0*x1f));
		x2=((int) (65536.0*x2f));
		y1=((int) (65536.0*y1f));
		y2=((int) (65536.0*y2f));

		minx=x1;maxx=x2;miny=y1;maxy=y2;
		if(maxy<miny){
			minx=x2;maxx=x1;miny=y2;maxy=y1;
		}

	
		pos=minx;

		dex=(maxx-minx);
		dey=(maxy-miny);
		if((dey>>16)==0) return;

		inc=dex/(dey>>16);
		aaliase=(miny>>8)&0xff;
		aliase=0x100-aaliase;

		count=(maxy-miny+0x7fff)>>16;

		yp=(miny)>>16;
		while(count){
			xp=pos>>16;

			antialias=(pos>>8)&0xff;
			alias=0x100-antialias;

			f00=(alias*aliase);
			f01=(antialias*aliase);
			f10=(alias*aaliase);
			f11=(antialias*aaliase);

			offs=(yp<<9)+(yp<<7)+xp;

			dst[offs]=((f00*(color-dst[offs]))>>16)+dst[offs];
			offs++;
			dst[offs]=((f01*(color-dst[offs]))>>16)+dst[offs];
			offs+=639;
			dst[offs]=((f10*(color-dst[offs]))>>16)+dst[offs];
			offs++;
			dst[offs]=((f11*(color-dst[offs]))>>16)+dst[offs];
			pos+=inc;
			yp++;
			count--;
		}
	}
}

static void FlatInner(int Color,int Width,int startpos,unsigned char *p) {
	p += startpos;
	while (Width > 0) {
		*p = Color;
		--Width;
		++p;
	}
}

void DrawFlatCV(float x1f,float y1f,float x2f,float y2f,float x3f,float y3f,unsigned char *dst,int Color)
{
        int     t,dx12,dx23,dx13,dxl,dxr,xpl,xpr,xa,xb,dxl2,dxr2;
        int     y2y1,y3y2,y3y1;
	int	ys;
        int     yi;

	int	tmp1;
	unsigned char	*tmp2;
        int	pos;
        int	pos2;
	int	iymin;
        int     x1,y1,x2,y2,x3,y3;

	x1=((int) (65536.0f*x1f));
	x2=((int) (65536.0f*x2f));
	x3=((int) (65536.0f*x3f));
	y1=((int) (y1f));
	y2=((int) (y2f));
	y3=((int) (y3f));

        if (y1>y2)      {t=y1; y1=y2; y2=t; t=x1; x1=x2; x2=t;}
        if (y1>y3)      {t=y1; y1=y3; y3=t; t=x1; x1=x3; x3=t;}
        if (y2>y3)      {t=y2; y2=y3; y3=t; t=x2; x2=x3; x3=t;}

	iymin=(int)(ymin);
	if(y1>ymax) return;
	if(y3<iymin) return;

        dx12=x2-x1;     dx23=x3-x2;     dx13=x3-x1;

        y2y1=y2-y1;     y3y1=y3-y1;     y3y2=y3-y2;

        if (y2y1)      {dx12/=y2y1;}
        if (y3y2)      {dx23/=y3y2;}
        if (y3y1)      {dx13/=y3y1;}

        if (dx12>dx13) {dxl=dx13;    dxr=dx12;    dxl2=dx13;   dxr2=dx23;}
        else 	       {dxl=dx12;    dxr=dx13;    dxl2=dx23;   dxr2=dx13;}

        pos=(y1<<9)+(y1<<7);
        xpr=xpl=x1;
        yi=y1;

	if(y1<iymin){
		if(y2>=iymin){
	                xpr+=dxr*(iymin-y1);
        	        xpl+=dxl*(iymin-y1);
			pos=(iymin<<9)+(iymin<<7);
			y1=iymin;
		} else {
		        if(dx12>dx13){
        		        xpr=x2;
				xpl+=dxl*(y2-y1);
		        } else {
        		        xpl=x2;
				xpr+=dxr*(y2-y1);
		        }
			y1=y2;
		}
	}

	if(y2>=iymin){
		if(y2<ymax) ys=y2;
		else ys=ymax;

	        for (;y1<ys;y1++)
        	{
	                xb=(xpr>>16);
                	xa=(xpl>>16);
			pos2=pos+xa;
			tmp1=xb-xa;
			tmp2=(unsigned char *) (dst+pos2);
			FlatInner(Color,tmp1,pos+xa,dst);
			xpl+=dxl;
        	        xpr+=dxr;
	                pos+=640;
        	}
	        if(dx12>dx13){
        	        xpr=x2;
	        } else {
        	        xpl=x2;
	        }
		if(y3<ymax) ys=y3;
		else ys=ymax;

	        for (;y1<ys;y1++)
        	{
	                xb=(xpr>>16);
                	xa=(xpl>>16);
			pos2=pos+xa;
			tmp1=xb-xa;
			tmp2=(unsigned char *) (dst+pos2);
			FlatInner(Color,tmp1,pos+xa,dst);
			xpl+=dxl2;
        	        xpr+=dxr2;
			pos+=640;
        	}
	} else {
       	        xpr+=dxr2*(iymin-y2);
               	xpl+=dxl2*(iymin-y2);
		pos=(iymin<<9)+(iymin<<7);
       	        y1=iymin;

		if(y3<ymax) ys=y3;
		else ys=ymax;

	        for (;y1<ys;y1++)
        	{
                	xb=(xpr>>16);
	                xa=(xpl>>16);
        	       	pos2=pos+xa;
			tmp1=xb-xa;
			tmp2=(unsigned char *) (dst+pos2);
			FlatInner(Color,tmp1,pos+xa,dst);
	                xpl+=dxl2;
        	        xpr+=dxr2;
	                pos+=640;
        	}
	}
}

void	InitADN(){
	maxyADN=100.0;
}

void	RenderADN(char *dst,char color,float angle,float FOV,int dx,int dy){
	int	a,idx1,idx2,idx3;
	float	nx,ny,nz;
	float	px1,px2,px3,py1,py2,py3;
	float	sa,ca;
	float	con = 0.0f;
	int	fc = 0;

	ymax=479;
	ymin=0;

	sa=fsin(angle);
	ca=fcos(angle);

	for(a=0;a<104;a++){
		nx=ADN_vertex[a][1];
		ny=ADN_vertex[a][2];
		nz=ADN_vertex[a][0];
		rot[a][0]=nx*ca+nz*sa;
		rot[a][1]=ny;
		rot[a][2]=-nx*sa+nz*ca;
	}

	for(a=0;a<168*3;a+=2){
		if(a<168) 	      {con=-maxyADN;fc=a;}
		if(a>=168 && a<168*2) {con=0.0;fc=a-168;}
		if(a>=168*2) 	      {con=maxyADN;fc=a-168*2;}

		idx1=ADN_faces[fc][0];
		idx2=ADN_faces[fc][1];
		idx3=ADN_faces[fc][2];

		if(rot[idx1][2]!=0.0f){
			px1=(FOV*rot[idx1][0]/(rot[idx1][2]+ZCONS))+dx;
			py1=(FOV*(rot[idx1][1]+con)/(rot[idx1][2]+ZCONS))+dy;
		}else{  px1=dx;	py1=dy;	}

		if(rot[idx2][2]!=0.0f){
			px2=(FOV*rot[idx2][0]/(rot[idx2][2]+ZCONS))+dx;
			py2=(FOV*(rot[idx2][1]+con)/(rot[idx2][2]+ZCONS))+dy;
		}else{	px2=dx;	py2=dy;}

		if(rot[idx3][2]!=0.0f){
			px3=(FOV*rot[idx3][0]/(rot[idx3][2]+ZCONS))+dx;
			py3=(FOV*(rot[idx3][1]+con)/(rot[idx3][2]+ZCONS))+dy;
		}else{	px3=dx;	py3=dy;}

		LineAA_2(dst,px1,py1,px2,py2,color);
		LineAA_2(dst,px1,py1,px3,py3,color);
		LineAA_2(dst,px2,py2,px3,py3,color);
	}
}

void	RenderADNHor(char *dst,char color,float angle,float FOV,int dx,int dy){
	int	a,idx1,idx2,idx3;
	float	nx,ny,nz;
	float	px1,px2,px3,py1,py2,py3;
	float	sa,ca;
	float	con = 0.0f;
	int	fc = 0;

	sa=fsin(angle);
	ca=fcos(angle);

	xmin=49.0f;
	xmax=590.0f;

	for(a=0;a<104;a++){
		nx=ADN_vertex[a][2];
		ny=ADN_vertex[a][1];
		nz=ADN_vertex[a][0];
		rot[a][0]=nx;
		rot[a][1]=ny*ca+nz*sa;
		rot[a][2]=-ny*sa+nz*ca;
	}

	for(a=0;a<168*3;a+=2){
		if(a<168) 	      {con=-100.0f;fc=a;}
		if(a>=168 && a<168*2) {con=0.0f;fc=a-168;}
		if(a>=168*2) 	      {con=100.0f;fc=a-168*2;}

		idx1=ADN_faces[fc][0];
		idx2=ADN_faces[fc][1];
		idx3=ADN_faces[fc][2];

		if(rot[idx1][2]!=0.0f){
			px1=(FOV*(rot[idx1][0]+con)/(rot[idx1][2]+ZCONS))+dx;
			py1=(FOV*(rot[idx1][1])/(rot[idx1][2]+ZCONS))+dy;
		}else{  px1=dx;	py1=dy;	}

		if(rot[idx2][2]!=0.0f){
			px2=(FOV*(rot[idx2][0]+con)/(rot[idx2][2]+ZCONS))+dx;
			py2=(FOV*(rot[idx2][1])/(rot[idx2][2]+ZCONS))+dy;
		}else{	px2=dx;	py2=dy;}

		if(rot[idx3][2]!=0.0f){
			px3=(FOV*(rot[idx3][0]+con)/(rot[idx3][2]+ZCONS))+dx;
			py3=(FOV*(rot[idx3][1])/(rot[idx3][2]+ZCONS))+dy;
		}else{	px3=dx;	py3=dy;}

		LineAA_2(dst,px1,py1,px2,py2,color);
		LineAA_2(dst,px1,py1,px3,py3,color);
		LineAA_2(dst,px2,py2,px3,py3,color);
	}

	xmin=0;
	xmax=639;
}

void	RenderADNFlat(char *dst,float angle,float FOV,int dx,int dy){
	int	a,idx1,idx2,idx3;
	float	nx,ny,nz;
	float	px1,px2,px3,py1,py2,py3;
	float	sa,ca;
	VECTOR	ve1,ve2;
	VECTOR	Normal;
	char	col;
	int	maxi,c;

	ymax=450.0f;
	ymin=310.0f;


	sa=fsin(angle);
	ca=fcos(angle);

	for(a=0;a<104;a++){
		nx=ADN_vertex[a][1];
		ny=ADN_vertex[a][2];
		nz=ADN_vertex[a][0];
		rot[a][0]=nx*ca+nz*sa;
		rot[a][1]=ny;
		rot[a][2]=-nx*sa+nz*ca;
	}

	sa=fsin(-PI/4.0f);
	ca=fcos(-PI/4.0f);

	for(a=0;a<104;a++){
		nx=rot[a][0];
		ny=rot[a][1];
		nz=rot[a][2];
		rot[a][0]=nx*ca+ny*sa;
		rot[a][1]=-nx*sa+ny*ca;
		rot[a][2]=nz;
	}

	maxi=0;

	for(a=0;a<168;a++){
		idx1=ADN_faces[a][0];
		idx2=ADN_faces[a][1];
		idx3=ADN_faces[a][2];

		ve1.v1=rot[idx1][0]-rot[idx2][0];
		ve1.v2=rot[idx1][1]-rot[idx2][1];
		ve1.v3=rot[idx1][2]-rot[idx2][2];

		ve2.v1=rot[idx3][0]-rot[idx2][0];
		ve2.v2=rot[idx3][1]-rot[idx2][1];
		ve2.v3=rot[idx3][2]-rot[idx2][2];

		Normal.v3=(ve1.v1*ve2.v2)-(ve1.v2*ve2.v1);
		if(a<=80) Normal.v3=-Normal.v3;

		if(Normal.v3>0.0f){
			Normal.v1=(ve1.v2*ve2.v3)-(ve1.v3*ve2.v2);
			Normal.v2=(ve1.v3*ve2.v1)-(ve1.v1*ve2.v3);
			sa=fsqrt(Normal.v1*Normal.v1+Normal.v2*Normal.v2+Normal.v3*Normal.v3);
			Normal.v3/=sa;

			col=(int)((31.0f*Normal.v3));

			ca=rot[idx1][2];
			if(ca>rot[idx2][2]) ca=rot[idx2][2];
			if(ca>rot[idx3][2]) ca=rot[idx3][2];

			sorted[maxi].zval=(int) (65536.0f*ca);
			sorted[maxi].nface=a;
			sorted[maxi].color=col;

			maxi++;
		}
	}

	sort_pol(&sorted[0],&sorted[maxi-1]);

	for(a=0;a<maxi;a++){
		c=sorted[a].nface;
		idx1=ADN_faces[c][0];
		idx2=ADN_faces[c][1];
		idx3=ADN_faces[c][2];

		if(rot[idx1][2]!=0.0f){
			px1=(FOV*rot[idx1][0]/(rot[idx1][2]+ZCONS))+dx;
			py1=(FOV*(rot[idx1][1])/(rot[idx1][2]+ZCONS))+dy;
		}else{  px1=dx;	py1=dy;	}

		if(rot[idx2][2]!=0.0f){
			px2=(FOV*rot[idx2][0]/(rot[idx2][2]+ZCONS))+dx;
			py2=(FOV*(rot[idx2][1])/(rot[idx2][2]+ZCONS))+dy;
		}else{	px2=dx;	py2=dy;}

		if(rot[idx3][2]!=0.0f){
			px3=(FOV*rot[idx3][0]/(rot[idx3][2]+ZCONS))+dx;
			py3=(FOV*(rot[idx3][1])/(rot[idx3][2]+ZCONS))+dy;
		}else{	px3=dx;	py3=dy;}

		DrawFlatCV(px1,py1,px2,py2,px3,py3,dst,sorted[a].color);
	}
}


void	RenderPill(char *dst,char color,float anglex,float angley,float anglez,float FOV,int dx,float dy,int m,int wired){
	int	a,c,idx1,idx2,idx3;
	float	nx,ny,nz;
	float	px1,px2,px3,py1,py2,py3;
	float	sa,ca;
	VECTOR	ve1,ve2;
	VECTOR	Normal;
	char	col;
	int	maxi;

	p_minx=640.0f;
	p_maxx=0.0f;

	if(m) ymin=40.0f+10.0f;
	else ymin=0.0f;

	if(wired) ymax=479.0f;
	else ymax=440.0f-10.0f;

	sa=fsin(angley);
	ca=fcos(angley);

	for(a=0;a<102;a++){
		nx=Aspirina_vertex[a][0];
		ny=Aspirina_vertex[a][1];
		nz=Aspirina_vertex[a][2];
		rot[a][0]=nx*ca+nz*sa;
		rot[a][1]=ny;
		rot[a][2]=-nx*sa+nz*ca;
	}

	sa=fsin(anglex);
	ca=fcos(anglex);

	for(a=0;a<102;a++){
		nx=rot[a][0];
		ny=rot[a][1];
		nz=rot[a][2];
		rot[a][0]=nx;
		rot[a][1]=ny*ca+nz*sa;
		rot[a][2]=-ny*sa+nz*ca;
	}

	sa=fsin(anglez);
	ca=fcos(anglez);

	for(a=0;a<102;a++){
		nx=rot[a][0];
		ny=rot[a][1];
		nz=rot[a][2];
		rot[a][0]=nx*ca+ny*sa;
		rot[a][1]=-nx*sa+ny*ca;
		rot[a][2]=nz;
	}

	maxi=0;
	for(a=0;a<204;a++){
		idx1=Aspirina_faces[a][0];
		idx2=Aspirina_faces[a][1];
		idx3=Aspirina_faces[a][2];

		ve1.v1=rot[idx2][0]-rot[idx1][0];
		ve1.v2=rot[idx2][1]-rot[idx1][1];
		ve1.v3=rot[idx2][2]-rot[idx1][2];

		ve2.v1=rot[idx3][0]-rot[idx1][0];
		ve2.v2=rot[idx3][1]-rot[idx1][1];
		ve2.v3=rot[idx3][2]-rot[idx1][2];

		Normal.v3=(ve1.v1*ve2.v2)-(ve1.v2*ve2.v1);

		if(Normal.v3>0.0f || wired==1){
			Normal.v1=(ve1.v2*ve2.v3)-(ve1.v3*ve2.v2);
			Normal.v2=(ve1.v3*ve2.v1)-(ve1.v1*ve2.v3);
			sa=fsqrt(Normal.v1*Normal.v1+Normal.v2*Normal.v2+Normal.v3*Normal.v3);
			if(sa!=0.0f) Normal.v3/=sa;

			col=(int)((31.0f*Normal.v3));
			ca=rot[idx1][2]+rot[idx2][2]+rot[idx3][2];

			sorted[maxi].zval=(int) (65536.0f*ca);
			sorted[maxi].nface=a;
			sorted[maxi].color=col;
			maxi++;
		}
	}

	if(wired==0)
		sort_pol(&sorted[0],&sorted[maxi-1]);

	for(a=0;a<maxi;a++){
		c=sorted[a].nface;
		idx1=Aspirina_faces[c][0];
		idx2=Aspirina_faces[c][1];
		idx3=Aspirina_faces[c][2];

		if(rot[idx1][2]!=0.0f){
			px1=(FOV*rot[idx1][0]/(rot[idx1][2]+ZCONS))+dx;
			py1=(FOV*rot[idx1][1]/(rot[idx1][2]+ZCONS))+dy;
		}else{  px1=dx;	py1=dy;	}

		if(rot[idx2][2]!=0.0f){
			px2=(FOV*rot[idx2][0]/(rot[idx2][2]+ZCONS))+dx;
			py2=(FOV*rot[idx2][1]/(rot[idx2][2]+ZCONS))+dy;
		}else{	px2=dx;	py2=dy;}

		if(rot[idx3][2]!=0.0f){
			px3=(FOV*rot[idx3][0]/(rot[idx3][2]+ZCONS))+dx;
			py3=(FOV*rot[idx3][1]/(rot[idx3][2]+ZCONS))+dy;
		}else{	px3=dx;	py3=dy;}

		if(px1>p_maxx) p_maxx=px1;
		if(px2>p_maxx) p_maxx=px2;
		if(px3>p_maxx) p_maxx=px3;
		if(px1<p_minx) p_minx=px1;
		if(px2<p_minx) p_minx=px2;
		if(px3<p_minx) p_minx=px3;

		if(wired==1){
			LineAA_2(dst,px1,py1,px2,py2,color);
			LineAA_2(dst,px1,py1,px3,py3,color);
			LineAA_2(dst,px2,py2,px3,py3,color);
		} else {
			DrawFlatCV(px1,py1,px2,py2,px3,py3,dst,sorted[a].color);
			//EsborrarHeap();
		}
	}
}

static VECTOR	ve1c,ve2c;
static VECTOR	Normalc;

void	RenderCapsule(char *dst,char color,float anglex,float angley,float anglez,float FOV,int dx,float dy,int wired){
	int	a,c,idx1,idx2,idx3;
	float	nx,ny,nz;
	float	px1,px2,px3,py1,py2,py3;
	float	sa,ca;
	char	col;
	int	maxi;
	if(wired!=2) ymax=430.0f;	//440-10;
	else ymax=479.0f;
	if(!wired) ymin=200.0f;		//40+10;
	else ymin=0.0f;

	if(wired==2) ymin=0.0f;
	sa=fsin(angley);
	ca=fcos(angley);

	for(a=0;a<74;a++){
		nx=Capsule_vertex[a][0];
		ny=Capsule_vertex[a][1];
		nz=Capsule_vertex[a][2];
		rot[a][0]=nx*ca+nz*sa;
		rot[a][1]=ny;
		rot[a][2]=-nx*sa+nz*ca;
	}

	sa=fsin(anglex);
	ca=fcos(anglex);

	for(a=0;a<74;a++){
		nx=rot[a][0];
		ny=rot[a][1];
		nz=rot[a][2];
		rot[a][0]=nx;
		rot[a][1]=ny*ca+nz*sa;
		rot[a][2]=-ny*sa+nz*ca;
	}

	sa=fsin(anglez);
	ca=fcos(anglez);

	for(a=0;a<74;a++){
		nx=rot[a][0];
		ny=rot[a][1];
		nz=rot[a][2];
		rot[a][0]=nx*ca+ny*sa;
		rot[a][1]=-nx*sa+ny*ca;
		rot[a][2]=nz;
	}

	maxi=0;
	for(a=0;a<144;a++){
		idx1=Capsule_faces[a][0];
		idx2=Capsule_faces[a][1];
		idx3=Capsule_faces[a][2];

		ve1c.v1=rot[idx2][0]-rot[idx1][0];
		ve1c.v2=rot[idx2][1]-rot[idx1][1];
		ve1c.v3=rot[idx2][2]-rot[idx1][2];

		ve2c.v1=rot[idx3][0]-rot[idx1][0];
		ve2c.v2=rot[idx3][1]-rot[idx1][1];
		ve2c.v3=rot[idx3][2]-rot[idx1][2];

		Normalc.v3=(ve1c.v1*ve2c.v2)-(ve1c.v2*ve2c.v1);

		if(Normalc.v3>0.0f || (wired==1 && (a%2==0))){
			Normalc.v1=(ve1c.v2*ve2c.v3)-(ve1c.v3*ve2c.v2);
			Normalc.v2=(ve1c.v3*ve2c.v1)-(ve1c.v1*ve2c.v3);
			sa=fsqrt(Normalc.v1*Normalc.v1+Normalc.v2*Normalc.v2+Normalc.v3*Normalc.v3);
			if(sa!=0.0f) Normalc.v3/=sa;

			col=(int)((31.0f*Normalc.v3));
			ca=rot[idx1][2]+rot[idx2][2]+rot[idx3][2];

			sorted[maxi].zval=(int) (65536.0f*ca);
			sorted[maxi].nface=a;
			sorted[maxi].color=col;

			maxi++;
		}
	}

	if(wired!=1) sort_pol(&sorted[0],&sorted[maxi-1]);

	for(a=0;a<maxi;a++){
		c=sorted[a].nface;
		idx1=Capsule_faces[c][0];
		idx2=Capsule_faces[c][1];
		idx3=Capsule_faces[c][2];

		if((rot[idx1][2]+ZCONS)!=0.0f){
			px1=(FOV*rot[idx1][0]/(rot[idx1][2]+ZCONS))+dx;
			py1=(FOV*rot[idx1][1]/(rot[idx1][2]+ZCONS))+dy;
		}else{  px1=dx;	py1=dy;	}

		if((rot[idx2][2]+ZCONS)!=0.0f){
			px2=(FOV*rot[idx2][0]/(rot[idx2][2]+ZCONS))+dx;
			py2=(FOV*rot[idx2][1]/(rot[idx2][2]+ZCONS))+dy;
		}else{	px2=dx;	py2=dy;}

		if((rot[idx3][2]+ZCONS)!=0.0f){
			px3=(FOV*rot[idx3][0]/(rot[idx3][2]+ZCONS))+dx;
			py3=(FOV*rot[idx3][1]/(rot[idx3][2]+ZCONS))+dy;
		}else{	px3=dx;	py3=dy;}

		if(wired==1){
			LineAA_2(dst,px1,py1,px2,py2,color);
			LineAA_2(dst,px1,py1,px3,py3,color);
			LineAA_2(dst,px2,py2,px3,py3,color);
		} else {
			if(Capsule_vertex[idx3][0]>0.0f || c==129)
				DrawFlatCV(px1,py1,px2,py2,px3,py3,dst,sorted[a].color);
			else	DrawFlatCV(px1,py1,px2,py2,px3,py3,dst,sorted[a].color+31);
		}
	}
}

