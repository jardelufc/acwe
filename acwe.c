#include <stdio.h>
#include <malloc.h>
#include <string.h>

#include <time.h>       /* time_t, struct tm, difftime, time, mktime */
 

  time_t timer;
  struct tm y2k = {0};
  double seconds,seconds2; 

void Boundary( unsigned char *matrizLS, unsigned char *matriz,unsigned char *matriz2,unsigned long long c0,unsigned long long c1, int iteracoes);
void splitbyfour(unsigned char *pls, unsigned char **plsvec, int m0, int n0, int k0 );
int  saveraw(char *name, unsigned char *p, unsigned int size);
int circle_levelset(unsigned char *p,int centerm, int centern, int centerk, int raio, int m0, int n0, int k0);

int main(int argc, char **argv)
{
	FILE *fp;
	FILE *fpls;
	
	unsigned char  *p,*matriz,*matriz2;
	unsigned char *pls,*matrizls;
	unsigned int aux;
	unsigned int i,j,k;
	unsigned long long c0;
	unsigned long long c1;
	unsigned char *matrizlssplit[4];


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
	fclose(fp);
	fclose(fpls);
	
	memcpy(matriz2, matriz, 205*281*420*sizeof(char));
  y2k.tm_hour = 0;   y2k.tm_min = 0; y2k.tm_sec = 0;
  y2k.tm_year = 100; y2k.tm_mon = 0; y2k.tm_mday = 1;




    circle_levelset(matriz, 158, 175, 51, 50, 210, 281,102);

  time(&timer);  /* get current time; same as: timer = time(NULL)  */

  seconds = difftime(timer,mktime(&y2k));

    Boundary( matrizlssplit[0], matriz,matriz2,c0,c1, 177);
  time(&timer);  /* get current time; same as: timer = time(NULL)  */

  seconds2 = difftime(timer,mktime(&y2k));
    printf ("%.f seconds\n", seconds2-seconds);

	
	p=matriz;

	saveraw(argv[3],p,210*102*281);
	
    free(matrizls);
	free(matriz);
	free(matriz2);
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

void masscenter(void) {
	
	
}

void joint4volumes(void){
	
	
}
