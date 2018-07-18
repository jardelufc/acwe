#define IMG_WIDTH_OR_COLS 205*281
#define IMG_HEIGHT_OR_ROWS 420

#define LARGURA 420 // m
#define ALTURA 281  // n
#define FATIAS 205 // k

#include <stdlib.h>
#include <string.h>

void Boundary( unsigned char *matrizLS, unsigned char *matriz,unsigned char *matriz2,int c0,int c1, int iteracoes){

    unsigned int c2=0; 
	unsigned int c3=0;
    unsigned int m=0;  
	unsigned int n=0; 
	int limite = 0;
	unsigned int k=0;
	
	unsigned int j=0;
	unsigned int p=0;

    unsigned char *pmatriz;
    unsigned char *pmatriz2;

 
	
	unsigned int idxPixel = 0;


for(int z = 0; z<iteracoes; z++) {        //iteracoes

    c2=0;
	c3=0;
	p=0;
	j=0;
	
    if(z%2 == 0){
        pmatriz =(unsigned char*) matriz2;
        pmatriz2 =(unsigned char*) matriz;
    }
    else{
        pmatriz = (unsigned char*) matriz;
        pmatriz2 =(unsigned char*) matriz2;
    }
	idxPixel = 0;

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
				if (((pmatriz2[idxPixel-512] - pmatriz2[idxPixel+512])!=0) || ((pmatriz2[idxPixel+1] - pmatriz2[idxPixel-1])!=0) || ((pmatriz2[idxPixel+LARGURA*ALTURA] - pmatriz2[idxPixel-LARGURA*ALTURA])!=0)){

					limite =((matrizLS[idxPixel]-c1)*(matrizLS[idxPixel]-c1)-(matrizLS[idxPixel]-c0)*(matrizLS[idxPixel]-c0));
					if(limite < 0) *pmatriz = 1;
					if(limite > 0) *pmatriz = 0;
				}
				if(*pmatriz == 0){
					c2+=matrizLS[idxPixel];
					p++;
				}

				if(*pmatriz > 0){
					c3 +=matrizLS[idxPixel];
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
        c0=c2/p;
        c1=c3/j;
}

}
