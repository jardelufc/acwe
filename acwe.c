
#include "acwe.h"



int initimage(TImage *image, int m0, int n0, int k0, int datasize) {
	image->m0=m0;
	image->n0=n0;
	image->k0=k0;
	image->pdata=malloc(n0*m0*k0);
	image->datasize=datasize;
	return 1; //(int) image->pdata;
}

void freeimage(TImage *image) {
	free(image->pdata);
}

void copyimage(TImage *dest, TImage *source) {
	//memcpy (dest->pdata,source->pdata,dest)
	
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

int readmhdraw (char *name, TImage *Image) {
      FILE *fp;
      unsigned char *p;
      unsigned int aux;

      char sz1[100],sz2[100],sz3[100],sz4[100];
      int n0,m0,k0;

      char szin[100]="DimSize = 512 512 121";

      FILE *f;
      unsigned char *pdata;

      f=fopen(name, "rb");
     if(f==NULL) {
		printf("error opening file\n");
		return -1;
     }
      while(!feof(f)) {
         fgets(szin,100,f);
         if(!memcmp(szin,"DimSize",7)) {
            sscanf(szin,"%s = %d %d %d",sz1,&n0,&m0,&k0);
            //printf("%s = %d %d %d",sz1,m0,n0,k0);
         }
         if(!memcmp(szin,"ElementDataFile",15)) {
            sscanf(szin,"%s = %s",sz1,sz2);
            //printf("%s = %d %d %d",sz1,m0,n0,k0);
         }


      }

     Image->n0=n0;
     Image->m0=m0;
     Image->k0=k0;
     
     pdata=malloc(n0*m0*k0);
     if(!pdata)
	return -1;
     Image->pdata=pdata;
     fp=fopen(sz2, "rb");
     if(fp==NULL) {
		printf("error opening file\n");
		return -1;
     }
     while(!feof(fp)) {
		
	fread(&aux,4,1,fp);
	*pdata=(unsigned char) aux;
	pdata++;		
     }	
    fclose(fp);	
    fclose(f);

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

void Boundary( unsigned char *matrizLS, unsigned char *matriz,unsigned char *matriz2,unsigned long long c0,unsigned long long c1, int iteracoes,int LARGURA, int ALTURA, int FATIAS){

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

    unsigned char aux;
	
    unsigned int idxPixel = 0;
    int ret,z;


    // inicializar varios ponteiros com p-LARGURA, P+LARGURA etc
    pmatriz =(unsigned char*) matriz;
    pmatriz2 =(unsigned char*) matriz;
    z=0;
    matriz2[0]=!matriz[0];
    ret=1;

    while(ret && z<iteracoes) {        //iteracoes

	
    /*if(z%2 == 0){
        pmatriz =(unsigned char*) matriz;
        pmatriz2 =(unsigned char*) matriz;
    }
    else{
        pmatriz = (unsigned char*) matriz;
        pmatriz2 =(unsigned char*) matriz2;
    }*/
	idxPixel = 0;
        c2=0L;
	c3=0L;
	p=0;
	j=0;

	
	// pula mn
	//memset(pmatriz,0,LARGURA*ALTURA);
	//pmatriz+=LARGURA*ALTURA;
	idxPixel+=LARGURA*ALTURA;
	
	for (k=0;k<(FATIAS-2);k++) {
		// pula m+1
		//memset(pmatriz,0,LARGURA+1);
		//pmatriz+=(LARGURA+1);
		idxPixel+=(LARGURA+1);

		for(n=0;n<(ALTURA-2);n++) {
		
			for(m=0;m<(LARGURA-2);m++) {
				aux=pmatriz[idxPixel];
				if(aux){
					c3 +=(unsigned long long)matrizLS[idxPixel];
					j++;

				} else {
				    c2+=(unsigned long long)matrizLS[idxPixel];
					p++;
					
				}
				// processa o pixel
				if (((pmatriz2[idxPixel-LARGURA] - pmatriz2[idxPixel+LARGURA])!=0) || ((pmatriz2[idxPixel+1] - pmatriz2[idxPixel-1])!=0) || ((pmatriz2[idxPixel+LARGURA*ALTURA] - pmatriz2[idxPixel-LARGURA*ALTURA])!=0)){

					limite =(((int)matrizLS[idxPixel]-(int)c1)*((int)matrizLS[idxPixel]-(int)c1)-2*((int)matrizLS[idxPixel]-(int)c0)*((int)matrizLS[idxPixel]-(int)c0));
					if(limite < 0) pmatriz[idxPixel] = 1;
					if(limite > 0) pmatriz[idxPixel] = 0;
				}

							
				//pmatriz++;
				idxPixel++;
			}
			//pula 2
			//memset(pmatriz,0,2);
			//pmatriz+=2;
			idxPixel+=2;

		}
		// pula m-1	
		//memset(pmatriz,0,LARGURA-1);
		//pmatriz+=(LARGURA-1);
		idxPixel+=(LARGURA-1);
	}
    c0=c2/(unsigned long long)p;
    c1=c3/(unsigned long long)j;
	if(!(z%10)) ret=memcmp(matriz2, matriz, LARGURA*ALTURA*FATIAS);
	if(!ret)
	printf ("\nterminou com %d iteracoes\n", z);
	z++;

	if(!(z%10)) memcpy(matriz2, matriz,LARGURA*ALTURA*FATIAS);
		
  }

}


