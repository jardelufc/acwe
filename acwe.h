#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>

#include <time.h>       /* time_t, struct tm, difftime, time, mktime */
 typedef  struct  {
	
	unsigned char *pdata;
	unsigned int m0,n0,k0;
	unsigned int datasize;
	
} TImage;



void Boundary( unsigned char *matrizLS, unsigned char *matriz,unsigned char *matriz2,unsigned long long c0,unsigned long long c1, int iteracoes,int LARGURA, int ALTURA, int FATIAS);
void splitbyfour(unsigned char *pls, unsigned char **plsvec, int m0, int n0, int k0 );
int  saveraw(char *name, unsigned char *p, unsigned int size);
int circle_levelset(unsigned char *p,int centerm, int centern, int centerk, int raio, int m0, int n0, int k0);
void copy4volume(unsigned char *v, unsigned char *vsrc, int position, int m0, int n0, int k0);
void binmasscenter(unsigned char *p, int *pm, int *pn, int *pk,int m0, int n0, int k0, int limiar);
void splitby2x(int x,unsigned char *pls, unsigned char **plsvec, int m0, int n0, int k0 );
void copy2xvolume(int x, unsigned char *v, unsigned char *vsrc, int position, int m0, int n0, int k0);
void binarize(unsigned char *p, unsigned char *pout,int m0, int n0, int k0, unsigned int limiar);
void cleanborder(unsigned char *img,int m0, int n0, int k0, int limiar);
float dsc(unsigned char *img,unsigned char *imggold,int size);
void crop(TImage *t, TImage *tsrc, int im0, int in0, int ik0 );
int readmhdraw (char *name, TImage *Image) ;
