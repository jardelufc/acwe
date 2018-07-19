#define IMG_WIDTH_OR_COLS 205*281
#define IMG_HEIGHT_OR_ROWS 420

#define LARGURA 210 // m
#define ALTURA 281  // n
#define FATIAS 102 // k

#include <stdlib.h>
#include <string.h>

void Boundary( unsigned char *matrizLS, unsigned char *matriz,unsigned char *matriz2,unsigned long long c0,unsigned long long c1, int iteracoes){

    unsigned long long c2=0L; 
	unsigned long long c3=0L;
    unsigned int m=0;  
	unsigned int n=0; 
	int limite = 0;
	unsigned int k=0;
	
	unsigned int j=0;
	unsigned int p=0;

    unsigned char *pmatriz;
    unsigned char *pmatriz2;

 
	
	unsigned int idxPixel = 0;


	// inicializar varios ponteiros com p-LARGURA, P+LARGURA etc
	
for(int z = 0; z<iteracoes; z++) {        //iteracoes

	
    if(z%2 == 0){
        pmatriz =(unsigned char*) matriz2;
        pmatriz2 =(unsigned char*) matriz;
    }
    else{
        pmatriz = (unsigned char*) matriz;
        pmatriz2 =(unsigned char*) matriz2;
    }
	idxPixel = 0;
    c2=0L;
	c3=0L;
	p=0;
	j=0;

	
	// pula mn
	memset(pmatriz,0,LARGURA*ALTURA);
	pmatriz+=LARGURA*ALTURA;
	idxPixel+=LARGURA*ALTURA;
	
	for (k=0;k<(FATIAS-2);k++) {
		// pula m+1
		memset(pmatriz,0,LARGURA+1);
		pmatriz+=(LARGURA+1);
		idxPixel+=(LARGURA+1);

		for(n=0;n<(ALTURA-2);n++) {
		
			for(m=0;m<(LARGURA-2);m++) {
				// processa o pixel
				if (((pmatriz2[idxPixel-LARGURA] - pmatriz2[idxPixel+LARGURA])!=0) || ((pmatriz2[idxPixel+1] - pmatriz2[idxPixel-1])!=0) || ((pmatriz2[idxPixel+LARGURA*ALTURA] - pmatriz2[idxPixel-LARGURA*ALTURA])!=0)){

					limite =(((int)matrizLS[idxPixel]-(int)c1)*((int)matrizLS[idxPixel]-(int)c1)-2*((int)matrizLS[idxPixel]-(int)c0)*((int)matrizLS[idxPixel]-(int)c0));
					if(limite < 0) *pmatriz = 1;
					if(limite > 0) *pmatriz = 0;
				}
				if(*pmatriz == 0){
					c2+=(unsigned long long)matrizLS[idxPixel];
					p++;
				}

				if(*pmatriz > 0){
					c3 +=(unsigned long long)matrizLS[idxPixel];
					j++;
				}
							
				pmatriz++;
				idxPixel++;
			}
			//pula 2
			memset(pmatriz,0,2);
			pmatriz+=2;
			idxPixel+=2;

		}
		// pula m-1	
		memset(pmatriz,0,LARGURA-1);
		pmatriz+=(LARGURA-1);
		idxPixel+=(LARGURA-1);
	}
    c0=c2/(unsigned long long)p;
    c1=c3/(unsigned long long)j;
	
	
    /*if(z%2 == 0){
        pmatriz =(unsigned char*) matriz2;
        pmatriz2 =(unsigned char*) matriz;
    }
    else{
        pmatriz = (unsigned char*) matriz;
        pmatriz2 =(unsigned char*) matriz2;
    }
	idxPixel = 0;
    c2=0L;
	c3=0L;
	p=0;
	j=0;

	
	// pula mn
	memset(pmatriz,0,LARGURA*ALTURA);
	pmatriz+=LARGURA*ALTURA;
	idxPixel+=LARGURA*ALTURA;
	
	for (k=0;k<(FATIAS-2);k++) {
		// pula m+1
		memset(pmatriz,0,LARGURA+1);
		pmatriz+=(LARGURA+1);
		idxPixel+=(LARGURA+1);

		for(n=0;n<(ALTURA-2);n++) {
		
			for(m=0;m<(LARGURA-2);m++) {
				if(*pmatriz == 0){
					c2+=(unsigned long long)matrizLS[idxPixel];
					p++;
				}

				if(*pmatriz > 0){
					c3 +=(unsigned long long)matrizLS[idxPixel];
					j++;
				}
							
				pmatriz++;
				idxPixel++;
			}
			//pula 2
			memset(pmatriz,0,2);
			pmatriz+=2;
			idxPixel+=2;

		}
		// pula m-1	
		memset(pmatriz,0,LARGURA-1);
		pmatriz+=(LARGURA-1);
		idxPixel+=(LARGURA-1);
	}
    c0=c2/(unsigned long long)p;
    c1=c3/(unsigned long long)j;*/
	
}

}
