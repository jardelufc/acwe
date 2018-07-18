#include <stdio.h>
#include <malloc.h>
#include <string.h>

#include <time.h>       /* time_t, struct tm, difftime, time, mktime */
 

  time_t timer;
  struct tm y2k = {0};
  double seconds,seconds2; 

void Boundary( unsigned char *matrizLS, unsigned char *matriz,unsigned char *matriz2,unsigned long long c0,unsigned long long c1, int iteracoes);


int main(int argc, char **argv)
{
	FILE *fp;
	FILE *fpls;
	FILE *foutput;
	
	unsigned char  *p,*matriz,*matriz2;
	unsigned char *pls,*matrizls;
	unsigned int aux;
	unsigned int i,j,k;
	unsigned long long c0;
	unsigned long long c1;
	


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

	foutput=fopen(argv[3], "wb");
	if(foutput==NULL) {
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

  time(&timer);  /* get current time; same as: timer = time(NULL)  */

  seconds = difftime(timer,mktime(&y2k));

    Boundary( matrizls, matriz,matriz2,c0,c1, 333);
  time(&timer);  /* get current time; same as: timer = time(NULL)  */

  seconds2 = difftime(timer,mktime(&y2k));

  printf ("%.f seconds\n", seconds2-seconds);

	
	p=matriz;
	
	for(i=0;i<(420*205*281);i++) {
		aux=(unsigned int)*p;
		fwrite(&aux, sizeof(int),1,foutput);
		p++;
	}
	fclose(foutput);
    free(matrizls);
	free(matriz);
	free(matriz2);
	return 1;
	
	
}