#include <stdio.h>
#include <malloc.h>
#include <string.h>

#include <time.h>       /* time_t, struct tm, difftime, time, mktime */
 

  time_t timer;
  struct tm y2k = {0};
  double seconds,seconds2, tempo; 

void Boundary( unsigned char *matrizLS, unsigned char *matriz,unsigned char *matriz2,unsigned long long c0,unsigned long long c1, int iteracoes,int LARGURA, int ALTURA, int FATIAS);
void splitbyfour(unsigned char *pls, unsigned char **plsvec, int m0, int n0, int k0 );
int  saveraw(char *name, unsigned char *p, unsigned int size);
int circle_levelset(unsigned char *p,int centerm, int centern, int centerk, int raio, int m0, int n0, int k0);
void copy4volume(unsigned char *v, unsigned char *vsrc, int position, int m0, int n0, int k0);
void binmasscenter(unsigned char *p, int *pm, int *pn, int *pk,int m0, int n0, int k0, int limiar);


typedef struct  {
	
	unsigned char *pdata;
	unsigned int m0,n0,k0;
	
} TImage;

int initimage(TImage *image, int m0, int n0, int k0) {
	image->m0=m0;
	image->n0=n0;
	image->k0=k0;
	image->pdata=malloc(n0*m0*k0);
	return (int) image->pdata;
}

void freeimage(TImage *image) {
	free(image->pdata);
}

void copyimage(TImage *dest, TImage *source) {
	//memcpy (dest->pdata,source->pdata,dest)
	
}
int main(int argc, char **argv)
{
	FILE *fp;
	FILE *fpls;
	
	unsigned char  *p,*matriz,*matriz2,*matrizpartial;
	unsigned char *pls,*matrizls;
	unsigned int aux;
	unsigned int i,j,k;
	unsigned long long c0;
	unsigned long long c1;
	unsigned char *matrizlssplit[4];
    int mx, ny, kz;

	i=j=k=0;
	
	
	fpls=fopen(argv[1], "rb");
	if(fpls==NULL) {
		printf("error opening file\n");
		exit(0);
	}
	fp=fopen(argv[2], "rb");
	if(fp==NULL) {
		printf("error opening file\n");
		exit(0);
	}

	
	matrizls=malloc(205*281*420*sizeof(char));
	if(matrizls==NULL) {
		printf("error memory allocation\n");
		exit(0);
	}
		
    matriz=malloc(205*281*420*sizeof(char));
	if(matriz==NULL) {
		printf("error memory allocation\n");
		exit(0);
	}
    matriz2=malloc(205*281*420*sizeof(char));
	if(matriz2==NULL) {
		printf("error memory allocation\n");
		exit(0);
	}
    matrizpartial=malloc(102*281*210*sizeof(char));
	if(matrizpartial==NULL) {
		printf("error memory allocation\n");
		exit(0);
	}
	
	for(i=0;i<4;i++) {
		matrizlssplit[i]=malloc(103*281*210*sizeof(char));
		if(matrizlssplit[i]==NULL) {
			printf("error memory allocation\n");
			exit(0);
		}
	}

	p=matriz;
	
	while(!feof(fp)) {
		
		fread(&aux,sizeof(int),1,fp);
		*p=(unsigned char)aux;		
		p++;
		
	}

	pls=matrizls;
	p=matriz;

	c0=0L;
	c1=0L;
	j=0;
	k=0;
	while(!feof(fpls)) {
		
		fread(&aux,sizeof(int),1,fpls);
	    *pls=(unsigned char)aux;
		if(*p==0) {
            c0 = c0 +(unsigned long long)(*pls);
            j++;
	    }
		else if(*p==1)
		{
			c1=c1+(unsigned long long)(*pls);
            k++;
			//printf ("teste %d", (int)(*p));
		}
		pls++;
		p++;
		
	}
     
	splitbyfour(matrizls, matrizlssplit, 420, 281, 205 );

	printf("c0=%llu numero de amostras= %d e sua media= %ld \n",c0,j,c0/(unsigned long long)j);
    printf("c1=%llu numero de amostras= %d e sua media: %ld \n",c1,k,c1/(unsigned long long)k);
	
    c0=c0/(unsigned long long)j;
    c1=c1/(unsigned long long)k;
	
	pls=matrizls;	
	//printf("alo mundo %d %d %d %d\n", i ,j ,k,matrizls[0]);
	fflush(stdout);
	//fclose(fp);
	fclose(fpls);
	
	memcpy(matriz2, matriz, 205*281*420*sizeof(char));
  y2k.tm_hour = 0;   y2k.tm_min = 0; y2k.tm_sec = 0;
  y2k.tm_year = 100; y2k.tm_mon = 0; y2k.tm_mday = 1;





   tempo=0.0;
  for(i=0;i<4;i++) { 
   time(&timer);  /* get current time; same as: timer = time(NULL)  */

  seconds = difftime(timer,mktime(&y2k));
    binmasscenter(matrizlssplit[i], &mx, &ny, &kz,210, 281,102,100);
    //circle_levelset(matrizpartial, 158, 175, 51, 50, 210, 281,102);
    circle_levelset(matrizpartial, mx, ny, kz, 50, 210, 281,102);
    Boundary( matrizlssplit[i], matrizpartial,matriz2,c0,c1, 300,210, 281,102);


  copy4volume(matriz, matrizpartial, i,420,281,205);
   time(&timer);  /* get current time; same as: timer = time(NULL)  */

  seconds2 = difftime(timer,mktime(&y2k)); 
  tempo+=(seconds2-seconds);
    saveraw(argv[i+5],matrizpartial,210*102*281);
  }
    time(&timer);  /* get current time; same as: timer = time(NULL)  */

  seconds = difftime(timer,mktime(&y2k));
      Boundary( matrizls, matriz,matriz2,c0,c1, 300,420,281,205);
   time(&timer);  /* get current time; same as: timer = time(NULL)  */

  seconds2 = difftime(timer,mktime(&y2k)); 
  tempo+=(seconds2-seconds);
  
  saveraw(argv[3],matriz,420*205*281);
  
	

    printf ("%lf seconds\n", tempo);
	
	p=matriz;
	rewind(fp);
	while(!feof(fp)) {
		
		fread(&aux,sizeof(int),1,fp);
		*p=(unsigned char)aux;		
		p++;
		
	}
    	
   time(&timer);  /* get current time; same as: timer = time(NULL)  */

  seconds = difftime(timer,mktime(&y2k));
    //binmasscenter(matrizls, &mx, &ny, &kz,420, 281,205,100);
    //circle_levelset(matrizpartial, 158, 175, 51, 50, 210, 281,102);
    //circle_levelset(matrizpartial, mx, ny, kz, 50, 210, 281,102);
    Boundary( matrizls, matriz,matriz2,c0,c1, 300,420,281,205);
  time(&timer);  /* get current time; same as: timer = time(NULL)  */


  //copy4volume(matriz, matrizpartial, i,420,281,205);
  seconds2 = difftime(timer,mktime(&y2k)); 
	
    printf ("%lf seconds\n", seconds2-seconds);
	

	saveraw(argv[4],matriz,420*205*281);

	/*p=matriz;

	saveraw(argv[3],p,210*102*281);*/
	
    free(matrizls);
	free(matriz);
	free(matriz2);
	free (matrizpartial);
	fclose(fp);

	for(i=0;i<4;i++)
		free(matrizlssplit[i]);
	return 1;
	
}

int saveraw(char *name, unsigned char *p, unsigned int size) {
	int i;
	unsigned int aux;
	FILE *foutput;
	
	foutput=fopen(name, "wb");
	if(foutput==NULL) {
		printf("error opening file\n");
		exit(0);
	}


	for(i=0;i<(size);i++) {
		aux=(unsigned int)*p;
		fwrite(&aux, sizeof(int),1,foutput);
		p++;
	}
	fclose(foutput);
	

}



void splitbyfour(unsigned char *pls, unsigned char **plsvec, int m0, int n0, int k0 ) {
	
	/*
#define LARGURA 420 // m
#define ALTURA 281  // n
#define FATIAS 205 // k
*/

   unsigned char *p[4],*pdest[4];
   int n,k,i;
   int offset[4];
   offset[0]=0;
   offset[1]=(m0/2);
   offset[2]=m0*n0*(k0/2);
   offset[3]=offset[2]+offset[1];
   i=0;
   
   for(i=0;i<4;i++) { 
      pdest[i]=plsvec[i];
	  p[i]=pls+offset[i];
   }
	for (k=0;k<(k0/2);k++) {
		for(n=0;n<n0;n++) {
			for(i=0;i<4;i++) {
		     	memcpy(pdest[i],p[i],m0/2);
				pdest[i]+=(m0/2);
				p[i]+=m0;
			}
			
		}		
	}
	
}

int circle_levelset(unsigned char *p,int centerm, int centern, int centerk, int raio, int m0, int n0, int k0) {
	
	int msqraio,sqraio;
	sqraio=raio*raio;
	int m,n,k;
	
	
	for (k=0;k<k0;k++) {
		for(n=0;n<n0;n++) {
			for(m=0;m<m0;m++) {
				msqraio=0;
				msqraio+=((m-centerm)*(m-centerm));
				msqraio+=((n-centern)*(n-centern));
				msqraio+=((k-centerk)*(k-centerk));	
				if(msqraio<sqraio)
					*p=1;
				else
					*p=0;
				p++;
			}
		 }
	}
}

void binarize(unsigned char *p, unsigned char *pout,int m0, int n0, int k0, unsigned int limiar) {
		
	int m,n,k;
	
	
	for (k=0;k<k0;k++) {
		for(n=0;n<n0;n++) {
			for(m=0;m<m0;m++) {
				if(*p>limiar)
					*pout=1;
				else
					*pout=0;
				p++;
			}
		}
	}
	
	
}

void cleanexterior(void) {
	
	
}
void binmasscenter(unsigned char *p, int *pm, int *pn, int *pk,int m0, int n0, int k0, int limiar) {
		
	int m,n,k;
	
	int mx,ny,kz;
	
	int c;
	
	c=0;
	
	mx=0;
	ny=0;
	kz=0;
	
	for (k=0;k<k0;k++) {
		for(n=0;n<n0;n++) {
			for(m=0;m<m0;m++) {
				if(*p<limiar){
					mx+=m;
					ny+=n;
					kz+=k;
					c++;
				}
					
				p++;
				
			}
		}
	}
	
	mx/=c;
	ny/=c;
	kz/=c;
	
	*pm=mx;
	*pn=ny;
	*pk=kz;
	
}

void copy4volume(unsigned char *v, unsigned char *vsrc, int position, int m0, int n0, int k0){
   unsigned char *pv,*pvsrc;
   int n,k,i;
   int offset[4];
   offset[0]=0;
   offset[1]=((m0/2));
   offset[2]=m0*n0*((k0/2));
   offset[3]=offset[2]+offset[1];
   i=0;
   pv=v;
   pvsrc=vsrc;
   
   pv+=offset[position];
   
	for (k=0;k<(k0/2);k++) {
		for(n=0;n<n0;n++) {
			//for(i=0;i<4;i++) {
		     	memcpy(pv,pvsrc,m0/2);
				pvsrc+=(m0/2);
				pv+=m0;
			//}
		}		
	}
	
}

// funcao q faz um crop em uma imagem 3d 
void crop3d(void) {
	
	
}

// funcao que toma uma imagem bidimensional e apaga os pixels
// do interior do contor. faz isso seguindo as bordas de baixo para cima, de cima para baixo
// Isso pode substituir as operações morfológicas ?

void cleaninterior (void) {
	
	
	
}