extern	void	DrawString(int x,int y,char *fb,char *db,char *str,int size,float iangle);
extern	void	DrawStringSemi(int x,int y,char *fb,char *db,char *str,int size,float iangle);
extern	void	DrawStringSemiSp(int x,int y,char *fb,char *db,char *str,int size,float iangle,int spacen);
extern	void	DrawStringSemiV(int x,int y,char *fb,char *db,char *str,int size,float iangle);
extern	void	DrawStringSemiB(int x,int y,char *fb,char *db,char *str,int size,float iangle);
extern	void	DrawStringQuarter(int x,int y,char *fb,char *db,char *str,int size,float iangle);
extern	void	DrawStringA(int x,int y,char *fb,char *db,char *str,int size,float iangle,int cadd);
extern	void	DrawStringSemiA(int x,int y,char *fb,char *db,char *str,int size,float iangle,int cadd);

extern	void	DeCrunchFont(unsigned char *fb);
extern	int	fpal[768];
