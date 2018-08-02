#include "acwe.h"
 

int main(int argc, char **argv)
{
	//FILE *fp;
	FILE *fpls;

        TImage ImageLS, Image;
	
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
	time_t timer;
  	struct tm y2k = {0};
  	double seconds,seconds2, tempo; 
        int iteracoes;

        sscanf(argv[4],"%d",&iteracoes);
        sscanf(argv[5],"%d",&x);
 	

	i=j=k=0;
	
	//x=4;

	//readmhdraw (&argv[1][0], &ImageLS);
        //printf ("%d %d %d", ImageLS.m0,ImageLS.n0,ImageLS.k0);
        //fflush(stdout);
	/*fpls=fopen(argv[1], "rb");
	if(fpls==NULL) {
		printf("error opening file\n");
		exit(0);
	}*/
	/*fp=fopen(argv[2], "rb");
	if(fp==NULL) {
		printf("error opening file\n");
		exit(0);
	}*/

	
	/*matrizls= ImageLS.pdata ; /*malloc(205*281*420*sizeof(char));
	if(matrizls==NULL) {
		printf("error memory allocation\n");
		exit(0);
	}*/
    	/*matrizjoint=malloc(205*281*420*sizeof(char));
	if(matrizjoint==NULL) {
		printf("error memory allocation\n");
		exit(0);
	}*/		
    	/*matriz=malloc(205*281*420*sizeof(char));
	if(matriz==NULL) {
		printf("error memory allocation\n");
		exit(0);
	}
    	matriz2=malloc(205*281*420*sizeof(char));
	if(matriz2==NULL) {
		printf("error memory allocation\n");
		exit(0);
	}*/
    /*matrizpartial=malloc(102*281*210*sizeof(char));
	if(matrizpartial==NULL) {
		printf("error memory allocation\n");
		exit(0);
	}*/
	
	/*for(i=0;i<(2*x);i++) {
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
	}*/
	/*p=matriz;
	
	while(!feof(fp)) {
		
		fread(&aux,sizeof(int),1,fp);
		*p=(unsigned char)aux;		
		p++;
		
	}*/
	/*memset(matriz,0,420*281*205);

   	circle_levelset(matriz, 130, 100,100, 20,420,281,205);
    	circle_levelset(matriz, 290, 100,100, 20,420,281,205);
	
	pls=matrizls;
	p=matriz;

	c0=0L;
	c1=0L;
	j=0;
	k=0;*/
	/*while(!feof(fpls)) {
		
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
		
	}*/
        /*c0=50;
	c1=100;
        j=1;
        k=1;
	splitby2x(x,matrizls, matrizlssplit, 420, 281, 205 );
    	binarize(matrizls, matrizjoint,420,281,205, 100);
	splitby2x(x,matrizjoint, matrizpartial, 420, 281, 205 );

	printf("c0=%llu numero de amostras= %d e sua media= %lld \n",c0,j,c0/(unsigned long long)j);
    	printf("c1=%llu numero de amostras= %d e sua media: %lld \n",c1,k,c1/(unsigned long long)k);
	
    	c0=c0/(unsigned long long)j;
    	c1=c1/(unsigned long long)k;
	
	pls=matrizls;	
	//printf("alo mundo %d %d %d %d\n", i ,j ,k,matrizls[0]);
	fflush(stdout);
	//fclose(fp);
	//fclose(fpls);
	
	memcpy(matriz2, matriz, 205*281*420*sizeof(char));
  	y2k.tm_hour = 0;   y2k.tm_min = 0; y2k.tm_sec = 0;
  	y2k.tm_year = 100; y2k.tm_mon = 0; y2k.tm_mday = 1;

	tempo=0.0;
	strcpy(nome,argv[2]);
  
  	for(i=0;i<(2*x);i++) { 
   		time(&timer);  

  		seconds = difftime(timer,mktime(&y2k));
    		//binmasscenter(matrizlssplit[i], &mx, &ny, &kz,210, 281,(205/x),100);
    		printf ("x=%d y=%d z=%d raio %d\n", mx,ny,kz,205/(3*x));
		//circle_levelset(matrizpartial, 158, 175, 51, 50, 210, 281,102);
		memset(matrizpartial[i],0,210*281*(205/x));
    		//circle_levelset(matrizpartial[i], mx, ny, kz, 100/x, 210, 281,(205/x));
    		binarize(matrizlssplit[i], matrizpartial[i],210,281,205/x, 100);

    		//Boundary( matrizlssplit[i], matrizpartial[i],matriz2,c0,c1, 0,210, 281,(205/x));


  		copy2xvolume(x,matrizjoint, matrizpartial[i], i,420,281,205);
   		time(&timer);  

  		seconds2 = difftime(timer,mktime(&y2k)); 
  		tempo+=(seconds2-seconds);
  		nome[strlen(nome)-5]=i+'0';
    		saveraw(nome,matrizpartial[i],210*(205/x)*281);
  	}
  
  
    	time(&timer); 

 	seconds = difftime(timer,mktime(&y2k));
      	//Boundary( &ImageLS, matrizjoint,matriz2,c0,c1, 0);
   	time(&timer);  

  	seconds2 = difftime(timer,mktime(&y2k)); 
  	tempo+=(seconds2-seconds);
  
  	saveraw(argv[2],matrizjoint,420*205*281);
  
	

    	printf ("%lf seconds\n", tempo);*/
	
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
	//memset(matriz,0,420*281*205);

    	//circle_levelset(matriz, 210, 140,41, 10,420,281,205);

    	//circle_levelset(matriz, 130, 100,100, 20,420,281,205);
    	//circle_levelset(matriz, 290, 100,100, 20,420,281,205);
    	//binarize(matrizls, matriz,420,281,205, 100);
	//cleanborder(matriz,420,281,205, 3);
        
        //c1=100;
        //c0=50;
        //density(&ImageLS, matriz,&c0, &c1);
	//printf("c0=%llu \n",c0);
    	//printf("c1=%llu \n",c1);
    	//Boundary( &ImageLS, matriz,matriz2,c0,c1, iteracoes);
        acwex(x,&argv[1][0], &Image, iteracoes);
	//cleanborder(matriz,420,281,205, 0);

  	time(&timer);  /* get current time; same as: timer = time(NULL)  */


  	//copy4volume(matriz, matrizpartial, i,420,281,205);
  	seconds2 = difftime(timer,mktime(&y2k)); 
	
    	printf ("%lf seconds\n", seconds2-seconds);
	/*similar=dsc(matriz,matrizjoint,205*420*281);
    	printf ("dsc=%lf\n", similar);*/

        
	saveraw(argv[3],Image.pdata,420*205*281);

	/*p=matriz;

	saveraw(argv[3],p,210*102*281);*/
	
    	//free(matrizls);
	//free(matriz);
	//free(matriz2);
	//free(matrizjoint);
	//free (matrizpartial);
	//fclose(fp);

	/*for(i=0;i<(2*x);i++)
		free(matrizlssplit[i]);
	for(i=0;i<(2*x);i++)
		free(matrizpartial[i]);*/
        system("python raw2npy.py");
        system("python mostrarnpy.py");
	return 1;
	
}

