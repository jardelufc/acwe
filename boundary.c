#define IMG_WIDTH_OR_COLS 205*281
#define IMG_HEIGHT_OR_ROWS 420

#define LARGURA 420 // m
#define ALTURA 281  // n
#define FATIAS 205 // k

#include <stdlib.h>
#include <string.h>

void Boundary( unsigned char *matrizLS, unsigned char *matriz,unsigned char *matriz2,int c0,int c1, int iteracoes){

    int c2=0; int c3=0;
    int m=0;  int n=0; int limite = 0;
	int k=0;
	
	int j=0;
	int p=0;

    char *pmatriz;
    char *pmatriz2;

 
    int idxRow=0;int idxCol = 0;
    int pixProcessed=0;



    unsigned char pixelIn;
    unsigned char pixelInLS;
	
	int idxPixel = 0;


for(int z = 0; z<iteracoes; z++) {        //iteracoes

    c2=0;c3=0;p=0;j=0;
    if(z%2 == 0){
        pmatriz =(char*) matriz2;
        pmatriz2 =(char*) matriz;
    }
    else{
        pmatriz = (char*) matriz;
        pmatriz2 =(char*) matriz2;
    }

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
				if(pmatriz[idxPixel] == 0){
					c2=c2+matrizLS[idxPixel];
					p = p+1;
				}

				if(pmatriz[idxPixel] > 0){
					c3 = c3+matrizLS[idxPixel];
					j = j+1;
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
