
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "oldskool.h"

#include "drawchar.h"
#include "3de.h"
#include "font.h"
#define PI 3.141592f

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

extern primperan_exit(int val);
extern int oldskool;
extern unsigned char *FontBuffer;
extern int  timer_count;
extern unsigned char rmask[];
extern unsigned char rpmask[];
extern char bubble_a[64];
extern int	ymin,ymax;
extern char nimg;

static unsigned char	*wrkbuf,*SBuffer2;
static int	timer_start;

#define SUI_RAND_MAX 32768

int	p_minx;
int	p_maxx;
static int	cclip=320;

#define Order (os_mod_get_order())
#define Row (os_mod_get_row())

static int oldseed=6;
static int suiRand();
static float disp (float v, float r) {
	float val=0;

	val=suiRand();
	val/=(float) SUI_RAND_MAX;
	val*=(suiRand() < SUI_RAND_MAX/2) ? -1.0f : 1.0f;
	return (v+r*val);
}

static int suiRand () {
	int a;
	a = oldseed * 0x3ec1f;
	a += 7;
	oldseed = a;
	a = a >> 16;
	a &= 0x7fff;
	return a;
}

static char		*_CREDITS_oldskool[]={
"SOLUCI•N COMPRIMIDOS............................BP mg",
"Excipientes:Etilcelulosa;c.s.p. .........1 comprimido",
"",
"SOLUCI•N VISUAL Y ORAL.............TRACE ml",
"Excipientes:Ciclamato s¢dico, c.s.p. ........100 ml",
"",
"SOLUCI•N AUDITIVA.......................................SML ml",
"Excipientes:Cloruro s¢dico; c.s.p. ...................48 kb"};
static char		*_CREDITS_newskool[]={
"CODED TABLETS..............................................BP kb",
"Excipients: fuzziochloride.............................1 kb",
"",
"VISUAL AND ORAL SYRUP.....................TRACE kb",
"Excipients: threepixybenzamide..............100 kb",
"",
"AUDITIVE AND ORAL SOLUTION.........SML kb",
"Excipients: fuzzioxypropate...........................2 pills"};

static char* INDICACIONES_newskool="Symptomatic treatment of quakers and vomits.\r\
Digestive overturns brought about lamers and\r\
antineoplastic chemicaldemotherapy. It is\r\
specially recommended in_the polimorf demopathy,\r\
vesselmotor rinitis, dezign_allergy and_yellow_fever.\r\
For the relief of upper gastro_quakestinal symptoms\r\
including heartburn, nausea and_vomiting in_flatulent\r\
dyspepsia, where no_addiction is_demonstrable.";

static char* INDICACIONES_oldskool="Tratamiento sintom tico de n useas y v¢mitos.\r\
Trastornos funcionales de la motilidad digestiva.\r\
Preparaci¢n: exploraciones del tubo digestivo.\r\
Trastornos digestivos provocados por radioterapia,\r\
cobaltoterap¡a y quimioterap¡a antineopl¢stica.\r\
Esta especialmente indicado en el eritema polimorfo,\r\
rinitis vasomotora, alergia medicamentosa, fiebre\r\
del heno, reacciones s‚ricas y urticaria.";

static char	*Posologia_txt_oldskool="ADULTOS:\r\n\
      La_dosis usual es_de 64k\r\
      cada 10_minutos.\r\n\
NI¥OS:\r\n\
      32:40kbytes al_d¡a. La\r\
      dosis_m xima diaria no\r\
      debe exceder de_64k.\r\n\
      Lactantes y ni¤os peque¤os\r\
      (de menos_de 10 kg): Media\r\
      o_sesion entera de Primperan\r\
      soluci¢n_cada 2_d¡as.";

static char	*Posologia_txt_newkskool="ADULTS:\r\n\
      Two_64k medicine measurefuls\r\
      three_times hourly.\r\n\
CHILDREN:\r\n\
      Half to_one 64k\r\
      medicine measureful two\r\
      times daily as_required.\r\
      Sucklers and children (0_to_10_kg):\r\
      Half to one 5_ml spoonful\r\
      of_syrup 20_to_30 minutes before\r\
      the_barium_meal or_intubation.";

static char	*contra_txt_newskool="Since commercialism effectively blocks\r\
the_action of the_scene\r\
the_administration should_be avoided.";

static char	*precau_txt_newskool="This solution contains design.\r\
The use of_this component in_some\r\
spanish sceners is_considered unsafe\r\
as intolerancy has_been demonstrated\r\
in party studies.\r\
Antiparkinsonian medicines of\r\
the anticholinergic type may\r\
be_used to counteract these\r\
reactions.";

static char	*contra_txt_oldskool="Hipersensibilidad individual a_la escena.\r\
Pacientes_con probada disquinesia\r\
tard¡a a neurol‚pticos.";

static char	*precau_txt_oldskool="Esta solucion contiene dise¤o.\r\
Se_han descrito casos de intolerancia\r\
a_este componente en sceners\r\
espa¤oles. Aunque_la cantidad no_es \r\
probablemente, suficiente para\r\
desencadenar los sintomas\r\
de_la intolerancia, en_caso de_que\r\
aparecieran problemas consultar a_su\r\
M‚dico.";

static char	*greets_newskool="ANACONDYL 500. for anal_problems\r\
COLLAPSIROL. for the_headache\r\
RGBADRYL. for_the_burns\r\
THREEPIXOL. for the drowsiness\r\
HANSATINE. for the_psychic_state\r\
STRAVAGANZEROL. for the_cough\r\
NETWORKASAN. for throat pains\r\
OZONID_FORTE. for the_schizophrenia\r\
UNKNOWN_COMPLEX. for the hearing\r\
CONCEPTEREL. for hyperactivity\r\
TLOTBINOL. for the old_age";

static char	*greets_oldskool="ANACONDIL 500. para la_somnolencia\r\
CENTOSIROL. para la_vejez\r\
EKLIPSEDRYL. para_quemaduras\r\
GENESIXOL. para dolores de_garganta\r\
INCOGNITIN. para problemas_anales\r\
INSIDEROL. para la_hiperactividad\r\
NETWORKASAN. para el_estado an¡mico\r\
OZONERID. para la_esquizofr‚nia\r\
TCM COMPLEX. para la_tos\r\
WAVEMASTEREL. para la_migra¤a\r\
WILDBITDRINOL. para el_oido";

static char	*secundarios_newskool="Primperan_has_given rise_to_a reasonably low_incidence\r\
of side_effects, with_absence of_serious toxicity_and\r\
excellent tolerance by_oral and_parenteral routes.\r\
Although uncommon, drowsiness and dystonic reactions\r\
have_been reported in_elite sceners subjects to_long\r\
treatmens. These are reversible, usually disappearing\r\
within 24_hours of non_stopping party_attendance.";

static char	*secundarios_oldskool="Excitacion, trastornos g stricos,\r\
trastornos del sue¤o, posibles cefaleas\r\
y reacciones cut neas de hipersensibilidad.\r\
Si se sobrepasan las dosis recomendadas en\r\
ni¤os pueden originarse reacciones extrapiramidales.\r\
Metahemoglobinemia en reci‚n nacidos.\r\
Somnolencia en elite sceners sometidos\r\
a tratamientos prolongados.";

#define credits (oldskool ? _CREDITS_oldskool : _CREDITS_newskool)

static void Cls2(int *where, int n, int value) { 
	int low = value & 0xff;
	memset(where, low | (low << 8) | (low << 16) | (low << 24), n*4);
}

static void	DrawFCube(int x1,int y1,int x2,int y2,char color,char *dst){
	int	a,b;
	char	c;
	int	offs;

	c=color+247;
	for(a=y1;a<y2;a++){
		offs=a*640+x1;
		for(b=x1;b<x2;b++){dst[offs]=c;offs++;}
	}
}

static void	DrawFRCube(int x1,int y1,int x2,int y2,char color,char *dst){
	int		a,b;
	unsigned char	c,d;
	int		offs;
	int		coffs;

	c=color*16+64;
	for(a=y1;a<y1+31;a++){
		offs=a*640+x1;
		coffs=(a-y1)*32+30;

		for(b=x1;b<x1+31;b++){
			d=(rmask[coffs]);
			if(d!=0xe) dst[offs]=d+c;
			offs++;
			coffs--;
		}

		for(b=x1+31;b<x2-31;b++) {dst[offs]=c;offs++;}

		coffs=(a-y1)*32;
		for(b=x2-31;b<x2;b++){
			d=(rmask[coffs]);
			if(d!=0xe) dst[offs]=d+c;
			offs++;
			coffs++;
		}
	}

	for(a=y1+31;a<y2-31;a++){
		offs=a*640+x1;
		for(b=x1;b<x2;b++){dst[offs]=c;offs++;}
	}
	for(a=y2-31;a<y2;a++){
		offs=a*640+x1;
		coffs=(30-(a-(y2-31)))*32+30;

		for(b=x1;b<x1+31;b++){
			d=(rmask[coffs]);
			if(d!=0xe) dst[offs]=d+c;
			offs++;
			coffs--;
		}

		for(b=x1+31;b<x2-31;b++){dst[offs]=c;offs++;}

		coffs=(30-(a-(y2-31)))*32;
		for(b=x2-31;b<x2;b++){
			d=(rmask[coffs]);
			if(d!=0xe) dst[offs]=d+c;
			offs++;
			coffs++;
		}
	}

}

static void	DrawCircle(int x1,int y1,unsigned char color,unsigned short int mask,unsigned char add,unsigned char *dst){
	int		a,b;
	unsigned char	c,d = 0;
	int		offs;
	int		coffs;
	unsigned short	int	wmask;
	unsigned char		cmax;

	wmask=mask&0x000f;

	c=color*16+64;
	for(a=y1;a<=y1+31;a++){	//= jurl
		offs=a*640+x1;
		coffs=(a-y1)*32+30;

		if(wmask==2){
			for(b=0;b<31;b++){
				d=(rmask[coffs]);
				if(d!=0xe) {
					d+=add;
					if(d>0xf) d=0xf;
					dst[offs]=d+c;
				}
				offs++;
				coffs--;
			}
		}
		if(wmask==4){
			cmax=d+add;
			if(cmax>0xf) cmax=0xf;
			for(b=0;b<31;b++){
				dst[offs]=cmax;
				offs++;
			}
		}
	}

	wmask=(mask>>4)&0x000f;

	for(a=y1;a<y1+31;a++){
		offs=a*640+x1+31;

		coffs=(a-y1)*32;

		if(wmask==2){
			for(b=0;b<31;b++){
				d=(rmask[coffs]);
				if(d!=0xe) {
					d+=add;
					if(d>0xf) d=0xf;
					dst[offs]=d+c;
				}
				offs++;
				coffs++;
			}
		}

		if(wmask==4){
			cmax=d+add;
			if(cmax>0xf) cmax=0xf;
			for(b=0;b<31;b++){
				dst[offs]=cmax;
				offs++;
				coffs++;
			}
		}
	}

	wmask=(mask>>8)&0x000f;

	for(a=y1+31;a<y1+62;a++){
		offs=a*640+x1;
		coffs=(30-(a-(y1+31)))*32+30;

		if(wmask==2){
			for(b=x1;b<x1+31;b++){
				d=(rmask[coffs]);
				if(d!=0xe) {
					d+=add;
					if(d>0xf) d=0xf;
					dst[offs]=d+c;
				}
				offs++;
				coffs--;
			}
		}
		if(wmask==4){
			cmax=d+add;
			if(cmax>0xf) cmax=0xf;
			for(b=x1;b<x1+31;b++){
				dst[offs]=cmax;
				offs++;
			}
		}
	}

	wmask=(mask>>12)&0x000f;
	for(a=y1+31;a<y1+62;a++){
		coffs=(30-(a-(y1+31)))*32;
		offs=a*640+x1+31;

		if(wmask==2){
			for(b=0;b<31;b++){
				d=(rmask[coffs]);
				if(d!=0xe) {
					d+=add;
					if(d>0xf) d=0xf;
					dst[offs]=d+c;
				}
				offs++;
				coffs++;
			}
		}
		if(wmask==4){
			cmax=d+add;
			if(cmax>0xf) cmax=0xf;
			for(b=0;b<31;b++){
				dst[offs]=cmax;
				offs++;
			}
		}
	}
}

static void	DrawCircleL(int x1,int y1,unsigned char color,unsigned short int mask,unsigned char add,unsigned char *dst){
	int		a,b;
	unsigned char	c,d;
	int		offs;
	int		coffs;
	int	pcy;

	c=color*16+64;
	pcy=y1*640;
	for(a=y1;a<y1+31;a+=2){
		offs=pcy+x1;	//((a-y1)/2+y1)*640+x1;
		coffs=(a-y1)*32+30;

		for(b=0;b<31;b+=2){
			d=(rmask[coffs]);
			if(d!=0xe) {
				d+=add;
				if(d>0xf) d=0xf;
				dst[offs]=d+c;
			}
			offs++;
			coffs-=2;
		}
		offs=pcy+x1+15;	//((a-y1)/2+y1)*640+x1+15;

		coffs=(a-y1)*32;

		for(b=0;b<31;b+=2){
			d=(rmask[coffs]);
			if(d!=0xe) {
				d+=add;
				if(d>0xf) d=0xf;
				dst[offs]=d+c;
			}
			offs++;
			coffs+=2;
		}
		pcy+=640;
	}

	for(a=y1+31;a<y1+62;a+=2){
		offs=pcy+x1; 	//((a-(y1+31))/2+y1+31)*640+x1;
		coffs=(30-(a-(y1+31)))*32+30;

		for(b=x1;b<x1+31;b+=2){
			d=(rmask[coffs]);
			if(d!=0xe) {
				d+=add;
				if(d>0xf) d=0xf;
				dst[offs]=d+c;
			}
			offs++;
			coffs-=2;
		}

		coffs=(30-(a-(y1+31)))*32;
		offs=pcy+x1+15;	//((a-(y1+31))/2+y1+31)*640+x1+15;


		for(b=0;b<31;b+=2){
			d=(rmask[coffs]);
			if(d!=0xe) {
				d+=add;
				if(d>0xf) d=0xf;
				dst[offs]=d+c;
			}
			offs++;
			coffs+=2;
		}
		pcy+=640;
	}
}

static void	DrawCircleT(int x1,int y1,unsigned char color,unsigned char *dst){
	int		a,b;
	unsigned char	c,d;
	int		offs;
	int		coffs;

	c=color*16+64;
	for(a=y1;a<y1+24;a++){
		offs=a*640+x1;
		coffs=(a-y1)*24+23;

		for(b=x1;b<x1+23;b++){
			d=(rpmask[coffs]);
			if(d) dst[offs]=c+d;
			offs++;
			coffs--;
		}

		coffs=(a-y1)*24;

		for(b=x1;b<x1+23;b++){
			d=(rpmask[coffs]);
			if(d) dst[offs]=c+d;
			offs++;
			coffs++;
		}
	}

	for(a=y1+24;a<y1+47;a++){
		offs=a*640+x1;
		coffs=(22-(a-(y1+24)))*24+23;

		for(b=x1;b<x1+23;b++){
			d=(rpmask[coffs]);
			if(d) dst[offs]=c+d;
			offs++;
			coffs--;
		}

		coffs=(22-(a-(y1+24)))*24+1;
		for(b=x1;b<x1+23;b++){
			d=(rpmask[coffs]);
			if(d) dst[offs]=c+d;
			offs++;
			coffs++;
		}
	}
}

static void	DrawCircleTL(int x1,int y1,unsigned char color,unsigned char *dst){
	int		a,b;
	unsigned char	c,d;
	int		offs;
	int		coffs;
	int		pcy;

	c=color*16+64;
	pcy=y1*640;
	for(a=y1;a<y1+24;a+=2){
		offs=pcy+x1;	//((a-y1)/2+y1)*640+x1;
		coffs=(a-y1)*24+23;

		for(b=x1;b<x1+23;b+=2){
			d=(rpmask[coffs]);
			if(d) dst[offs]=c+d;
			offs++;
			coffs-=2;
		}

		coffs=(a-y1)*24;
		offs--;

		for(b=x1;b<x1+23;b+=2){
			d=(rpmask[coffs]);
			if(d) dst[offs]=c+d;
			offs++;
			coffs+=2;
		}
		pcy+=640;
	}

	for(a=y1+24;a<y1+47;a+=2){
		offs=pcy+x1;
		coffs=(22-(a-(y1+24)))*24+23;

		for(b=x1;b<x1+23;b+=2){
			d=(rpmask[coffs]);
			if(d) dst[offs]=c+d;
			offs++;
			coffs-=2;
		}

		coffs=(22-(a-(y1+24)))*24+1;
		offs--;
		for(b=x1;b<x1+23;b+=2){
			d=(rpmask[coffs]);
			if(d) dst[offs]=c+d;
			offs++;
			coffs+=2;
		}
		pcy+=640;
	}
}


static void	DrawFRespCube(int x1,int y1,int x2,int y2,char color,char *dst){
	int		a,b;
	unsigned char	c,d;
	int		offs;
	int		coffs;

	c=color*16+64;
	for(a=y1;a<y1+31;a++){
		offs=a*640+x1;
		coffs=(a-y1)*32+30;

		for(b=x1;b<x1+31;b++){
			d=(rmask[coffs]);
			if(d!=0xe) dst[offs]=d+c+32;
			offs++;
			coffs--;
		}

		for(b=x1+31;b<x2-31;b++){dst[offs]=c;offs++;}

		coffs=(a-y1)*32;
		for(b=x2-31;b<x2;b++){
			d=(rmask[coffs]);
			if(d!=0xe) dst[offs]=d+c+32;
			offs++;
			coffs++;
		}
	}

	for(a=y1+31;a<y2-31;a++){
		offs=a*640+x1;
		for(b=x1;b<x2;b++){dst[offs]=c;offs++;}
	}
	for(a=y2-31;a<y2;a++){
		offs=a*640+x1;
		coffs=(30-(a-(y2-31)))*32+30;

		for(b=x1;b<x1+31;b++){
			d=(rmask[coffs]);
			if(d!=0xe) dst[offs]=d+c;
			offs++;
			coffs--;
		}

		for(b=x1+31;b<x2-31;b++){dst[offs]=c;offs++;}

		coffs=(30-(a-(y2-31)))*32;
		for(b=x2-31;b<x2;b++){
			d=(rmask[coffs]);
			if(d!=0xe) dst[offs]=d+c;
			offs++;
			coffs++;
		}
	}

}

static void	DrawFRTCube(int x1,int y1,int x2,int y2,char color,char *dst){
	int		a,b;
	unsigned char	c,d;
	int		offs;
	int		coffs;

	c=color*16+64+16;

	for(a=y1;a<y1+24;a++){
		offs=a*640+x1;
		coffs=(a-y1)*24+23;

		for(b=x1;b<x1+23;b++){
			d=(rpmask[coffs]);
			if(d) dst[offs]=c-(0xf-d);
			offs++;
			coffs--;
		}

		for(b=x1+23;b<x2-23;b++){dst[offs]=c;offs++;}

		coffs=(a-y1)*24+1;
		for(b=x2-23;b<x2;b++){
			d=(rpmask[coffs]);
			if(d) dst[offs]=c-(0xf-d);
			offs++;
			coffs++;
		}
	}

	for(a=y1+23;a<y2-23;a++){
		offs=a*640+x1;
		for(b=x1;b<x2;b++){dst[offs]=c;offs++;}
	}
	for(a=y2-23;a<y2;a++){
		offs=a*640+x1;
		coffs=(22-(a-(y2-23)))*24+23;

		for(b=x1;b<x1+23;b++){
			d=(rpmask[coffs]);
			if(d) dst[offs]=c-(0xf-d);
			offs++;
			coffs--;
		}

		for(b=x1+23;b<x2-23;b++){dst[offs]=c;offs++;}

		coffs=(22-(a-(y2-23)))*24+1;
		for(b=x2-23;b<x2;b++){
			d=(rpmask[coffs]);
			if(d) dst[offs]=c-(0xf-d);
			offs++;
			coffs++;
		}
	}

}

static void	DrawFRTCube2(int x1,int y1,int x2,int y2,char color,char *dst){
	int		a,b;
	unsigned char	c,d;
	int		offs;
	int		coffs;

//	c=color*16+64;

	c=color;
	for(a=y1;a<y1+24;a++){
		offs=a*640+x1;
		coffs=(a-y1)*24+23;

		for(b=x1;b<x1+23;b++){
			d=(rpmask[coffs]);
			if(d) dst[offs]=c+(0xf-d);
			offs++;
			coffs--;
		}

		for(b=x1+23;b<x2-23;b++){dst[offs]=c;offs++;}

		coffs=(a-y1)*24+1;
		for(b=x2-23;b<x2;b++){
			d=(rpmask[coffs]);
			if(d) dst[offs]=c+(0xf-d);
			offs++;
			coffs++;
		}
	}

	for(a=y1+23;a<y2-23;a++){
		offs=a*640+x1;
		for(b=x1;b<x2;b++){dst[offs]=c;offs++;}
	}

	for(a=y2-23;a<y2;a++){
		offs=a*640+x1;
		coffs=(22-(a-(y2-23)))*24+23;

		for(b=x1;b<x1+23;b++){
			d=(rpmask[coffs]);
			if(d) dst[offs]=c+(0xf-d);
			offs++;
			coffs--;
		}

		for(b=x1+23;b<x2-23;b++){dst[offs]=c;offs++;}

		coffs=(22-(a-(y2-23)))*24+1;
		for(b=x2-23;b<x2;b++){
			d=(rpmask[coffs]);
			if(d) dst[offs]=c+(0xf-d);
			offs++;
			coffs++;
		}
	}

}

static void	HBlur(char *dst,int x1,int y1,int x2,int y2,int factor){
	int	a,b,c;
	int	pos,p2;
	int	col;
	int	ifactor;
	float	ifz;

	if(factor>0){
		ifz=1.0f/(factor*2+1);
		ifactor=(int) (65536.0f*ifz);
		for(a=y1;a<y2;a++){
			pos=a*640+x1;
			for(b=x1;b<x2;b++){
				p2=pos-factor;
				col=0;
				for(c=-factor;c<=factor;c++){
					col+=dst[p2];
					p2++;
				}
				dst[pos]=((col*ifactor)>>16)+1;
				//dst[pos+1]=31;
				pos++;
			}
		}
	}
	
}

void	Credits(char *SBuffer){
	int	a;
	float	angle=0.0f;
	int	timer_start;
	int	blf=30;
	int	ADNColor=31;
	int	fcount=0;
	int	timer_stb;

	InitADN();

	wrkbuf=SBuffer;

	os_ticks_reset();
	timer_stb=0;
	timer_start=os_ticks_get();
        while(Order<0x6){
		Cls2(wrkbuf,SCREEN_SIZE>>2,31);

		RenderADN(wrkbuf,27,angle,1100.0,320,240);

		for(a=0;a<fcount;a++)
			DrawStringSemi(153,145+a*37,FontBuffer,wrkbuf,credits[a],1,0.0);

		if(blf>0 && fcount!=0)
			HBlur(wrkbuf,125,145+(fcount-1)*37,520,165+(fcount-1)*37,blf);

		DrawFCube(3,0,11,480,0,wrkbuf);
		DrawFCube(15,0,38,480,1,wrkbuf);
		DrawFCube(42,0,75,480,2,wrkbuf);
		DrawFCube(79,0,125,480,3,wrkbuf);


		if(!(Order==0x4 && Row<30)){
			if (oldskool) {
				DrawStringSemiB(270,80,FontBuffer,wrkbuf,"COMPOSICI•N",1,0.0);
				DrawStringQuarter(305,105,FontBuffer,wrkbuf,"V¡a Oral",1,0.0);
			} else {
				DrawStringSemiB(270,80,FontBuffer,wrkbuf,"COMPOSITION",1,0.0);
				DrawStringQuarter(310,105,FontBuffer,wrkbuf,"Orally",1,0.0);
			}
		}


		if(Order==0x4){
			if(Row>=60) ADNColor=31-(Row-60)*3;
			if(Row==12 && fcount!=1) {fcount=1;timer_stb=os_ticks_get();}
			if(Row==28 && fcount!=2) {fcount=2;timer_stb=os_ticks_get();}
			if(Row==44 && fcount!=4) {fcount=4;timer_stb=os_ticks_get();}
		}
		if(Order==0x5){
			if(Row<60)  ADNColor=19;
			if(Row>=60) ADNColor=19+(Row-60)*3;
			if(Row==12 && fcount!=5) {fcount=5;timer_stb=os_ticks_get();}
			if(Row==28 && fcount!=7) {fcount=7;timer_stb=os_ticks_get();}
			if(Row==44 && fcount!=8) {fcount=8;timer_stb=os_ticks_get();}
		}


		if(ADNColor!=31)
			RenderADN(wrkbuf,ADNColor,angle,450.0,580,230);


		if(fcount)
			blf=30-((int) (0.6*(os_ticks_get()-timer_stb)));

		angle=0.0025f*(os_ticks_get()-timer_start);
		if (os_fb_swap_other_buffer(wrkbuf))
			primperan_exit(0);
        }
}

#define Indicac_txt (oldskool ? INDICACIONES_oldskool : INDICACIONES_newskool)

static int hbst[4];
static int hb[4];
static int thb[4];

void	Indicaciones(char *SBuffer){
	int	a;
	float	angle=0.0f;
	float	angle2=0.0f;
	int	timer_start;
	int	timer_st2;
	int	LineY[8];
	int	YSize[8];
	int	LastRow=-1;
	int	spccount=0;
	int	lrsc=-1;
	float	ADNfov=850.0f;
	int	vSpos[8]={20+314,29+314,39+314,47+314,65+314,82+314,99+314,108+314};
	int	vEpos[8]={21+315,34+315,42+315,58+315,78+315,91+315,102+315,110+315};

	InitADN();

	wrkbuf=SBuffer;

	oldseed=0x1703;

	a=0;
	while(a<4){
		hbst[a]=0;
		a++;
	}

	for(a=0;a<8;a++){
		LineY[a]=(suiRand()&0x7f);
		if(LineY[a]>100) LineY[a]=100;
		LineY[a]+=320;
		YSize[a]=(suiRand()&0x3)+2;
	}
	os_ticks_reset();
	timer_start=os_ticks_get();
	timer_st2=timer_start;
        while(Order<0xB){
		Cls2(wrkbuf,SCREEN_SIZE>>2,31);
		
		if (oldskool) 
			DrawString(20,30,FontBuffer,wrkbuf,"INDICACIONES",10,0.0);
		else
			DrawString(20,30,FontBuffer,wrkbuf,"INDICATIONS",10,0.0);

		DrawFCube(380,40,620,60,2,wrkbuf);

		DrawFRCube(70-8,310-8,570+8,450+8,2,wrkbuf);	//??
		DrawFRTCube2(70,310,570,450,160,wrkbuf);	// 160

		RenderADNFlat(wrkbuf,angle,ADNfov,320,380);
		
		if(Order<0xA){
			if((Row==13 || Row==29 || Row==45 || Row==61 || 
		            Row==62 || Row==64) &&  LastRow!=Row)	{
				LastRow=Row;
				timer_start-=80;
			}

			if((Row==7  || Row==9  || Row==17 || Row==25 || Row==29 ||
			    Row==35 || Row==37 || Row==43 || Row==49 || Row==55 ||
			    Row==59) && lrsc!=Row){
				lrsc=Row;
				spccount++;
				timer_st2-=70;
			}
		}

		DrawStringSemiSp(215,100,FontBuffer,wrkbuf,Indicac_txt,1,0.0,spccount);

		if(Order==0xA){
			if(Row==5  || Row==9  || Row==12 || Row==20 || Row==29 || 
			   Row==31 || Row==36 || Row==44 || Row==52 || Row>60){
			 	LastRow=Row;
				timer_start-=80;
				//ADNfov+=15.0;
			}

			if(Row>=9 && Row<=12 && hbst[0]==0){
				hbst[0]=1;
				thb[0]=os_ticks_get();
				hb[0]=0;
			}

			if(Row>=25 && Row<=28 && hbst[1]==0){
				hbst[0]=1;
				hbst[1]=1;
				thb[1]=os_ticks_get();
				hb[1]=0;
			}

			if(Row>=30 && Row<=33 && hbst[2]==0){
				hbst[1]=0;
				hbst[2]=1;
				thb[2]=os_ticks_get();
				hb[2]=0;
			}

			if(Row>=40 && Row<=43 && hbst[3]==0){
				hbst[2]=0;
				hbst[3]=1;
				thb[3]=os_ticks_get();
				hb[3]=0;
			}
			if(Row>43) hbst[3]=0;
			if (oldskool) {
				if(Row>=9 && Row<=12 && hbst[0]!=0){
					HBlur(wrkbuf,200,122,600,144,hb[0]);
					HBlur(wrkbuf,200,188,580,210,hb[0]);
				}
	
				if(Row>=25 && Row<=28 && hbst[1]!=0){
					HBlur(wrkbuf,200,100,600,122,hb[1]);
					HBlur(wrkbuf,200,232,600,254,hb[1]);
				}
				if(Row>=30 && Row<=33 && hbst[2]!=0){
					HBlur(wrkbuf,200,166,618,188,hb[2]);
					HBlur(wrkbuf,200,210,630,232,hb[2]);
				}
	
				if(Row>=40 && Row<=43 && hbst[3]!=0){
					HBlur(wrkbuf,200,144,580,166,hb[3]);
					HBlur(wrkbuf,200,254,560,276,hb[3]);
				}
			} else {
				if(Row>=9 && Row<=12 && hbst[0]!=0){
					HBlur(wrkbuf,200,122,630,144,hb[0]);
					HBlur(wrkbuf,200,188,630,210,hb[0]);
				}
	
				if(Row>=25 && Row<=28 && hbst[1]!=0){
					HBlur(wrkbuf,200,100,630,122,hb[1]);
					HBlur(wrkbuf,200,232,630,254,hb[1]);
				}
				if(Row>=30 && Row<=33 && hbst[2]!=0){
					HBlur(wrkbuf,200,166,630,188,hb[2]);
					HBlur(wrkbuf,200,210,630,232,hb[2]);
				}
	
				if(Row>=40 && Row<=43 && hbst[3]!=0){
					HBlur(wrkbuf,200,144,630,166,hb[3]);
					HBlur(wrkbuf,200,254,630,276,hb[3]);
				}
			}
		}

		for(a=0;a<8;a++){
			DrawFCube(0,vSpos[a],70-8,vEpos[a],1,wrkbuf);	//??
			DrawFCube(578,vSpos[a],639,vEpos[a],1,wrkbuf);	//??
		}

		RenderCapsule(wrkbuf,25,PI/2+0.05+angle2/4,angle2,0,200.0,100,190,1);
		for(a=0;a<4;a++){
			hb[a]=((int) (0.95*(os_ticks_get()-thb[a])));
		}

		angle=0.006f*(os_ticks_get()-timer_start);
                angle2=0.002f*(os_ticks_get()-timer_st2);
		if (os_fb_swap_other_buffer(wrkbuf))
			primperan_exit(0);
        }
}

#define NBUBBLES 55

static int pillsx[NBUBBLES];
static int pillsy[NBUBBLES];
static int stdx[NBUBBLES];
static float angleb[NBUBBLES];
static int maxc[NBUBBLES];

#define Posologia_txt (oldskool ? Posologia_txt_oldskool : Posologia_txt_newkskool)

void	Aspirina(char *SBuffer){
	int	a,b;
	char	c;
	float	anglex = 0.0f;
	float	angley = 0.0f;
	float	anglez = 0.0f;
	int	pos,rpos;
	float	vpos;
	int	ab;
	int	timer_start;
	int	spccount=0;
	int	lstr=-1;
	wrkbuf=SBuffer;

	a=0;
	while(a<NBUBBLES){
		maxc[a]=(12<<3);
		a++;
	}
	for(a=0;a<14;a++){
		pillsx[a]=(((int) disp(0.0f,50.0f))+525);
		pillsy[a]=disp(440.0f,400.0f);
	}
	for(a=14;a<NBUBBLES;a++){
		pillsx[a]=(((int) disp(0.0f,50.0f))+525);
		pillsy[a]=disp(450.0f,400.0f);
	}

	for(a=0;a<14;a++) {
		stdx[a]=disp(480.0f,100.0f);
		angleb[a]=disp(0.0f,3.141516f);
	}
	for(a=14;a<NBUBBLES;a++){
		angleb[a]=disp(0.0f,3.141516f);
		stdx[a]=500;	//disp(500,0);
	}

	vpos=0.0;
	timer_start=os_ticks_get();

        while(Order<0xD){

		Cls2(wrkbuf,SCREEN_SIZE>>2,31);
		DrawFRCube(350,40,600,440,2,wrkbuf);	//??

		DrawFRCube(350,40,600,440,2,wrkbuf);	//??
		DrawFRTCube(350+10,40+10,600-10,440-10,4,wrkbuf);	//??

		for(a=0;a<14;a++){
			pillsx[a]=stdx[a]+4*fsin(angleb[a]);
			maxc[a]++;
		}
		for(a=14;a<NBUBBLES;a++){
			pillsx[a]=stdx[a]+8*fsin(angleb[a]);
			maxc[a]++;
		}

		vpos=0.512f*(os_ticks_get()-timer_start);
		anglex=0.006739f*(os_ticks_get()-timer_start);
		angley=0.023f*(os_ticks_get()-timer_start);
		anglez=0.01f*(os_ticks_get()-timer_start);

		for(a=0;a<14;a++){
			angleb[a]+=disp(0.1f,0.1f);
			pillsy[a]-=ffabs(disp(0.0f,0.5f));
			if(pillsy[a]<50) {pillsy[a]=440;maxc[a]=0;}
		}
		for(a=14;a<NBUBBLES;a++){
			angleb[a]+=disp(0.1f,0.1f);
			pillsy[a]-=(ffabs(disp(0.0f,1.0f))+2);
			if(pillsy[a]<50) {pillsy[a]=vpos+20;stdx[a]=disp(500.0f,(p_maxx-p_minx)>>1);maxc[a]=0;}
		}

		for(ab=0;ab<14;ab++){
			pos=pillsy[ab]*640+pillsx[ab];
			rpos=0;
			for(a=0;a<8;a++){
				if(((pillsy[ab]+a)>50) && (pillsy[ab]+a<430)){
					for(b=0;b<8;b++){
						//wrkbuf[pos]=176+bubble_a[rpos];	//-
						if(wrkbuf[pos]<176) wrkbuf[pos]=176+(bubble_a[rpos]);
						if(wrkbuf[pos]>=176) wrkbuf[pos]+=(bubble_a[rpos]);
						pos++;
						rpos++;
					}
					pos+=640-8;
				}else {pos+=640;rpos+=8;}
			}
		}

	//	RenderCapsule(wrkbuf,159,anglex,angley,anglez,500.0,320,240);

		RenderPill(wrkbuf,25,anglex,angley,anglez,320.0,180,vpos,0,1);
		RenderPill(wrkbuf,159,anglex,angley,anglez,100.0,500,vpos,1,0);

		if((Row==1  || Row==5  || Row==7  || Row==11 || Row==13 ||
		    Row==15 || Row==23 || Row==29 || Row==33 || Row==37 ||
		    Row==39 || Row==43 || Row==45 || Row==47 || Row==53 ||
		    Row==58 || Row==61) && lstr!=Row){
			lstr=Row;
			spccount+=3;
		}

		DrawStringSemiSp(20,100,FontBuffer,wrkbuf,Posologia_txt,1,0.0,spccount);

		for(ab=14;ab<NBUBBLES;ab++){
			pos=pillsy[ab]*640+pillsx[ab];
			rpos=0;
			for(a=0;a<8;a++){
				if((pillsy[ab]+a)>50 && (pillsy[ab]+a<430)){
					for(b=0;b<8;b++){
						c=bubble_a[rpos];
						if(c>(maxc[ab])>>3) c=(maxc[ab])>>3;
						if(wrkbuf[pos]>32 && wrkbuf[pos]<176) wrkbuf[pos]=176+(c<<1);
						if(wrkbuf[pos]<31) wrkbuf[pos]-=(c<<1);
						if(wrkbuf[pos]>=176) wrkbuf[pos]+=c;
						pos++;
						rpos++;
					}
					pos+=640-8;
				}else {pos+=640;rpos+=8;}
			}
		}
		if (os_fb_swap_other_buffer(wrkbuf))
			primperan_exit(0);
        }
}

void	Title(char *SBuffer){
	int	a;
	float	angle;
	float	cfac[7];
	int	started[7];
	int maskfzn[7];
	int maskpres[8];
	int maskprsol[8];
	int maskprim[9];
	char *fzn[]={"F","U","Z","Z","I","O","N"};
	char *pres[]={"P","R","E","S","E","N","T","S"};
	char *prim[]={"P","R","I","M","P","E","R","A","N"};
	char	pset=0;

	angle=0.0;

	InitADN();

	for(a=0;a<32;a++){
		os_fb_set_palette(a, 63, 63, 63);
	}
	for(a=64+16;a<256;a++){
		os_fb_set_palette(a, 63, 63, 63);
	}

	for(a=0;a<=6;a++){
		cfac[a]=0.0;
		started[a]=0;
		maskfzn[a]=1;
	}

	for(a=0;a<=7;a++) {
		maskpres[a]=0;
		maskprsol[a]=0;
		maskprim[a]=0;
		maskprim[a+1]=0;
		//pfac[a]=0;
	}
	wrkbuf=SBuffer;
	DrawFCube(315,17,568,29,0,wrkbuf);

	timer_start=os_ticks_get();
        while(Order<0x4){
	      Cls2(wrkbuf,SCREEN_SIZE>>2,31);
	      if(Order==0x0){
		if(Row>=8+1 && Row<16+1) {
			if(!started[0]){
				started[0]=1;
				cfac[0]=0.0;
			}
			DrawCircle(0*(64+24)+24,240-32,0,0x2222,cfac[0],wrkbuf);
		}
		if(Row>=14+1 && Row<28+1) {
			if(!started[4]){
				started[4]=1;
				cfac[4]=0.0;
			}
			DrawCircle(4*(64+24)+24,240-32,0,0x2222,cfac[4],wrkbuf);
		}
		if(Row>=16) {
			DrawCircle(0*(64+24)+24,240-32,0,0x2222,0,wrkbuf);
			DrawCircleT(0*(64+24)+24+8,240-32+8,0,wrkbuf);
			DrawStringA(0*(64+24)+24+20,240-32+10,FontBuffer,wrkbuf,"F",8,0.0,64);
		}
		if(Row>=18) {
			if(!started[1]){
				started[1]=1;
				cfac[1]=0.0;
			}
			DrawCircle(1*(64+24)+24,240-32,0,0x2222,cfac[1],wrkbuf);
		}

		if(Row>=22 && Row<24) {
			DrawCircle(2*(64+24)+24,240-32,0,0x2222,0,wrkbuf);
			DrawCircleT(2*(64+24)+24+8,240-32+8,0,wrkbuf);
			DrawStringA(2*(64+24)+24+20,240-32+10,FontBuffer,wrkbuf,"Z",8,0.0,64);
		}
		if(Row>=24+1 && Row<56+1) {
			if(!started[2]){
				started[2]=1;
				cfac[2]=0.0;
			}
			DrawCircle(2*(64+24)+24,240-32,0,0x2222,cfac[2],wrkbuf);
		}
		if(Row>=26+1) {
			DrawCircle(3*(64+24)+24,240-32,0,0x2222,0,wrkbuf);
			DrawCircleT(3*(64+24)+24+8,240-32+8,0,wrkbuf);
			DrawStringA(3*(64+24)+24+20,240-32+10,FontBuffer,wrkbuf,"Z",8,0.0,64);
		}
		if(Row>=28+1 && Row<50+1) {
			if(started[4]){
				started[4]=0;
				cfac[4]=0.0;
			}
			DrawCircle(4*(64+24)+24,240-32,0,0x2222,cfac[4],wrkbuf);
		}
		if(Row>=32+1) {
			if(!started[5]){
				started[5]=1;
				cfac[5]=0.0;
			}
			DrawCircle(5*(64+24)+24,240-32,0,0x2222,cfac[5],wrkbuf);
		}

		if(Row>=40+1) {
			if(!started[6]){
				started[6]=1;
				cfac[6]=0.0;
			}
			DrawCircle(6*(64+24)+24,240-32,0,0x2222,cfac[6],wrkbuf);
		}
		if(Row>=46+1) {
			if(!started[1]){
				started[1]=1;
				cfac[1]=0.0;
			}
			DrawCircle(1*(64+24)+24,240-32,0,0x2222,cfac[1],wrkbuf);
		}
		if(Row>=48+1) {
			DrawCircle(5*(64+24)+24,240-32,0,0x2222,0,wrkbuf);
			DrawCircleT(5*(64+24)+24+8,240-32+8,0,wrkbuf);
			DrawStringA(5*(64+24)+24+20,240-32+10,FontBuffer,wrkbuf,"O",8,0.0,64);
		}
		if(Row>=50+1) {
			if(!started[4]){
				started[4]=1;
				cfac[4]=0.0;
			}
			DrawCircle(4*(64+24)+24,240-32,0,0x2222,cfac[4],wrkbuf);
		}
		if(Row>=52+1) {
			DrawCircle(4*(64+24)+24,240-32,0,0x2222,0,wrkbuf);
			DrawCircleT(4*(64+24)+24+8,240-32+8,0,wrkbuf);
			DrawStringA(4*(64+24)+24+20,240-32+10,FontBuffer,wrkbuf,"I",8,0.0,64);
		}
		if(Row>=54+1) {
			DrawCircle(6*(64+24)+24,240-32,0,0x2222,0,wrkbuf);
			DrawCircleT(6*(64+24)+24+8,240-32+8,0,wrkbuf);
			DrawStringA(6*(64+24)+24+20,240-32+10,FontBuffer,wrkbuf,"N",8,0.0,64);
		}
		if(Row>=56+1 && Row<60+1) {
			if(started[2]){
				started[2]=0;
				cfac[2]=0.0;
			}
			DrawCircle(2*(64+24)+24,240-32,0,0x2222,cfac[2],wrkbuf);
		}
		if(Row>=58+1) {
			DrawCircle(1*(64+24)+24,240-32,0,0x2222,0,wrkbuf);
			DrawCircleT(1*(64+24)+24+8,240-32+8,0,wrkbuf);
			DrawStringA(1*(64+24)+24+20,240-32+10,FontBuffer,wrkbuf,"U",8,0.0,64);
		}
		if(Row>=60+1) {
			DrawCircle(2*(64+24)+24,240-32,0,0x2222,0,wrkbuf);
			DrawCircleT(2*(64+24)+24+8,240-32+8,0,wrkbuf);
			DrawStringA(2*(64+24)+24+20,240-32+10,FontBuffer,wrkbuf,"Z",8,0.0,64);
		}
 	   }
	   if(Order==0x1){
		for(a=0;a<=6;a++){
			if(maskfzn[a]){
				DrawCircle(a*(64+24)+24,240-32,0,0x2222,0,wrkbuf);
				DrawCircleT(a*(64+24)+24+8,240-32+8,0,wrkbuf);
				DrawStringA(a*(64+24)+24+20,240-32+10,FontBuffer,wrkbuf,fzn[a],8,0.0,64);
			}
		}

		for(a=0;a<=7;a++){
			if(maskpres[a]){
				DrawCircleL(a*(32+7)+7,240+40,0,0x2222,0,wrkbuf);
				DrawCircleTL(a*(32+7)+7+4,240+40+4,0,wrkbuf);
				DrawStringSemiA(a*(32+7)+7+4+7,240+40+4+2,FontBuffer,wrkbuf,pres[a],8,0.0,64);
			}
		}

		if(Row==1) {maskpres[4]=1;maskprsol[1]=1;}//pfac[1]=0.0;}
		if(Row==6+1) {maskpres[1]=1;maskprsol[1]=0;}
		if(Row==8+1)  {maskprsol[5]=1;}//pfac[5]=0.0;}
		if(Row==12+1) maskpres[3]=1;
		if(Row==14+1) {maskprsol[0]=1;}//pfac[0]=0.0;}
		if(Row==16+1) maskpres[7]=1;
		if(Row==18+1) {maskprsol[2]=1;}//pfac[2]=0.0;}
		if(Row==22+1) {maskpres[5]=1;maskprsol[5]=0;}
		if(Row==24+1) {maskprsol[6]=1;}//pfac[6]=0.0;}
		if(Row==26+1) {maskpres[0]=1;maskprsol[0]=0;}
		if(Row==28+1) {maskprsol[2]=1;}//pfac[2]=0.0;}
		if(Row==32+1) {maskpres[2]=1;maskprsol[2]=0;maskprsol[6]=1;maskfzn[3]=0;}//pfac[6]=0.0;}
		if(Row==38+1) {maskpres[6]=1;maskprsol[6]=0;maskfzn[2]=0;}
		if(Row==48+1) maskfzn[5]=0;
		if(Row==52+1) maskfzn[0]=0;
		if(Row==54+1) maskfzn[6]=0;
		if(Row==58+1) maskfzn[4]=0;
		if(Row==60+1) maskfzn[1]=0;
	   }
	   if(Order==0x2){
		for(a=0;a<256;a++){
			if(a<32 || a>=64+16){
				os_fb_set_palette(a,(Row*fpal[a*3  ]+(64-Row)*63)>>6,
					(Row*fpal[a*3+1]+(64-Row)*63)>>6,
					(Row*fpal[a*3+2]+(64-Row)*63)>>6);

			}
		}
	   }
	   if(Order==0x3 && !pset){
		for(a=0;a<256;a++){
			os_fb_set_palette(a,fpal[a*3  ], fpal[a*3+1], fpal[a*3+2]);
		}
		pset=1;
	   }
	   if(Order>=0x2){
		for(a=0;a<=7;a++){
			if(maskpres[a]){
				DrawCircleL(a*(32+7)+7,240+40,0,0x2222,0,wrkbuf);
				DrawCircleTL(a*(32+7)+7+4,240+40+4,0,wrkbuf);
				DrawStringSemiA(a*(32+7)+7+4+7,240+40+4+2,FontBuffer,wrkbuf,pres[a],8,0.0,64);
			}
		}

		if(Order>=0x2){
			if(Row==32+1)	{maskprim[3+4*(Order-0x2)]=1;maskpres[4]=0;maskpres[7]=0;}
			if(Row==36+1)	{maskprim[1+3*(Order-0x2)]=1;maskpres[5]=0;}
			if(Row==38+1)	{maskprim[8-2*(Order-0x2)]=1;maskpres[6]=0;maskpres[0]=0;}
			if(Row==48+1)	{maskprim[0]=1;maskpres[2]=0;}
			if(Row==52+1)	{maskprim[5]=1;maskpres[3]=0;}
			if(Row==56+1)	{maskprim[2]=1;maskpres[1]=0;}
		}

		DrawFCube(340,14,593,29,0,wrkbuf);
		DrawFCube(340,34,593,63,1,wrkbuf);
		DrawFCube(340,68,593,136,2,wrkbuf);
		DrawFCube(340,140,593,246,3,wrkbuf);

		DrawStringSemi(529,264,FontBuffer,wrkbuf,"717926",1,0.0);

		DrawStringSemiV(549,480-283-20,FontBuffer,wrkbuf,"O",1,0.0);

		for(a=0;a<9;a++)
			if(maskprim[a]) {
				if(a==2) {
					DrawString(346+a*27+8,338,FontBuffer,wrkbuf,prim[a],8,0.0);//4=size
				} else{
					if(a==3)
						DrawString(346+a*27-5,338,FontBuffer,wrkbuf,prim[a],8,0.0);//4=size//-2...
					else
						DrawString(346+a*27,338,FontBuffer,wrkbuf,prim[a],8,0.0);//4=size
				}
			}

		if (oldskool) {
			DrawStringQuarter(346,373,FontBuffer,wrkbuf,"Metoclopramida",1,0.0);
			DrawStringQuarter(547,373,FontBuffer,wrkbuf,"Soluci¢n",1,0.0);
			DrawStringSemi(420-60,435,FontBuffer,wrkbuf,"VÖA ORAL Y VISUAL",3,0.0);
			DrawStringSemi(398,452,FontBuffer,wrkbuf,"INTRO DE 64 kb",1,0.0);
		} else {
			DrawStringQuarter(346,373,FontBuffer,wrkbuf,"Metoclopramide",1,0.0);
			DrawStringQuarter(547,373,FontBuffer,wrkbuf,"Solution",1,0.0);
			DrawStringSemi(350,435,FontBuffer,wrkbuf,"ORALLY AND VISUALLY",3,0.0);
			DrawStringSemi(418,452,FontBuffer,wrkbuf,"64KB INTRO",1,0.0);
		}
	   }
	   for(a=0;a<7;a++){
	     cfac[a]+=angle;
	   }
	   angle=0.0008*(os_ticks_get()-timer_start);
	if (os_fb_swap_other_buffer(wrkbuf))
		primperan_exit(0);
	}
}
	
#define	NPASTIS 20

static float pastillasY[NPASTIS];
static float pastillasX[NPASTIS];
static float pastivelY[NPASTIS];
static float pastivelX[NPASTIS];
static char deadpasti[NPASTIS];
static float pdx[NPASTIS];
static float pdy[NPASTIS];
static float pdz[NPASTIS];
static float pvel[NPASTIS];

#define contra_txt (oldskool ? contra_txt_oldskool : contra_txt_newskool)
#define precau_txt (oldskool ? precau_txt_oldskool : precau_txt_newskool)

void	Pastillaz(char *SBuffer){
	int	a;
	int	timer_start;
	int	spccount1=3;
	int	lstr=-1;
	float	dif;

	oldseed=33;
	for(a=0;a<NPASTIS;a++){
		pastillasY[a]=disp(200.0,200.0)-300;
		pastillasX[a]=disp(175.0,125.0);
		pastivelY[a]=0;
		pastivelX[a]=disp(0,5.0);
		deadpasti[a]=0;
		pdx[a]=disp(10.0,10.0);
		pdy[a]=0.0;
		pdz[a]=0.0;
		pvel[a]=disp(0,2.0)/4.0;
	}

	ymin=200;
	ymax=430;

	wrkbuf=SBuffer;

	Cls2(wrkbuf,SCREEN_SIZE>>2,31);

	DrawFRCube(50,20,300,190,0,wrkbuf);
	DrawFRTCube2(50,190-50,300,250,80,wrkbuf);
	DrawFRespCube(50,190,300,440,2,wrkbuf);

	DrawFRTCube(50+10,190+10,300-10,440-10,4,wrkbuf);

	timer_start=os_ticks_get();

        while(Order<0xf){
		if (oldskool) {
			DrawStringSemi(320,80,FontBuffer,wrkbuf,"CONTRAINDICACIONES",1,0.0);
			DrawStringSemi(320,200,FontBuffer,wrkbuf,"PRECAUCIONES",1,0.0);
		} else {
			DrawStringSemi(320,80,FontBuffer,wrkbuf,"CONTRAINDICATION",1,0.0);
			DrawStringSemi(320,200,FontBuffer,wrkbuf,"PRECAUTIONS",1,0.0);
		}

		DrawStringSemiSp(325,100,FontBuffer,wrkbuf,contra_txt,1,0.0,spccount1);
		DrawStringSemiSp(325,220,FontBuffer,wrkbuf,precau_txt,1,0.0,spccount1);

		DrawFRespCube(50,190,300,440,2,wrkbuf);
		DrawFRTCube(50+10,190+10,300-10,440-10,4,wrkbuf);

		if((Row==1  || Row==5  || Row==7  || Row==11 || Row==13 ||
		    Row==15 || Row==23 || Row==29 || Row==33 || Row==37 ||
		    Row==39 || Row==43 || Row==45 || Row==47 || Row==53 ||
		    Row==58 || Row==61) && lstr!=Row){
			lstr=Row;
			spccount1++;
		}

		for(a=0;a<NPASTIS;a++){
			RenderCapsule(wrkbuf,159,pdx[a],pdy[a],pdz[a],(50.0+(a>>1)),pastillasX[a],pastillasY[a],0);
		}

		if(timer_start!=os_ticks_get()){
			dif=((float)(os_ticks_get()-timer_start))/3.6;
			timer_start=os_ticks_get();
		   for(a=0;a<NPASTIS;a++){
			if(deadpasti[a]==0){
			   pastillasY[a]+=pastivelY[a];
			   pastillasX[a]+=pastivelX[a];
			   if(pastillasX[a]<100){	//90){
				pastivelX[a]=-pastivelX[a];
				pastillasX[a]=100;
			   }

			   if(pastillasX[a]>250){
				pastivelX[a]=-pastivelX[a];
				pastillasX[a]=250;
			   }

			   if(pastillasY[a]>423) {
				if(pastivelY[a]>0) {
					if(pastivelY[a]<0.8) {pastivelY[a]=0.0;deadpasti[a]=1;}
					pastivelY[a]=-pastivelY[a]/2.5;
					pastillasY[a]=423;
				}
			   }
			   pastivelX[a]*=0.99;
			   pvel[a]*=0.99;
			} else {
			   if(pdy[a]>5) pvel[a]*=0.80;
			   else pvel[a]*=0.25;
			   pastivelX[a]*=0.59;
			}
			pastivelY[a]+=(dif*0.981)/(1.5*8);//12
			pdy[a]+=pvel[a];
		    }
		}
		if (os_fb_swap_other_buffer(wrkbuf))
			primperan_exit(0);
	}
}

#define greets (oldskool ? greets_oldskool : greets_newskool)

void	Greetings(char *SBuffer){
	float	angle;
	float	angle2;
	int	timer_start;
	int	timer_st2;
	int	spccount=0;
	int	lstr=-1;

	angle=0.0;
	angle2=0.0;

	wrkbuf=SBuffer;

	oldseed=0x1703;

	os_ticks_reset();
	timer_start=os_ticks_get();
	timer_st2=timer_start;
        while(Order<0x11){
		Cls2(wrkbuf,SCREEN_SIZE>>2,31);

		if (oldskool)
			DrawString(57,67,FontBuffer,wrkbuf,"AGRADECIMIENTOS",1,0.0);
		else
			DrawString(57,67,FontBuffer,wrkbuf,"GREETINGS",1,0.0);

		DrawFCube(0,20,34,30+1,1,wrkbuf);
		DrawFCube(0,39,34,49+1,1,wrkbuf);
		DrawFCube(0,58,34,68+1,1,wrkbuf);

		DrawFCube(564,451,574,480,1,wrkbuf);
		DrawFCube(582,451,592,480,1,wrkbuf);
		DrawFCube(601,451,611,480,1,wrkbuf);

		DrawFRCube(402,49,580+1,202+1,2,wrkbuf);
//		DrawFCube(419,65,563+1,185+1,0,wrkbuf);
		DrawFRTCube2(419,65,563+1,185+1,160,wrkbuf);

		if((Row==1  || Row==5  || Row==7  || Row==11 || Row==13 ||
		    Row==15 || Row==17 || Row==19 || Row==23 || Row==27 ||
		    Row==29 || Row==33 || Row==35 || Row==39 || Row==43 ||
		    Row==47 || Row==49 || Row==53 || Row==55 || Row==61) &&
		    (lstr!=Row)){
			lstr=Row;
			spccount++;
			timer_start-=50;
			timer_st2-=30;
		}

		DrawFRCube(402,49+200,580+1,202+1+200,2,wrkbuf); //frcube...,2
		DrawFRTCube2(419,65+200,563+1,185+1+200,160,wrkbuf);

		RenderCapsule(wrkbuf,25,PI/2+0.05+angle/4,angle,0,250.0,419+77,320,2);
		RenderPill(wrkbuf,159,angle2/3,angle2/8,0,200.0,500,120,0,0);

		DrawStringSemiSp(45,147,FontBuffer,wrkbuf,greets,1,0.0,spccount);

		angle=0.00834*(os_ticks_get()-timer_start);
		angle2=0.0342*(os_ticks_get()-timer_st2);

		if (os_fb_swap_other_buffer(wrkbuf))
			primperan_exit(0);
	}
}

#define secundarios (oldskool ? secundarios_oldskool : secundarios_newskool)

void	Secundarios(char *SBuffer){
	int	timer_start;
	float	angle;
	int	spccount=0;
	int	timer_stb;
	int	lstr=-1;

	angle=0.0;

	wrkbuf=SBuffer;

	os_ticks_reset();
	timer_stb=0;
	timer_start=os_ticks_get();

        while(Order<0x13){
		Cls2(wrkbuf,SCREEN_SIZE>>2,31);

		RenderPill(wrkbuf,10,angle,angle/3,angle/4,90.0,580,60,0,1);

		if((Row==1  || Row==5  || Row==7  || Row==11 || Row==13 ||
		    Row==15 || Row==17 || Row==19 || Row==23 || Row==27 ||
		    Row==29 || Row==33 || Row==35 || Row==39 || Row==43 ||
		    Row==47 || Row==49 || Row==53 || Row==55 || Row==61) &&
		    (lstr!=Row)){
			lstr=Row;
			spccount++;
		}

		DrawStringSemiSp(15,108,FontBuffer,wrkbuf,secundarios,1,0.0,spccount);

		if (oldskool)
			DrawString(25,20,FontBuffer,wrkbuf,"EFECTOS SECUNDARIOS",3,0.0);
		else
			DrawString(25,20,FontBuffer,wrkbuf,"SIDE EFFECTS",3,0.0);

		DrawFRCube(49-8,305-8,591+8,458+8,3,wrkbuf);
		DrawFRTCube2(49,305,591,458,160,wrkbuf);

		DrawFCube(0,14+32+20,502,14+32+32+20,0,wrkbuf);
		DrawFCube(502,14+32+31+20,502+31,14+32+32+32+20,0,wrkbuf); //frcube...,2
		DrawFCube(502+31,14+32+64+20,639,14+32+32+62+20,0,wrkbuf); //frcube...,2
		DrawCircle(502-31,14+32+20,0,0x0020,0,wrkbuf);
		DrawCircle(502,14+64+20,0,0x0200,0,wrkbuf);

		RenderADNHor(wrkbuf,160+16,angle,500.0,49+440/2,305+143/2);

		angle=0.004*(os_ticks_get()-timer_start);
		if (os_fb_swap_other_buffer(wrkbuf))
			primperan_exit(0);
	}
}

void	Initbuf(){
	SBuffer2=(char *) malloc(SCREEN_SIZE+SCREEN_WIDTH);
	Cls2(SBuffer2,SCREEN_SIZE>>2,0);
}

static void EndInner(unsigned int *a, unsigned int *b, unsigned int *c) {
	int eax, ecx = 160;
	while (ecx > 0) {
		eax = *c;
		eax &= 0xf0f0f0f0;
		eax += *b;
		*a = eax;
		++a;
		++b;
		++c;
		--ecx;
	}
}

void	End(unsigned char *SBuffer){
	int	a,b,offs,rpos;
	int	coffs;
	unsigned char	c,cc;
	unsigned char	*end_1,*end_2,*end_3,*endp;
	unsigned char	*rndbuf;
	float	r1;
	int	ldesp;
	unsigned char	lrnd[480];
	int	sfactor;
	int	ny;
	int	framenum = 0;
	int	barposy=400;
	unsigned char	d;

	end_1=(unsigned char *) &end1;
	end_2=(unsigned char *) &end2;
	end_3=(unsigned char *) &end3;
	endp=(unsigned char *) &palend;

	wrkbuf=SBuffer;

	rndbuf=(unsigned char *) malloc(SCREEN_SIZE+SCREEN_WIDTH*2);

	Cls2(wrkbuf,SCREEN_SIZE>>2,1);


	offs=39*640+61;
	for(a=0;a<92;a++){
		rpos=a*98/2;
			for(b=0;b<98/2;b++){
			c=end_1[rpos];
			cc=c&0xf;
			c>>=4;
			wrkbuf[offs]=c+1;
			wrkbuf[offs+1]=cc+1;
			offs+=2;
			rpos++;
		}
		offs+=640-98;
	}

	offs=191*640+69;
	for(a=0;a<83;a++){
		rpos=(a*72)/2;
		for(b=0;b<72/2;b++){
			c=end_2[rpos];
			cc=c&0xf;
			c=(c>>4)&0xf;
			wrkbuf[offs]=c+1;
			wrkbuf[offs+1]=cc+1;
			offs+=2;
			rpos++;
		}
		offs+=640-72;
	}

	offs=313*640+67;
	for(a=0;a<140;a++){
		rpos=(a*76)/2;
		for(b=0;b<74/2;b++){
			c=end_3[rpos];
			cc=c&0xf;
			c>>=4;
			wrkbuf[offs]=c+1;
			wrkbuf[offs+1]=cc+1;
			offs+=2;
			rpos++;
		}
		offs+=640-74;
	}

	if (oldskool) {
		DrawString(200,64,FontBuffer,wrkbuf,"Lea las instrucciones de",2,0.0);
		DrawString(200,212,FontBuffer,wrkbuf,"este medicamento y",2,0.0);
		DrawString(200,358,FontBuffer,wrkbuf,"consulte al farmac‚utico",2,0.0);
	} else {
		DrawString(200,64,FontBuffer,wrkbuf,"Read the instructions of",2,0.0);
		DrawString(200,212,FontBuffer,wrkbuf,"this medicine and",2,0.0);
		DrawString(200,358,FontBuffer,wrkbuf,"consult your pharmacist",2,0.0);
	}

	sfactor=0;
	cc=0;
	ldesp=0;
	for(a=0;a<480;a++){
		r1=(float) suiRand();
		r1=(r1/((float) SUI_RAND_MAX));
		r1*=128;
		lrnd[a]=((int) r1+100);

		offs=a*640;
		for(b=0;b<640;b++){
			r1=(float) suiRand();
			r1=(r1/((float) SUI_RAND_MAX));
			r1*=7*16;
			rndbuf[offs]=((((int) r1)*lrnd[a])>>8);
			offs++;
		}
	}
	for(b=0;b<640*2;b++){
		rndbuf[480*640+b]=0;
	}
	os_fb_set_palette(0, 0, 0, 0);
	for(a=1;a<256;a++){
		os_fb_set_palette(a,endp[(a-1)*3  ],endp[(a-1)*3+1],endp[(a-1)*3+2]);
	}
	while((Order==0x13 || (Order==0x14 && Row<60))){
		for(a=0;a<480;a++){
			offs=a*640;
			if(a>barposy && a<barposy+20){
				rpos=480*640;
			}else{
				r1=(float) suiRand();
				r1=(r1/((float) SUI_RAND_MAX));
				r1*=480;
				rpos=((int) r1);
				rpos=rpos*640;
			}
			if(a>35 && a<445)
				ny=((sfactor/2)*(fsin((a*16+framenum)*PI/180.0))+2*sfactor*(fsin((a+framenum)*PI/180.0)));
			else
				ny=0;
			if (ny > 630)
				ny = 630;
			if (ny < 0)
				ny = 0;
			
			rpos+=ny;
			ny+=offs;
			EndInner((char *) (SBuffer2+offs),(char *) (wrkbuf+ny),(char *) (rndbuf+rpos));
		}
		if((suiRand()&31)==0) sfactor+=1280;

		for(a=0;a<31;a++){
			offs=a*640;
			coffs=a*32+30;
			for(b=0;b<31;b++){
				d=(rmask[coffs]);
				if(d==0xe) SBuffer2[offs]=0;	//d+128-16;
				if(d>0x4) SBuffer2[offs]=129+d;
				offs++;
				coffs--;
			}

			offs+=640-31-31;
			coffs++;
			for(b=0;b<31;b++){
				d=(rmask[coffs]);
				if(d==0xe) SBuffer2[offs]=0;
				if(d>0x4) SBuffer2[offs]=129+d;
				offs++;
				coffs++;
			}

			offs=(a+480-31)*640;
			coffs=(30-a)*32+30;

			for(b=0;b<31;b++){
				d=(rmask[coffs]);
				if(d==0xe) SBuffer2[offs]=0;
				if(d>0x4) SBuffer2[offs]=129+d;
				offs++;
				coffs--;
			}
			offs+=640-31-31;
			coffs++;
			//coffs=(30-a)*32;
			for(b=0;b<31;b++){
				d=(rmask[coffs]);
				if(d==0xe) SBuffer2[offs]=0;
				if(d>0x4) SBuffer2[offs]=129+d;
				offs++;
				coffs++;
			}
		}
		if (os_fb_swap_other_buffer(SBuffer2))
			primperan_exit(0);
		if(sfactor>0) sfactor/=2;
		framenum++;
		barposy--;
		if((barposy+20)<0) barposy=480;
        }
}

