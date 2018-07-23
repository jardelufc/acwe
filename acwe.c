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

  time_t timer;
  struct tm y2k = {0};
  double seconds,seconds2, tempo; 

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




int initimage(TImage *image, int m0, int n0, int k0, int datasize) {
	image->m0=m0;
	image->n0=n0;
	image->k0=k0;
	image->pdata=malloc(n0*m0*k0);
	image->datasize=datasize;
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
	//FILE *fp;
	FILE *fpls;
	
	unsigned char  *p,*matriz,*matriz2,*matrizjoint;
	unsigned char *pls,*matrizls;
	unsigned int aux;
	unsigned int i,j,k;
	unsigned long long c0;
	unsigned long long c1;
	unsigned char *matrizlssplit[64], *matrizpartial[64];
    int mx, ny, kz,x;
	char nome[100];
	float similar;

	i=j=k=0;
	
	x=4;

	fpls=fopen(argv[1], "rb");
	if(fpls==NULL) {
		printf("error opening file\n");
		exit(0);
	}
	/*fp=fopen(argv[2], "rb");
	if(fp==NULL) {
		printf("error opening file\n");
		exit(0);
	}*/

	
	matrizls=malloc(205*281*420*sizeof(char));
	if(matrizls==NULL) {
		printf("error memory allocation\n");
		exit(0);
	}
    matrizjoint=malloc(205*281*420*sizeof(char));
	if(matrizjoint==NULL) {
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
    /*matrizpartial=malloc(102*281*210*sizeof(char));
	if(matrizpartial==NULL) {
		printf("error memory allocation\n");
		exit(0);
	}*/
	
	for(i=0;i<(2*x);i++) {
		matrizlssplit[i]=malloc((205/x)*281*210*sizeof(char));
		if(matrizlssplit[i]==NULL) {
			printf("error memory allocation\n");
			exit(0);
		}
	}
	for(i=0;i<(2*x);i++) {
		matrizpartial[i]=malloc((205/x)*281*210*sizeof(char));
		if(matrizpartial[i]==NULL) {
			printf("error memory allocation\n");
			exit(0);
		}
	}
	/*p=matriz;
	
	while(!feof(fp)) {
		
		fread(&aux,sizeof(int),1,fp);
		*p=(unsigned char)aux;		
		p++;
		
	}*/
	memset(matriz,0,420*281*205);

    circle_levelset(matriz, 130, 100,100, 20,420,281,205);
    circle_levelset(matriz, 290, 100,100, 20,420,281,205);
	
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
	splitby2x(x,matrizls, matrizlssplit, 420, 281, 205 );
    binarize(matrizls, matrizjoint,420,281,205, 100);
	splitby2x(x,matrizjoint, matrizpartial, 420, 281, 205 );

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
   strcpy(nome,argv[2]);
  
  for(i=0;i<(2*x);i++) { 
   time(&timer);  /* get current time; same as: timer = time(NULL)  */

  seconds = difftime(timer,mktime(&y2k));
    //binmasscenter(matrizlssplit[i], &mx, &ny, &kz,210, 281,(205/x),100);
    printf ("x=%d y=%d z=%d raio %d\n", mx,ny,kz,205/(3*x));
	//circle_levelset(matrizpartial, 158, 175, 51, 50, 210, 281,102);
	memset(matrizpartial[i],0,210*281*(205/x));
    //circle_levelset(matrizpartial[i], mx, ny, kz, 100/x, 210, 281,(205/x));
    binarize(matrizlssplit[i], matrizpartial[i],210,281,205/x, 100);

    Boundary( matrizlssplit[i], matrizpartial[i],matriz2,c0,c1, 300,210, 281,(205/x));


  copy2xvolume(x,matrizjoint, matrizpartial[i], i,420,281,205);
   time(&timer);  /* get current time; same as: timer = time(NULL)  */

  seconds2 = difftime(timer,mktime(&y2k)); 
  tempo+=(seconds2-seconds);
  nome[strlen(nome)-5]=i+'0';
    saveraw(nome,matrizpartial[i],210*(205/x)*281);
  }
  
  
    time(&timer);  /* get current time; same as: timer = time(NULL)  */

  seconds = difftime(timer,mktime(&y2k));
      Boundary( matrizls, matrizjoint,matriz2,c0,c1, 300,420,281,205);
   time(&timer);  /* get current time; same as: timer = time(NULL)  */

  seconds2 = difftime(timer,mktime(&y2k)); 
  tempo+=(seconds2-seconds);
  
  saveraw(argv[2],matrizjoint,420*205*281);
  
	

    printf ("%lf seconds\n", tempo);
	
	/*p=matriz;
	rewind(fp);
	while(!feof(fp)) {
		
		fread(&aux,sizeof(int),1,fp);
		*p=(unsigned char)aux;		
		p++;
		
	}*/
    	
   time(&timer);  /* get current time; same as: timer = time(NULL)  */

  seconds = difftime(timer,mktime(&y2k));
    //binmasscenter(matrizls, &mx, &ny, &kz,420, 281,205,100);
	memset(matriz,0,420*281*205);

    //circle_levelset(matriz, 130, 100,100, 20,420,281,205);
    //circle_levelset(matriz, 290, 100,100, 20,420,281,205);
    binarize(matrizls, matriz,420,281,205, 100);
//cleanborder(matriz,420,281,205, 3);

    Boundary( matrizls, matriz,matriz2,c0,c1, 300,420,281,205);
	//cleanborder(matriz,420,281,205, 0);

  time(&timer);  /* get current time; same as: timer = time(NULL)  */


  //copy4volume(matriz, matrizpartial, i,420,281,205);
  seconds2 = difftime(timer,mktime(&y2k)); 
	
    printf ("%lf seconds\n", seconds2-seconds);
	similar=dsc(matriz,matrizjoint,205*420*281);
    printf ("dsc=%lf\n", similar);


	saveraw(argv[3],matriz,420*205*281);

	/*p=matriz;

	saveraw(argv[3],p,210*102*281);*/
	
    free(matrizls);
	free(matriz);
	free(matriz2);
	free(matrizjoint);
	//free (matrizpartial);
	//fclose(fp);

	for(i=0;i<(2*x);i++)
		free(matrizlssplit[i]);
	for(i=0;i<(2*x);i++)
		free(matrizpartial[i]);
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
				/*else
					*p=0;*/
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
					*pout=0;
				else
					*pout=1;
				p++;
				pout++;
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



// funcao que toma uma imagem bidimensional e apaga os pixels
// do interior do contor. faz isso seguindo as bordas de baixo para cima, de cima para baixo
// Isso pode substituir as operações morfológicas ?

void cleaninterior (void) {
	
	
	
}

TImage *readraw (char *name) {
	FILE *fp;
	unsigned char *p;
	unsigned int aux;
	fp=fopen(name, "rb");
	if(fp==NULL) {
		printf("error opening file\n");
		exit(0);
	}
	while(!feof(fp)) {
		
		fread(&aux,sizeof(int),1,fp);
	    *p=(unsigned char)aux;
		p++;		
	}	
	fclose(fp);	
}

void splitby2x(int x,unsigned char *pls, unsigned char **plsvec, int m0, int n0, int k0 ) {

   unsigned char *p[64],*pdest[64];
   int k,i,n;
   int offset[64];
   offset[0]=0;
   offset[1]=(m0/2);
   for(i=2;i<(2*x);i++) {
	  offset[i]=(i/2)*m0*n0*(k0/x)+offset[i%2];
   }
   
   i=0;
   
   for(i=0;i<(2*x);i++) { 
      pdest[i]=plsvec[i];
	  p[i]=pls+offset[i];
   }
	for (k=0;k<(k0/(x));k++) {
		for(n=0;n<n0;n++) {
			for(i=0;i<(2*x);i++) {
		     	memcpy(pdest[i],p[i],m0/2);
				pdest[i]+=(m0/2);
				p[i]+=m0;
			}
			
		}		
	}
}

void copy2xvolume(int x, unsigned char *v, unsigned char *vsrc, int position, int m0, int n0, int k0){
   unsigned char *pv,*pvsrc;
   int n,k,i;
   int offset[64];
   
   offset[0]=0;
   offset[1]=((m0/2));
   for(i=2;i<(2*x);i++) {
	  offset[i]=(i/2)*m0*n0*(k0/x)+offset[i%2];
   }
   i=0;
   pv=v;
   pvsrc=vsrc;
   
   pv+=offset[position];
   
	for (k=0;k<(k0/x);k++) {
		for(n=0;n<n0;n++) {
			//for(i=0;i<4;i++) {
		     	memcpy(pv,pvsrc,m0/2);
				pvsrc+=(m0/2);
				pv+=m0;
			//}
		}		
	}
	
}

// pre-segmentar e entregar o level set do binario apenas para limpar


// limiar=20
void cleanborder(unsigned char *img,int m0, int n0, int k0, int limiar) {
	
		   int c,i,j,aux,z,k;		   
		   //unsigned char img[512][512];

           c=0;
           i=0;
           j=0;
        for(k=0;k<k0;k++) {
           for (j=0;j<m0;j++){
              c=0;
              i=0;
              aux=0;
              while (i<n0) { //for i in range (0,511):
                 if (img[k*n0*m0+i*m0+j]==0){                  
                       if(aux>limiar)
                          i=n0;
			           aux++;
				}
                 else
                    aux=0;
				 
                 i++;
                 c++;
              }  		   	  
              for (z=0;z<c;z++) // in range(0,c):
                 img[k*n0*m0+z*m0+j] = 0;
              c=n0-1;
              i=n0-1;
              aux=0;
              while (i>=0) { //: #for i in range (0,511):
                 if (img[k*n0*m0+i*m0+j]==0) {                  
                    if(aux>limiar)
                       i=0;
                    aux++;
				 }
                 else
                    aux=0;				 
                 i--;
                 c--;
			  }		   	  
              for (z=c;z<n0;z++) // in range(c,512):
                 img[k*n0*m0+z*m0+j] = 0;
              c=0;
		   }
		}
}

float dsc(unsigned char *img,unsigned char *imggold,int size) {
	int k;
	int TP,FP,FN;
	float DSC;

	TP=0;
	FP=0;
	FN=0;
	
	for (k=0;k<(size);k++) {
		//for(n=0;n<n0;n++) {
			//for(m=0;m<m0;m++) {
			if(*img!=0 || *imggold!=0){
				if(*img==*imggold)
					TP++;
				else if (*imggold==0)
					FP++;
				else
					FN++;
			}
			img++;
		    imggold++;

			//}
		//}
	}
	DSC = ((float)TP+(float)TP)/((float)FP+(float)TP+(float)TP+(float)FN);
	return DSC;
}

void crop(TImage *t, TImage *tsrc, int im0, int in0, int ik0 ) {
	
	/*
#define LARGURA 420 // m
#define ALTURA 281  // n
#define FATIAS 205 // k
*/

   unsigned char *p,*psrc;
   int n,k,i,m0,m0src,n0,n0src,k0,k0src;
   int offset;
   m0src=tsrc->m0;
   n0src=tsrc->n0;
   k0src=tsrc->k0;
   m0=t->m0;
   n0=t->n0;
   k0=t->k0;
   
   p=t->pdata;
   psrc=tsrc->pdata;
   psrc+=ik0*n0src*m0src+in0*m0src+ik0;
   i=0;
   

	for (k=0;k<k0;k++) {
		for(n=0;n<n0;n++) {
			
		     	memcpy(p,psrc,m0);
				p+=m0;
				psrc+=m0src;
			
			
		}		
	}
	
}



