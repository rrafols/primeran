#include "oldskool.h"
#include "font.h"

#define	FONTHOR	1416
#define	FONTVER 40
#define DFWIDTH	10

extern	short int FontPos;
static char	fontord[] = " a†bcdeÇfghi°jklmn§o¢pqrstu£vwxyzAµBCDEêFGHI÷JKLMN•OïPQRSTUÈVWXYZ1234567890!?.,:;'/- ";
			//≠≠≠≠nota!!!!		ï==O con Ô & '=="
int	fpal[256*3];
unsigned char	rmask[32*32];
unsigned char	rpmask[24*24];
unsigned char	bubble_a[64];

static unsigned char	colormap[5]={31,29,22,13,0};
//static unsigned char	icolormap[5]={0,13,22,29,31};

static unsigned char	colormap2[5]={15-15,15-13,15-10,15-5,15-0};
static unsigned char	colormap3[5]={15,12,8,4,0};

static void protoDrawString(int isa, int x,int y,char *fb,char *db,char *str,int size,float iangle, int cadd) {
	int	a,b,c,d;
	int	pos,rpos,wpos,xpos;
	char	alpha;
	char	*search;
	int	width;
	short 	int *fp;
	char	*tmp;
	float	ang;
	int	ay;
	int	rpos2;
	char	*cmap;

	if (isa) {
		cmap=(char *) &colormap3;
	} else {
		if(size==2) cmap=(char *) &colormap2;
		else cmap=(char *) &colormap;
	}

	fp=(short int *) &FontPos;

	ang=iangle;
	tmp=str;

	pos=640*y;
	if (isa == 0)
		xpos=x;

	while(*str!=0){
		if (isa)
			xpos=x;
		width=DFWIDTH;
		alpha=*str;	
		c=0;
		search=&fontord;
		while(alpha!=*search && c<=83){
			c++;
			search++;
		}
		if(c!=0 && c<=83){
			c--;
			rpos=fp[c];
			width=fp[c+1]-rpos;
			if (isa)
				xpos+=(24-width)/2;
			if(xpos+width<0) {
				ang+=0.05*width;
				xpos+=width;
			}else{
				for(a=0;a<width;a++){
					if(xpos>=0 && xpos<=639){
						ay=y*640;	//(y-((int)(10*(sin(ang)))))*640;//320-81
						wpos=ay+xpos;
						rpos2=rpos;
						for(b=0;b<FONTVER;b++){
							d=fb[rpos2];
							if (isa) {
								if(d) db[wpos]=cmap[d]+cadd;
							} else {
								if(d) db[wpos]=cmap[d];
							}
							wpos+=640;
							rpos2+=FONTHOR;
						}
					}
					rpos++;
					xpos++;
					ang+=0.05;
				}
			}
		} else {
			xpos+=width;
			ang+=0.05*width;
		}
		xpos+=size;
		if(xpos>640) return;
		ang+=0.05;	//disp
		str++;
	}
}

void DrawString(int x,int y,char *fb,char *db,char *str,int size,float iangle) {
	protoDrawString(0,x,y,fb,db,str,size,iangle,0);
}

void DrawStringA(int x,int y,char *fb,char *db,char *str,int size,float iangle, int cadd) {
	protoDrawString(1,x,y,fb,db,str,size,iangle,cadd);
}

void	DrawStringSemiA(int x,int y,char *fb,char *db,char *str,int size,float iangle,int cadd)
{
	int	a,b,c,d;
	int	pos,rpos,wpos,xpos;
	char	alpha;
	char	*search;
	int	width;
	short 	int *fp;
	char	*tmp;
	float	ang;
	int	ay;
	int	rpos2;
	char	oldc1,oldc2;
	char	*cmap;

	fp=(short int *) &FontPos;

	ang=iangle;
	tmp=str;

	pos=640*y;
	xpos=x;
	cmap=(char *) &colormap3;

	while(*str!=0){
		width=DFWIDTH;
		alpha=*str;	
		c=0;
		search=&fontord;
		while(alpha!=*search && c<=83){
			c++;
			search++;
		}
		if(c!=0 && c<=83){
			c--;
			rpos=fp[c];
			width=fp[c+1]-rpos;
			if(xpos+width<0) {
				ang+=0.05*width/2;
				xpos+=width/2;
			}else{
				for(a=0;a<width-1;a+=2){
					if(xpos>=0 && xpos<=639){
						ay=y*640;	//(y-((int)(10*(sin(ang)))))*640;//320-81
						wpos=ay+xpos;
						rpos2=rpos;
						for(b=0;b<FONTVER;b+=2){
							oldc1=(fb[rpos2]+fb[rpos2+1])>>1;
							oldc2=(fb[rpos2+FONTHOR]+fb[rpos2+FONTHOR+1])>>1;
							d=(oldc1+oldc2)>>1;
							if(d) db[wpos]=cmap[d]+cadd;
							wpos+=640;
							rpos2+=FONTHOR*2;
						}
					}
					rpos+=2;
					xpos++;
					ang+=0.05;
				}
			}
		} else {
			xpos+=width/2;
			ang+=0.05*width/2;
		}
		xpos+=size;
		if(xpos>640) return;
		ang+=0.05;	//disp
		str++;
	}
}


void	DrawStringSemi(int x,int y,char *fb,char *db,char *str,int size,float iangle)
{
	int	a,b,c,d;
	int	pos,rpos,wpos,xpos;
	char	alpha;
	char	*search;
	int	width;
	short 	int *fp;
	char	*tmp;
	float	ang;
	int	ay;
	int	rpos2;
	char	oldc1,oldc2;

	fp=(short int *) &FontPos;

	ang=iangle;
	tmp=str;

	pos=640*y;
	xpos=x;

	while(*str!=0){
		width=DFWIDTH;
		alpha=*str;	
		c=0;
		search=&fontord;
		while(alpha!=*search && c<=83){
			c++;
			search++;
		}
		if(c!=0 && c<=83){
			c--;
			rpos=fp[c];
			width=fp[c+1]-rpos;
			if(xpos+width<0) {
				ang+=0.05*width/2;
				xpos+=width/2;
			}else{
				for(a=0;a<width-1;a+=2){
					if(xpos>=0 && xpos<=639){
						ay=y*640;	//(y-((int)(10*(sin(ang)))))*640;//320-81
						wpos=ay+xpos;
						rpos2=rpos;
						for(b=0;b<FONTVER;b+=2){
							oldc1=(fb[rpos2]+fb[rpos2+1])>>1;
							oldc2=(fb[rpos2+FONTHOR]+fb[rpos2+FONTHOR+1])>>1;
							d=(oldc1+oldc2)>>1;
							if(d) db[wpos]=colormap[d];
							wpos+=640;
							rpos2+=FONTHOR*2;
						}
					}
					rpos+=2;
					xpos++;
					ang+=0.05;
				}
			}
		} else {
			xpos+=width/2;
			ang+=0.05*width/2;
		}
		xpos+=size;
		if(xpos>640) return;
		ang+=0.05;	//disp
		str++;
	}
}

void	DrawStringSemiSp(int x,int y,char *fb,char *db,char *str,int size,float iangle,int spacen)
{
	int	a,b,c,d;
	int	pos,rpos,wpos,xpos;
	char	alpha;
	char	*search;
	int	width;
	short 	int *fp;
	char	*tmp;
	float	ang;
	int	ay;
	int	rpos2;
	char	oldc1,oldc2;

	fp=(short int *) &FontPos;

	ang=iangle;
	tmp=str;
/*
	if(iangle==1.0) cmap=(char *) &icolormap;
	else cmap=(char *) &colormap;
*/
	pos=640*y;
	xpos=x;

	while(*str!=0 && spacen>0){
		width=DFWIDTH;
		alpha=*str;	
		c=0;
		if(alpha==' ') spacen--;
		if(alpha==0xD){
			y+=22;
			xpos=x-width/2-size;
			spacen--;			
		}

		if(alpha==0xA){
			y+=22;
			xpos=x-width/2-size;
		}

		search=&fontord;
		while(alpha!=*search && c<=83){
			c++;
			search++;
		}
		if(c!=0 && c<=83){
			c--;
			rpos=fp[c];
			width=fp[c+1]-rpos;
			if(xpos+width<0) {
				ang+=0.05*width/2;
				xpos+=width/2;
			}else{
				for(a=0;a<width-1;a+=2){
					if(xpos>=0 && xpos<=639){
						ay=y*640;	//(y-((int)(10*(sin(ang)))))*640;//320-81
						wpos=ay+xpos;
						rpos2=rpos;
						for(b=0;b<FONTVER;b+=2){
							oldc1=(fb[rpos2]+fb[rpos2+1])>>1;
							oldc2=(fb[rpos2+FONTHOR]+fb[rpos2+FONTHOR+1])>>1;
							d=(oldc1+oldc2)>>1;
							if(d) db[wpos]=colormap[d];
							wpos+=640;
							rpos2+=FONTHOR*2;
						}
					}
					rpos+=2;
					xpos++;
					ang+=0.05;
				}
			}
		} else {
			xpos+=width/2;
			ang+=0.05*width/2;
		}
		xpos+=size;
		if(xpos>640) return;
		ang+=0.05;	//disp
		str++;
	}
}

void	DrawStringSemiV(int x,int y,char *fb,char *db,char *str,int size,float iangle)
{
	int	a,b,c,d;
	int	rpos,wpos,xpos;
	char	alpha;
	char	*search;
	int	width;
	short 	int *fp;
	char	*tmp;
	float	ang;
	int	ay;
	int	rpos2;
	char	oldc1,oldc2;

	fp=(short int *) &FontPos;

	ang=iangle;
	tmp=str;

//	pos=640*y;
	xpos=y;

	while(*str!=0){
		width=DFWIDTH;
		alpha=*str;	
		c=0;
		search=&fontord;
		while(alpha!=*search && c<=83){
			c++;
			search++;
		}
		if(c!=0 && c<=83){
			c--;
			rpos=fp[c];
			width=fp[c+1]-rpos;
			if(xpos+width<0) {
				ang+=0.05*width/2;
				xpos+=width/2;
			}else{
				for(a=0;a<width-1;a+=2){
					if(xpos>=0 && xpos<=639){
						ay=x;	//(y-((int)(10*(sin(ang)))))*640;//320-81
						wpos=ay+(480-xpos)*640;
						rpos2=rpos;
						for(b=0;b<FONTVER;b+=2){
							if(b+x>0){	//clip_v
								oldc1=(fb[rpos2]+fb[rpos2+1])>>1;
								oldc2=(fb[rpos2+FONTHOR]+fb[rpos2+FONTHOR+1])>>1;
								d=(oldc1+oldc2)>>1;
								if(d) db[wpos]=colormap[d];
							}
							wpos++;
							rpos2+=FONTHOR*2;
						}
					}
					rpos+=2;
					xpos++;
					ang+=0.05;
				}
			}
		} else {
			xpos+=width/2;
			ang+=0.05*width/2;
		}
		xpos+=16;
		if(xpos>640) return;
		ang+=0.05;	//disp
		str++;
	}
}

void	DrawStringSemiB(int x,int y,char *fb,char *db,char *str,int size,float iangle)
{
	int	a,b,c,d;
	int	pos,rpos,wpos,xpos;
	char	alpha;
	char	*search;
	int	width;
	short 	int *fp;
	char	*tmp;
	float	ang;
	int	ay;
	int	rpos2;
	char	oldc1,oldc2;

	fp=(short int *) &FontPos;

	ang=iangle;
	tmp=str;

	pos=640*y;
	xpos=x;

	while(*str!=0){
		width=DFWIDTH;
		alpha=*str;	
		c=0;
		search=&fontord;
		while(alpha!=*search && c<=83){
			c++;
			search++;
		}
		if(c!=0 && c<=83){
			c--;
			rpos=fp[c];
			width=fp[c+1]-rpos;
			if(xpos+width<0) {
				ang+=0.05*width/2;
				xpos+=width/2;
			}else{
				for(a=0;a<width-1;a+=2){
					if(xpos>=0 && xpos<=639){
						ay=y*640;	//(y-((int)(10*(sin(ang)))))*640;//320-81
						wpos=ay+xpos;
						rpos2=rpos;
						for(b=0;b<FONTVER;b+=2){
							oldc1=(fb[rpos2]+fb[rpos2+1])>>1;
							oldc2=(fb[rpos2+FONTHOR]+fb[rpos2+FONTHOR+1])>>1;

							d=(oldc1+oldc2)>>1;
							if(d) d++;

							//d=(oldc1+oldc2);

							if(d>5) d=5;
							if(d) db[wpos]=colormap[d];
							wpos+=640;
							rpos2+=FONTHOR*2;
						}
					}
					rpos+=2;
					xpos++;
					ang+=0.05;
				}
			}
		} else {
			xpos+=width/2;
			ang+=0.05*width/2;
		}
		xpos++;
		if(xpos>640) return;
		ang+=0.05;	//disp
		str++;
	}
}

void	DrawStringQuarter(int x,int y,char *fb,char *db,char *str,int size,float iangle)
{
	int	a,b,c,d;
	int	pos,rpos,wpos,xpos;
	char	alpha;
	char	*search;
	int	width;
	short 	int *fp;
	char	*tmp;
	float	ang;
	int	ay;
	int	rpos2;
	char	oldc1,oldc2,oldc3;

	fp=(short int *) &FontPos;

	ang=iangle;
	tmp=str;

	pos=640*y;
	xpos=x;

	while(*str!=0){
		width=DFWIDTH;
		alpha=*str;	
		c=0;
		search=&fontord;
		while(alpha!=*search && c<=83){
			c++;
			search++;
		}
		if(c!=0 && c<=83){
			c--;
			rpos=fp[c];
			width=fp[c+1]-rpos;
			if(xpos+width<0) {
				ang+=0.05*width/2;
				xpos+=width/2;
			}else{
				for(a=0;a<width-1;a+=3){
					if(xpos>=0 && xpos<=639){
						ay=y*640;	//(y-((int)(10*(sin(ang)))))*640;//320-81
						wpos=ay+xpos;
						rpos2=rpos;
						for(b=0;b<FONTVER-1;b+=3){
							oldc1=(fb[rpos2]+fb[rpos2+1]+fb[rpos2+2])/3;
							oldc2=(fb[rpos2+FONTHOR]+fb[rpos2+FONTHOR+1]+fb[rpos2+FONTHOR+2])/3;
							oldc3=(fb[rpos2+FONTHOR*2]+fb[rpos2+FONTHOR*2+1]+fb[rpos2+FONTHOR*2+2])/3;
							d=(oldc1+oldc2+oldc3)/3;
							if(d) db[wpos]=colormap[d];
							wpos+=640;
							rpos2+=FONTHOR*3;
						}
					}
					rpos+=3;
					xpos++;
					ang+=0.05;
				}
			}
		} else {
			xpos+=width/2;
			ang+=0.05*width/2;
		}
		xpos++;
		if(xpos>640) return;
		ang+=0.05;	//disp
		str++;
	}
}

void DeCrunchFont(unsigned char *fb)
{
	unsigned char	*pf,*f;
	int	a,b;
	int	pos,c1,c2;
	unsigned char	*rm;

	pf=(unsigned char *) &FontPal;
	rm=(unsigned char *) &Curva;
	f=(unsigned char *) &CrunchedFont;
	for(a=0;a<255;a++){
		fpal[a*3  ]=pf[a*3 ];
		fpal[a*3+1]=pf[a*3+1];
		fpal[a*3+2]=pf[a*3+2];
		os_fb_set_palette(a, pf[a*3], pf[a*3+1], pf[a*3+2]);
	}
	
	pos=0;
	for(a=0;a<FONTVER;a++){
		for(b=0;b<FONTHOR/2;b++){
			c1=f[pos];
			c2=c1>>4;
			c1=c1&0xf;
			fb[pos*2]=c2;
			fb[pos*2+1]=c1;
			pos++;
		}
	}

	pos=0;
	for(a=0;a<31;a++){
		for(b=0;b<16;b++){
			c1=rm[pos];
			c2=c1>>4;
			c1&=0xf;
			rmask[pos*2  ]=0xe - c2;
			rmask[pos*2+1]=0xe - c1;
			pos++;
		}
	}
	rm=(char *) &Curvap;
	pos=0;
	for(a=0;a<24;a++){
		for(b=0;b<12;b++){
			c1=rm[pos];
			c2=c1>>4;
			c1&=0xf;
			rpmask[pos*2  ]=c2;
			rpmask[pos*2+1]=c1;
			pos++;
		}
	}
	rm=(char *) &Bubble;
	for(pos=0;pos<64;pos++) bubble_a[pos]=rm[pos];
}

