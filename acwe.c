
#include "acwe.h"
#include <omp.h>
const unsigned char morphvecs[9][9]={
{0,1,2,12,13,14,24,25,26},
{6,7,8,12,13,14,18,19,20},
{0,4,8,9,13,17,18,22,26},
{2,5,8,10,13,16,18,21,24},
{2,4,6,11,13,15,20,22,24},
{0,3,6,10,13,16,20,23,26},
{9,10,11,12,13,14,15,16,17},
{1,4,7,10,13,16,19,22,25},
{3,4,5,12,13,14,21,22,23}};

void window3d(unsigned char *pw, TImage *Image, unsigned int pos) {
   int n0,m0;
   unsigned char *pdata;
   n0=Image->n0;
   m0=Image->m0;
   pdata=Image->pdata+pos;

   pw[0]=pdata[0];
   pw[1]=pdata[1];
   pw[2]=pdata[2];
   pdata+=m0;
   pw[3]=pdata[0];
   pw[4]=pdata[1];
   pw[5]=pdata[2];
   pdata+=m0;
   pw[6]=pdata[0];
   pw[7]=pdata[1];
   pw[8]=pdata[2];

   pdata=Image->pdata+n0*m0+pos;

   pw[9]=pdata[0];
   pw[10]=pdata[1];
   pw[11]=pdata[2];
   pdata+=m0;
   pw[12]=pdata[0];
   pw[13]=pdata[1];
   pw[14]=pdata[2];
   pdata+=m0;
   pw[15]=pdata[0];
   pw[16]=pdata[1];
   pw[17]=pdata[2];


   pdata=Image->pdata+2*n0*m0+pos;

   pw[18]=pdata[0];
   pw[19]=pdata[1];
   pw[20]=pdata[2];
   pdata+=m0;
   pw[21]=pdata[0];
   pw[22]=pdata[1];
   pw[23]=pdata[2];
   pdata+=m0;
   pw[24]=pdata[0];
   pw[25]=pdata[1];
   pw[26]=pdata[2];



}
unsigned char minmax9(unsigned char *p) {
   unsigned char x,y,i;
   unsigned const char *pop;
   y=1;
   pop=morphvecs[0];

   for (i=0;i<9;i++) {
      x=p[pop[0]] | p[pop[1]] | p[pop[2]] | p[pop[3]] | p[pop[4]] | p[pop[5]] | p[pop[6]] | p[pop[7]] | p[pop[8]] ;
      y=y&x;
      pop+=9;
   }
   return y;

}

unsigned char maxmin9(unsigned char *p) {
   unsigned char x,y,i;
   unsigned const char *pop;
   y=0;
   pop=morphvecs[0];

   for (i=0;i<9;i++) {
      x=p[pop[0]] & p[pop[1]] & p[pop[2]] & p[pop[3]] & p[pop[4]] & p[pop[5]] & p[pop[6]] & p[pop[7]] & p[pop[8]] ;
      y=y|x;
      pop+=9;
   }
   return y;

}



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

int saveraw(char *name, TImage *Image) {
	int i,size;
	unsigned int aux;
	FILE *foutput;
        unsigned char *p;

        p=Image->pdata;
        size=Image->m0*Image->n0*Image->k0;
	
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

int sphere_levelset(unsigned char *p,int centerm, int centern, int centerk, int raio, int m0, int n0, int k0) {
	
	int msqraio,sqraio;
	sqraio=raio*raio;
	int m,n,k;
	
	
	for (k=0;k<k0;k++) {
		for(n=0;n<n0;n++) {
			for(m=0;m<m0;m++) {
                                //printf("%d %d %d\n",m,n,k);
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
      // printf("Dedel");
      // fflush(stdout);
}

int ellipsoid_levelset(unsigned char *p,int centerm, int centern, int centerk, int raio, int m0, int n0, int k0, int a, int b, int c) {
	
	int msqraio,sqraio;
	sqraio=raio*raio;
	int m,n,k;
	
	
	for (k=0;k<k0;k++) {
		for(n=0;n<n0;n++) {
			for(m=0;m<m0;m++) {
				msqraio=0;
				msqraio+=((m-centerm)*(m-centerm)) /  (a*a);
				msqraio+=((n-centern)*(n-centern)) / (b*b);
				msqraio+=((k-centerk)*(k-centerk)) / (c*c);	
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


void erosion(unsigned char *p, unsigned char *pout,int m0, int n0, int k0) {
		

	int m,n,k,idxPixel;
	
	idxPixel=0;



	for (k=0;k<(k0);k++) {
		// pula m+1
		//memset(pmatriz,0,LARGURA+1);
		//pmatriz+=(LARGURA+1);
		idxPixel+=(m0+1);


		for(n=0;n<(n0-2);n++) {
		
			for(m=0;m<(m0-2);m++) {

				// processa o pixel
                               //window3d(window,Image,idxPixel-(LARGURA*ALTURA)-LARGURA-1);
                               pout[idxPixel]=p[idxPixel] | p[idxPixel-1] | p[idxPixel+1] | p[idxPixel-m0] | p[idxPixel-1-m0] | p[idxPixel+1-m0] | p[idxPixel+m0] | p[idxPixel-1+m0] | p[idxPixel+1+m0];
				//pmatriz2[idxPixel]=operacoemorfsi(pmatriz[idxPixel],pmatriz[idxPixel-512],pmatriz[idxPixel+512],pmatriz[idxPixel-1],pmatriz[idxPixel+1],pmatriz[idxPixel-511],pmatriz[idxPixel-513],pmatriz[idxPixel+513],pmatriz[idxPixel+511]);			
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
		idxPixel+=(m0-1);
	}
   



	
}
void cleanexterior(void) {
	
	
}
void binmasscenter(unsigned char *p, int *pm, int *pn, int *pk,int m0, int n0, int k0, int limiar) {
		
	int m,n,k;
	
	int mx,ny,kz;
	
	int c;
        unsigned char *pflag;
	
	c=0;
	
	mx=0;
	ny=0;
	kz=0;



       pflag=malloc(m0);

	
	for (k=0;k<k0;k++) {
       memset(pflag,0,m0);
		for(n=0;n<n0;n++) {
			for(m=0;m<m0;m++) {
				if(*p<limiar){
					if(pflag[m]){mx+=m;
					ny+=n;
					kz+=k;
					c++;}
				}else
				  pflag[m]=1;
					
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
      unsigned short shortaux;
      unsigned char *p;
      unsigned int aux;

      char sz1[100],sz2[100],sz3[100],sz4[100], szElType[100];
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
            sscanf(szin,"%s = %d %d %d",sz1,&m0,&n0,&k0);
            //printf("%s = %d %d %d",sz1,m0,n0,k0);
         }
         if(!memcmp(szin,"ElementDataFile",15)) {
            sscanf(szin,"%s = %s",sz1,sz2);
            //printf("%s = %d %d %d",sz1,m0,n0,k0);
         }
         if(!memcmp(szin,"ElementType",11)) {
            sscanf(szin,"%s = %s",sz1,szElType);
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
     strcpy(Image->szFileRawName,sz2);
     fp=fopen(sz2, "rb");
     if(fp==NULL) {
		printf("error opening file\n");
		return -1;
     }
    
    if(!memcmp(szElType,"MET_SHORT",9)) {
     Image->datasize=2;
     while(!feof(fp)) {
		
	fread(&shortaux,2,1,fp);
	*pdata=(unsigned char) (shortaux*(unsigned short)0xFF/(unsigned short)0xFFFF);
	pdata++;		
     }	
     } else {
     Image->datasize=4;
     while(!feof(fp)) {
		
	fread(&aux,4,1,fp);
	*pdata=(unsigned char) aux;
	pdata++;		
     }	
    }
    fclose(fp);	
    fclose(f);

}
/**

*/
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

          //for(k=0;k<k0;k++) { memset(img+k*n0*m0,1,n0*m0/2);}

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
/**
@brief return similaritie between two image vectors
*/
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


void density(TImage *t, unsigned char *level7,unsigned long long *pc0, unsigned long long *pc1) {
	
	/*
#define LARGURA 420 // m
#define ALTURA 281  // n
#define FATIAS 205 // k
*/

   unsigned char *p,*plevel7;
   int nc0,nc1,i,m0,n0,k0;
   unsigned long long c1,c0;

   m0=t->m0;
   n0=t->n0;
   k0=t->k0;
   
   printf("%d %d %d\n",m0,n0,k0);
fflush(stdout);
   p=t->pdata;
   plevel7=level7;
   c0=0;
   c1=0;
   nc0=0;
   nc1=0;

   i=0;
   

	for (i=0;i<(k0*n0*m0);i++) {
           if(*plevel7) {
              c1+=(unsigned long long) *p;
              nc1++;
           }
           else {
	      c0+=(unsigned long long) *p;
              nc0++;
           }

	   p++;
           plevel7++;
				
	}
    if(nc0) *pc0=c0/(unsigned long long) nc0;
    if(nc1) *pc1=c1/(unsigned long long) nc1;
	
}
void acwe(char *filename, TImage *Image, unsigned int maxiteracoes, int smoothing){

   TImage ImageLS;
   unsigned char  *matriz,*matriz2;

	unsigned long long c0;
	unsigned long long c1;
   int n0,m0,k0,size;

   readmhdraw (filename, &ImageLS);

   m0=ImageLS.m0;
   n0=ImageLS.n0;
   k0=ImageLS.k0;

   size=n0*m0*k0;

   matriz=malloc(size*sizeof(char));
   if(matriz==NULL) {
	printf("error memory allocation\n");
	exit(0);
   }
   matriz2=malloc(size*sizeof(char));
   if(matriz2==NULL) {
	printf("error memory allocation\n");
	exit(0);
   }
   memset(matriz,0,size);

    	//circle_levelset(matriz, 210, 140,41, 10,420,281,205);

   sphere_levelset(matriz, 200, 250,50, 20,m0,n0,k0);
   sphere_levelset(matriz, 300, 250,50, 20,m0,n0,k0);
//   sphere_levelset(matriz, 130, 100,100, 20,m0,n0,k0);
//   sphere_levelset(matriz, 290, 100,100, 20,m0,n0,k0);

    	//binarize(ImageLS.pdata, matriz2,ImageLS.m0,ImageLS.n0,ImageLS.k0, 100);
    	//erosion(matriz2, matriz,ImageLS.m0,ImageLS.n0,ImageLS.k0);
	//cleanborder(matriz,ImageLS.m0,ImageLS.n0,ImageLS.k0, 55);
        

   density(&ImageLS, matriz,&c0, &c1);
   printf("c0=%llu \n",c0);
   printf("c1=%llu \n",c1);
   Boundary( &ImageLS, matriz,matriz2,c0,c1, maxiteracoes,smoothing);
   erosion(matriz, matriz2,ImageLS.m0,ImageLS.n0,ImageLS.k0);
   Image->pdata=matriz2;
   Image->n0=ImageLS.n0;
   Image->m0=ImageLS.m0;
   Image->k0=ImageLS.k0;
   free(matriz2);
   free(ImageLS.pdata);

}
void acwex(int x, char *filename, TImage *Image, unsigned int maxiteracoes, int smoothing){

 TImage ImageLSSplit[64],ImageLS;
   unsigned char  *matriz;

	unsigned long long c0;
	unsigned long long c1;
   	int mx, ny, kz,i;

	unsigned char *matrizlssplit[64], *matrizpartial[64], *matriz2partial[64];
   int n0,m0,k0,size;

   readmhdraw (filename, &ImageLS);



   m0=ImageLS.m0;
   n0=ImageLS.n0;
   k0=ImageLS.k0;

   size=n0*m0*k0;

   matriz=malloc(size*sizeof(char));
   if(matriz==NULL) {
	printf("error memory allocation\n");
	exit(0);
   }

   memset(matriz,0,size);

	
    for(i=0;i<(2*x);i++) {
	matrizlssplit[i]=malloc((k0/x)*n0*(m0/2)*sizeof(char));
	if(matrizlssplit[i]==NULL) {
	   printf("error memory allocation\n");
	   exit(0);
       }
    }
    for(i=0;i<(2*x);i++) {
       matrizpartial[i]=malloc((k0/x)*n0*(m0/2)*sizeof(char));
       if(matrizpartial[i]==NULL) {
          printf("error memory allocation\n");
	   exit(0);
	}
    }   
    for(i=0;i<(2*x);i++) {
       matriz2partial[i]=malloc((k0/x)*n0*(m0/2)*sizeof(char));
       if(matriz2partial[i]==NULL) {
          printf("error memory allocation\n");
	   exit(0);
	}
    }      
   splitby2x(x,ImageLS.pdata, matrizlssplit, m0, n0, k0 );
   for(i=0;i<(2*x);i++) {
      ImageLSSplit[i].pdata=matrizlssplit[i];
      ImageLSSplit[i].m0=ImageLS.m0/2;
      ImageLSSplit[i].n0=ImageLS.n0;
      ImageLSSplit[i].k0=ImageLS.k0/(x);  
   }

 omp_set_num_threads( 2*x );

#pragma omp parallel default(shared) private(i,mx,ny,kz,c0,c1)
{
  	//for(i=0;i<(2*x);i++) { 
		i = omp_get_thread_num();
                printf("numt=%i\n",i);
                //i =  omp_get_num_threads();
    		binmasscenter(matrizlssplit[i], &mx, &ny, &kz,m0/2, n0,(k0/x),100);
    		printf ("x=%d y=%d z=%d raio %d\n", mx,ny,kz,k0/(3*x));
		//circle_levelset(matrizpartial, 158, 175, 51, 50, 210, 281,102);
		memset(matrizpartial[i],0,(m0/2)*n0*(k0/x));
    		ellipsoid_levelset(matrizpartial[i], mx, ny, kz, (k0/(5*x)), (m0/2), n0,k0/x,3,2,1 );
    		//binarize(matrizlssplit[i], matrizpartial[i],210,281,205/x, 100);
                density(&ImageLSSplit[i], matrizpartial[i],&c0, &c1);
    		Boundary(&ImageLSSplit[i], matrizpartial[i],matriz2partial[i],c0,c1, maxiteracoes,smoothing);

  		copy2xvolume(x,matriz, matrizpartial[i], i,m0,n0,k0);
  		//nome[strlen(nome)-5]=i+'0';
    		//saveraw(nome,matrizpartial[i],210*(205/x)*281);
  	//}
}

   //printf("c0=%llu \n",c0);
   //printf("c1=%llu \n",c1);
   //Boundary( &ImageLS, matriz,matriz2,c0,c1, maxiteracoes);
   Image->pdata=matriz;
   Image->n0=ImageLS.n0;
   Image->m0=ImageLS.m0;
   Image->k0=ImageLS.k0;
   strcpy(Image->szFileRawName,ImageLS.szFileRawName);
   //free(matriz2);
   free(ImageLS.pdata);
   for(i=0;i<(2*x);i++) {
      printf("%d\n",i);
      free(matrizlssplit[i]);
      free(matrizpartial[i]);
      free(matriz2partial[i]);
   }

}



void acwex2d(int x, char *filename, TImage *Image, unsigned int maxiteracoes, int smoothing){

 TImage ImageLSSplit[64],ImageLS,ImageLSSlice, ImageLSori;
   unsigned char  *matriz,*pdataLSSplit,*pmatrizpartial, *pmatriz2partial;

	unsigned long long c0;
	unsigned long long c1;
   	int mx, ny, kz,i,j;

	unsigned char *matrizlssplit[64], *matrizpartial[64], *matriz2partial[64];
   int n0,m0,k0,size,fatiasize;

   readmhdraw (filename, &ImageLS);


    	//initimage(&ImageLS, 420, 281, ImageLSori.k0, 1);
        //crop(&ImageLS, &ImageLSori, 40, 109, 0 );

   m0=ImageLS.m0;
   n0=ImageLS.n0;
   k0=ImageLS.k0;

   size=n0*m0*k0;

   matriz=malloc(size*sizeof(char));
   if(matriz==NULL) {
	printf("error memory allocation\n");
	exit(0);
   }

   memset(matriz,0,size);

	
    for(i=0;i<(2*x);i++) {
	matrizlssplit[i]=malloc((k0/x)*n0*(m0/2)*sizeof(char));
	if(matrizlssplit[i]==NULL) {
	   printf("error memory allocation\n");
	   exit(0);
       }
    }
    for(i=0;i<(2*x);i++) {
       matrizpartial[i]=malloc((k0/x)*n0*(m0/2)*sizeof(char));
       if(matrizpartial[i]==NULL) {
          printf("error memory allocation\n");
	   exit(0);
	}
    }   
    for(i=0;i<(2*x);i++) {
       matriz2partial[i]=malloc((k0/x)*n0*(m0/2)*sizeof(char));
       if(matriz2partial[i]==NULL) {
          printf("error memory allocation\n");
	   exit(0);
	}
    }      
   splitby2x(x,ImageLS.pdata, matrizlssplit, m0, n0, k0 );
   for(i=0;i<(2*x);i++) {
      ImageLSSplit[i].pdata=matrizlssplit[i];
      ImageLSSplit[i].m0=ImageLS.m0/2;
      ImageLSSplit[i].n0=ImageLS.n0;
      ImageLSSplit[i].k0=ImageLS.k0/(x);  
   }
  ImageLSSlice.m0=m0/2;
  ImageLSSlice.n0=n0;
  ImageLSSlice.k0=1;

   fatiasize=(m0/2)*n0;
// omp_set_num_threads( 2*x );

//#pragma omp parallel default(shared) private(i,mx,ny,kz,c0,c1,j,pdataLSSplit,pmatrizpartial,pmatriz2partial,ImageLSSlice)
//{
  	for(i=0;i<(2*x);i++) { 

	  // i = omp_get_thread_num();
           printf("numt=%i\n",i);
           pdataLSSplit=ImageLSSplit[i].pdata;
	   pmatrizpartial=matrizpartial[i];
	   pmatriz2partial=matriz2partial[i];
           for(j=0;j<(k0/x);j++) {
           //i =  omp_get_num_threads();
    	      binmasscenter(pdataLSSplit, &mx, &ny, &kz,m0/2, n0,1,100);
    	      printf ("x=%d y=%d z=%d raio %d\n", mx,ny,kz,k0/(3*x));
	      //circle_levelset(matrizpartial, 158, 175, 51, 50, 210, 281,102);
	      memset(pmatrizpartial,0,(m0/2)*n0);
    	      sphere_levelset(pmatrizpartial, mx, ny,0, 10, (m0/2), n0,1);

    	      //binarize(matrizlssplit[i], matrizpartial[i],210,281,205/x, 100);
              ImageLSSlice.pdata=pdataLSSplit;
              density(&ImageLSSlice, pmatrizpartial,&c0, &c1);
    	      Boundary2d(&ImageLSSlice, pmatrizpartial,pmatriz2partial,c0,c1, maxiteracoes,smoothing);
              pdataLSSplit+=fatiasize;
              pmatrizpartial+=fatiasize;
	      pmatriz2partial+=fatiasize;
           }
  	   copy2xvolume(x,matriz, matrizpartial[i], i,m0,n0,k0);
  	      //nome[strlen(nome)-5]=i+'0';
             //saveraw(nome,matrizpartial[i],210*(205/x)*281);
  	}
//}

   //printf("c0=%llu \n",c0);
   //printf("c1=%llu \n",c1);
   //Boundary( &ImageLS, matriz,matriz2,c0,c1, maxiteracoes);
   Image->pdata=matriz;
   Image->n0=ImageLS.n0;
   Image->m0=ImageLS.m0;
   Image->k0=ImageLS.k0;
   strcpy(Image->szFileRawName,ImageLS.szFileRawName);
   //free(matriz2);
   free(ImageLS.pdata);
   for(i=0;i<(2*x);i++) {
      printf("%d\n",i);
      free(matrizlssplit[i]);
      free(matrizpartial[i]);
      free(matriz2partial[i]);
   }

}


void Boundary( TImage *Image, unsigned char *matriz,unsigned char *matriz2,unsigned long long c0,unsigned long long c1, int iteracoes, int smoothing){

    unsigned char *matrizLS;
    int LARGURA,  ALTURA,  FATIAS;
    unsigned char window[27];


    unsigned long long c2=0L; 
    unsigned long long c3=0L;
    unsigned int m=0;  
    unsigned int n=0; 
    int limite = 0;
    unsigned int k=0;
	
    unsigned int j=0;
    unsigned int p=0;

    unsigned char *pmatriz;
    unsigned char *pmatriz2; //,*pmatriz3;

    unsigned char aux;
	
    unsigned int idxPixel,idxPixelStop;
    int ret,z;
    float newdsc,olddsc;

    newdsc=0.0;
    olddsc=1.0;

    idxPixel=0;
    matrizLS=Image->pdata;

    LARGURA=Image->m0;
    ALTURA=Image->n0;
    FATIAS=Image->k0;

   

  // pmatriz3=malloc(LARGURA*ALTURA*FATIAS);

    // inicializar varios ponteiros com p-LARGURA, P+LARGURA etc
    pmatriz =(unsigned char*) matriz;
    pmatriz2 =(unsigned char*) matriz;
    z=0;
    //matriz2[0]=!matriz[0];
    ret=1;

    while(ret && z<iteracoes) {        //iteracoes

	
    if(z%2 == 0){
        pmatriz =(unsigned char*) matriz2;
        pmatriz2 =(unsigned char*) matriz;
    }
    else{
        pmatriz = (unsigned char*) matriz;
        pmatriz2 =(unsigned char*) matriz2;
    }
	//idxPixel = 0;
        c2=0L;
	c3=0L;
	p=0;
	j=0;

	
	// pula mn
	//	memset(pmatriz,0,LARGURA*ALTURA);
	//pmatriz+=LARGURA*ALTURA;
	idxPixel=LARGURA*ALTURA;
	
	for (k=0;k<(FATIAS-2);k++) {
		// pula m+1
		//memset(pmatriz,0,LARGURA+1);
		//pmatriz+=(LARGURA+1);
		idxPixel+=(LARGURA+1);


		for(n=0;n<(ALTURA-2);n++) {
		
			for(m=0;m<(LARGURA-2);m++) {

				// processa o pixel
				if (((pmatriz2[idxPixel-LARGURA] - pmatriz2[idxPixel+LARGURA])!=0) || ((pmatriz2[idxPixel+1] - pmatriz2[idxPixel-1])!=0) || ((pmatriz2[idxPixel+LARGURA*ALTURA] - pmatriz2[idxPixel-LARGURA*ALTURA])!=0)){

					limite =(((int)matrizLS[idxPixel]-(int)c1)*((int)matrizLS[idxPixel]-(int)c1)-2*((int)matrizLS[idxPixel]-(int)c0)*((int)matrizLS[idxPixel]-(int)c0));
					if(limite < 0) pmatriz[idxPixel] = 1;
					if(limite > 0) pmatriz[idxPixel] = 0;
				}
				aux=pmatriz[idxPixel];
				if(aux){
					c3 +=(unsigned long long)matrizLS[idxPixel];
					j++;

				} else {
				    c2+=(unsigned long long)matrizLS[idxPixel];
					p++;
					
				}
				//pmatriz2[idxPixel]=operacoemorfsi(pmatriz[idxPixel],pmatriz[idxPixel-512],pmatriz[idxPixel+512],pmatriz[idxPixel-1],pmatriz[idxPixel+1],pmatriz[idxPixel-511],pmatriz[idxPixel-513],pmatriz[idxPixel+513],pmatriz[idxPixel+511]);			
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
    
    if(smoothing) {    
    idxPixel=LARGURA*ALTURA;
   if(z%2 == 0){
        pmatriz =(unsigned char*) matriz;
        pmatriz2 =(unsigned char*) matriz2;

    }

    else{

        pmatriz = (unsigned char*) matriz2;

        pmatriz2 =(unsigned char*) matriz;

    }

        Image->pdata=pmatriz2;
	for (k=0;k<(FATIAS-2);k++) {
		// pula m+1
		//memset(pmatriz,0,LARGURA+1);
		//pmatriz+=(LARGURA+1);
		idxPixel+=(LARGURA+1);
		idxPixelStop=idxPixel;

		for(n=0;n<(ALTURA-2);n++) {
		
			for(m=0;m<(LARGURA-2);m++) {

				// processa o pixel
                               window3d(window,Image,idxPixel-(LARGURA*ALTURA)-LARGURA-1);
                               pmatriz[idxPixel]=minmax9(window);
				//pmatriz2[idxPixel]=operacoemorfsi(pmatriz[idxPixel],pmatriz[idxPixel-512],pmatriz[idxPixel+512],pmatriz[idxPixel-1],pmatriz[idxPixel+1],pmatriz[idxPixel-511],pmatriz[idxPixel-513],pmatriz[idxPixel+513],pmatriz[idxPixel+511]);			
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
    
        idxPixel=LARGURA*ALTURA;
   if(z%2 == 0){
        pmatriz =(unsigned char*) matriz2;
        pmatriz2 =(unsigned char*) matriz;
    }
    else{
        pmatriz = (unsigned char*) matriz;
        pmatriz2 =(unsigned char*) matriz2;
    }
       Image->pdata=pmatriz2;

	for (k=0;k<(FATIAS-2);k++) {
		// pula m+1
		//memset(pmatriz,0,LARGURA+1);
		//pmatriz+=(LARGURA+1);
		idxPixel+=(LARGURA+1);
		idxPixelStop=idxPixel;

		for(n=0;n<(ALTURA-2);n++) {
		
			for(m=0;m<(LARGURA-2);m++) {

				// processa o pixel
                               window3d(window,Image,idxPixel-(LARGURA*ALTURA)-LARGURA-1);
                               pmatriz[idxPixel]=maxmin9(window);
				//pmatriz2[idxPixel]=operacoemorfsi(pmatriz[idxPixel],pmatriz[idxPixel-512],pmatriz[idxPixel+512],pmatriz[idxPixel-1],pmatriz[idxPixel+1],pmatriz[idxPixel-511],pmatriz[idxPixel-513],pmatriz[idxPixel+513],pmatriz[idxPixel+511]);			
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
   }

        c0=c2/(unsigned long long)p;
        c1=c3/(unsigned long long)j;
	//if(!(z%10))        {
            newdsc=dsc(pmatriz,pmatriz2,LARGURA*ALTURA*FATIAS);
            printf("\n%lf",newdsc); //ret=memcmp(pmatriz3, pmatriz, LARGURA*ALTURA*FATIAS);
            /*if(((newdsc-olddsc)*(newdsc-olddsc))<0.0000000001)
               ret=0;
            olddsc=newdsc;*/

        //}
	//if(!ret)
	printf ("\n%d iteracoes\n", z);
	z++;

	/*if(!(z%10)) {
           memcpy(pmatriz3, pmatriz,LARGURA*ALTURA*FATIAS);
        }*/

		
  }
  //Image->pdata=matrizLS;

}


void Boundary2d( TImage *Image, unsigned char *matriz,unsigned char *matriz2,unsigned long long c0,unsigned long long c1, int iteracoes, int smoothing){

    unsigned char *matrizLS;
    int LARGURA,  ALTURA,  FATIAS;
    unsigned char window[27];


    unsigned long long c2=0L; 
    unsigned long long c3=0L;
    unsigned int m=0;  
    unsigned int n=0; 
    int limite = 0;
    unsigned int k=0;
	
    unsigned int j=0;
    unsigned int p=0;

    unsigned char *pmatriz;
    unsigned char *pmatriz2; //,*pmatriz3;

    unsigned char aux;
	
    unsigned int idxPixel,idxPixelStop,c;
    int ret,z;
    float newdsc,olddsc;

    newdsc=0.0;
    olddsc=1.0;

    idxPixel=0;
    matrizLS=Image->pdata;

    LARGURA=Image->m0;
    ALTURA=Image->n0;
    FATIAS=Image->k0;

   

  // pmatriz3=malloc(LARGURA*ALTURA*FATIAS);

    // inicializar varios ponteiros com p-LARGURA, P+LARGURA etc
        pmatriz =(unsigned char*) matriz;
        pmatriz2 =(unsigned char*) matriz2;
    z=0;
    //matriz2[0]=!matriz[0];
    ret=1;

    while(ret && z<iteracoes) {        //iteracoes

	
    /*if(z%2 == 0){
        pmatriz =(unsigned char*) matriz2;
        pmatriz2 =(unsigned char*) matriz;
    }
    else{
        pmatriz = (unsigned char*) matriz;
        pmatriz2 =(unsigned char*) matriz2;
    }*/swap((unsigned char **)&pmatriz,(unsigned char **)&pmatriz2);
	//idxPixel = 0;
        c2=0L;
	c3=0L;
	p=0;
	j=0;

	
	// pula mn
	//	memset(pmatriz,0,LARGURA*ALTURA);
	//pmatriz+=LARGURA*ALTURA;
	//idxPixel=LARGURA*ALTURA;
	
	/*for (k=0;k<(FATIAS-2);k++) {
		// pula m+1
		//memset(pmatriz,0,LARGURA+1);
		//pmatriz+=(LARGURA+1);*/
		idxPixel=(LARGURA+1);


		for(n=0;n<(ALTURA-2);n++) {
		
			for(m=0;m<(LARGURA-2);m++) {
                                //printf("%d %d\n",m,n);
				// processa o pixel
				if (((pmatriz2[idxPixel-LARGURA] - pmatriz2[idxPixel+LARGURA])!=0) || ((pmatriz2[idxPixel+1] - pmatriz2[idxPixel-1])!=0)	){

					limite =(((int)matrizLS[idxPixel]-(int)c1)*((int)matrizLS[idxPixel]-(int)c1)-2*((int)matrizLS[idxPixel]-(int)c0)*((int)matrizLS[idxPixel]-(int)c0));
					if(limite < 0) pmatriz[idxPixel] = 1;
					if(limite > 0) pmatriz[idxPixel] = 0;
				}
				aux=pmatriz[idxPixel];
				if(aux){
					c3 +=(unsigned long long)matrizLS[idxPixel];
					j++;

				} else {
				    c2+=(unsigned long long)matrizLS[idxPixel];
					p++;
					
				}
				//pmatriz2[idxPixel]=operacoemorfsi(pmatriz[idxPixel],pmatriz[idxPixel-512],pmatriz[idxPixel+512],pmatriz[idxPixel-1],pmatriz[idxPixel+1],pmatriz[idxPixel-511],pmatriz[idxPixel-513],pmatriz[idxPixel+513],pmatriz[idxPixel+511]);			
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
	




 
	//idxPixel = 0;


	
	// pula mn
	//	memset(pmatriz,0,LARGURA*ALTURA);
	//pmatriz+=LARGURA*ALTURA;
	//idxPixel=LARGURA*ALTURA;
	
	/*for (k=0;k<(FATIAS-2);k++) {
		// pula m+1

		//memset(pmatriz,0,LARGURA+1);

		//pmatriz+=(LARGURA+1);*/
    if(smoothing) { 
   /*if(z%2 == 0){
        pmatriz =(unsigned char*) matriz;
        pmatriz2 =(unsigned char*) matriz2;
    }
    else{
        pmatriz = (unsigned char*) matriz2;
        pmatriz2 =(unsigned char*) matriz;


    }*/ 
      c=0;
     while(c<smoothing) {
    swap((unsigned char **)&pmatriz,(unsigned char **)&pmatriz2);   
		idxPixel=(LARGURA+1);


		for(n=0;n<(ALTURA-2);n++) {
		
			for(m=0;m<(LARGURA-2);m++) {


				pmatriz[idxPixel] = (pmatriz2[idxPixel] | pmatriz2[idxPixel-LARGURA] | pmatriz2[idxPixel+LARGURA]) &  (pmatriz2[idxPixel] | pmatriz2[idxPixel-1] | pmatriz2[idxPixel+1]) & (pmatriz2[idxPixel] | pmatriz2[idxPixel-LARGURA-1] | pmatriz2[idxPixel+LARGURA+1]) & (pmatriz2[idxPixel] | pmatriz2[idxPixel-LARGURA+1] | pmatriz2[idxPixel+LARGURA-1]);


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

   /*if(z%2 == 0){
        pmatriz =(unsigned char*) matriz2;
        pmatriz2 =(unsigned char*) matriz;
    }
    else{
        pmatriz = (unsigned char*) matriz;
        pmatriz2 =(unsigned char*) matriz2;
    } */swap((unsigned char **)&pmatriz,(unsigned char **)&pmatriz2);  
		idxPixel=(LARGURA+1);


		for(n=0;n<(ALTURA-2);n++) {
		
			for(m=0;m<(LARGURA-2);m++) {


				pmatriz[idxPixel] = (pmatriz2[idxPixel] & pmatriz2[idxPixel-LARGURA] & pmatriz2[idxPixel+LARGURA]) |  (pmatriz2[idxPixel] & pmatriz2[idxPixel-1] & pmatriz2[idxPixel+1]) | (pmatriz2[idxPixel] & pmatriz2[idxPixel-LARGURA-1] & pmatriz2[idxPixel+LARGURA+1]) | (pmatriz2[idxPixel] & pmatriz2[idxPixel-LARGURA+1] & pmatriz2[idxPixel+LARGURA-1]);


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



   /*if(z%2 == 0){
        pmatriz =(unsigned char*) matriz;
        pmatriz2 =(unsigned char*) matriz2;
    }
    else{
        pmatriz = (unsigned char*) matriz2;
        pmatriz2 =(unsigned char*) matriz;

    }  */

     c++;
   if(!(c<smoothing))
      break;
  swap((unsigned char **)&pmatriz,(unsigned char **)&pmatriz2); 
		idxPixel=(LARGURA+1);


		for(n=0;n<(ALTURA-2);n++) {
		
			for(m=0;m<(LARGURA-2);m++) {
				pmatriz[idxPixel] = (pmatriz2[idxPixel] & pmatriz2[idxPixel-LARGURA] & pmatriz2[idxPixel+LARGURA]) |  (pmatriz2[idxPixel] & pmatriz2[idxPixel-1] & pmatriz2[idxPixel+1]) | (pmatriz2[idxPixel] & pmatriz2[idxPixel-LARGURA-1] & pmatriz2[idxPixel+LARGURA+1]) | (pmatriz2[idxPixel] & pmatriz2[idxPixel-LARGURA+1] & pmatriz2[idxPixel+LARGURA-1]);




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

  /* if(z%2 == 0){
        pmatriz =(unsigned char*) matriz2;
        pmatriz2 =(unsigned char*) matriz;
    }
    else{
        pmatriz = (unsigned char*) matriz;
        pmatriz2 =(unsigned char*) matriz2;
    }*/

   swap((unsigned char **)&pmatriz,(unsigned char **)&pmatriz2);   
		idxPixel=(LARGURA+1);


		for(n=0;n<(ALTURA-2);n++) {
		
			for(m=0;m<(LARGURA-2);m++) {

				pmatriz[idxPixel] = (pmatriz2[idxPixel] | pmatriz2[idxPixel-LARGURA] | pmatriz2[idxPixel+LARGURA]) &  (pmatriz2[idxPixel] | pmatriz2[idxPixel-1] | pmatriz2[idxPixel+1]) & (pmatriz2[idxPixel] | pmatriz2[idxPixel-LARGURA-1] | pmatriz2[idxPixel+LARGURA+1]) & (pmatriz2[idxPixel] | pmatriz2[idxPixel-LARGURA+1] | pmatriz2[idxPixel+LARGURA-1]);



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
               c++;
    }

/*
    swap((unsigned char **)&pmatriz,(unsigned char **)&pmatriz2); 
		idxPixel=(LARGURA+1);


		for(n=0;n<(ALTURA-2);n++) {
		
			for(m=0;m<(LARGURA-2);m++) {

				pmatriz[idxPixel] = (pmatriz2[idxPixel] | pmatriz2[idxPixel-LARGURA] | pmatriz2[idxPixel+LARGURA]) &  (pmatriz2[idxPixel] | pmatriz2[idxPixel-1] | pmatriz2[idxPixel+1]) & (pmatriz2[idxPixel] | pmatriz2[idxPixel-LARGURA-1] | pmatriz2[idxPixel+LARGURA+1]) & (pmatriz2[idxPixel] | pmatriz2[idxPixel-LARGURA+1] | pmatriz2[idxPixel+LARGURA-1]);



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

  swap((unsigned char **)&pmatriz,(unsigned char **)&pmatriz2); 
		idxPixel=(LARGURA+1);


		for(n=0;n<(ALTURA-2);n++) {
		
			for(m=0;m<(LARGURA-2);m++) {


				pmatriz[idxPixel] = (pmatriz2[idxPixel] & pmatriz2[idxPixel-LARGURA] & pmatriz2[idxPixel+LARGURA]) |  (pmatriz2[idxPixel] & pmatriz2[idxPixel-1] & pmatriz2[idxPixel+1]) | (pmatriz2[idxPixel] & pmatriz2[idxPixel-LARGURA-1] & pmatriz2[idxPixel+LARGURA+1]) | (pmatriz2[idxPixel] & pmatriz2[idxPixel-LARGURA+1] & pmatriz2[idxPixel+LARGURA-1]);



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

*/


    }





   /* 
    if(smoothing) {    
    idxPixel=LARGURA*ALTURA;
   if(z%2 == 0){
        pmatriz =(unsigned char*) matriz;
        pmatriz2 =(unsigned char*) matriz2;

    }

    else{

        pmatriz = (unsigned char*) matriz2;

        pmatriz2 =(unsigned char*) matriz;

    }

        Image->pdata=pmatriz2;
	for (k=0;k<(FATIAS-2);k++) {
		// pula m+1
		//memset(pmatriz,0,LARGURA+1);
		//pmatriz+=(LARGURA+1);
		idxPixel+=(LARGURA+1);
		idxPixelStop=idxPixel;

		for(n=0;n<(ALTURA-2);n++) {
		
			for(m=0;m<(LARGURA-2);m++) {

				// processa o pixel
                               window3d(window,Image,idxPixel-(LARGURA*ALTURA)-LARGURA-1);
                               pmatriz[idxPixel]=minmax9(window);
				//pmatriz2[idxPixel]=operacoemorfsi(pmatriz[idxPixel],pmatriz[idxPixel-512],pmatriz[idxPixel+512],pmatriz[idxPixel-1],pmatriz[idxPixel+1],pmatriz[idxPixel-511],pmatriz[idxPixel-513],pmatriz[idxPixel+513],pmatriz[idxPixel+511]);			
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
    
        idxPixel=LARGURA*ALTURA;
   if(z%2 == 0){
        pmatriz =(unsigned char*) matriz2;
        pmatriz2 =(unsigned char*) matriz;
    }
    else{
        pmatriz = (unsigned char*) matriz;
        pmatriz2 =(unsigned char*) matriz2;
    }
       Image->pdata=pmatriz2;

	for (k=0;k<(FATIAS-2);k++) {
		// pula m+1
		//memset(pmatriz,0,LARGURA+1);
		//pmatriz+=(LARGURA+1);
		idxPixel+=(LARGURA+1);
		idxPixelStop=idxPixel;

		for(n=0;n<(ALTURA-2);n++) {
		
			for(m=0;m<(LARGURA-2);m++) {

				// processa o pixel
                               window3d(window,Image,idxPixel-(LARGURA*ALTURA)-LARGURA-1);
                               pmatriz[idxPixel]=maxmin9(window);
				//pmatriz2[idxPixel]=operacoemorfsi(pmatriz[idxPixel],pmatriz[idxPixel-512],pmatriz[idxPixel+512],pmatriz[idxPixel-1],pmatriz[idxPixel+1],pmatriz[idxPixel-511],pmatriz[idxPixel-513],pmatriz[idxPixel+513],pmatriz[idxPixel+511]);			
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
  // }

   */     
       if(p) c0=c2/(unsigned long long)p;
       if(j)  c1=c3/(unsigned long long)j;
	//if(!(z%10))        {
            //newdsc=dsc(pmatriz,pmatriz2,LARGURA*ALTURA);
            //printf("\n%lf",newdsc); //ret=memcmp(pmatriz3, pmatriz, LARGURA*ALTURA*FATIAS);
            /*if(((newdsc-olddsc)*(newdsc-olddsc))<0.0000000001)
               ret=0;
            olddsc=newdsc;*/

        //}
	//if(!ret)
	//printf ("\n%d iteracoes\n", z);
	z++;

	/*if(!(z%10)) {
           memcpy(pmatriz3, pmatriz,LARGURA*ALTURA*FATIAS);
        }*/

		
  }
  //Image->pdata=matrizLS;

}

void swap (unsigned char **p1, unsigned char **p2) {
   unsigned char *ptemp;
   ptemp=*p1;
   *p1=*p2;
   *p2=ptemp;
}

int min(int a, int b, int c){
    if( a<b){
        if(a<c) return a;
        else return c;
    }
    else{
        if(b<c) return b;
        else return c;
    }
}

int max(int a, int b, int c){
    if( a>b){
        if(a>c) return a;
        else return c;
    }
    else{
        if(b>c) return b;
        else return c;
    }
}

int maxsi(int a, int b, int c, int d){
    int l = max(a,b,c);
    if(d>l) return d;
    else return l;
}

int minis(int a, int b, int c, int d){
    int l = min(a,b,c);
    if(l<d) return l;
    else return d;
}

int operacoemorfsi(int pixel, int up, int down, int left, int right, int upright, int upleft, int downright, int downleft){
    int a = min(up,pixel,down);
    int b = min(left,pixel,right);
    int c = min(downleft,pixel,upright);
    int d = min(upleft,pixel,downright);

    int e = maxsi(a,b,c,d);
    return e;
}

int operacoemorfis(int pixel, int up, int down, int left, int right, int upright, int upleft, int downright, int downleft){
    int a = max(up,pixel,down);
    int b = max(left,pixel,right);
    int c = max(downleft,pixel,upright);
    int d = max(upleft,pixel,downright);

    int e = minis(a,b,c,d);
    return e;
}














