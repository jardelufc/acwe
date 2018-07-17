#define IMG_WIDTH_OR_COLS 205*281
#define IMG_HEIGHT_OR_ROWS 420



void Boundary( unsigned char *matrizLS, unsigned char *matriz,unsigned char *matriz2,int c0,int c1, int iteracoes){

    int c2=0; int c3=0;
    int m=0;  int n=0; int limite = 0;


    int idxRow=0;int idxCol = 0;
    int pixProcessed=0;

    unsigned char pixelIn;
    unsigned char pixelInLS;

for(int z = 0; z< iteracoes; z++){        //iteracoes
    c2=0;c3=0;m=0;n=0;
        for(int idxPixel = 0; idxPixel < (IMG_WIDTH_OR_COLS*IMG_HEIGHT_OR_ROWS); idxPixel++){

            short valOutput =0;
            if(z%2 == 0){
                if(idxPixel <512 /*|| idxPixel > 261120 || idxPixel%512 == 0 || idxPixel%511 == 0*/){
                    matriz2[idxPixel] = 0;
                }
                else if(((matriz[idxPixel-512] - matriz[idxPixel+512])!=0) || ((matriz[idxPixel+1] - matriz[idxPixel-1])!=0)){
                    limite=0;

                    limite =( (matrizLS[idxPixel]-c1)*(matrizLS[idxPixel]-c1)-(matrizLS[idxPixel]-c0)*(matrizLS[idxPixel]-c0) );
                    if(limite < 0) matriz2[idxPixel] = 1;
                    if(limite > 0) matriz2[idxPixel] = 0;

                }
            }

            if(z%2 != 0){
                if(idxPixel <512 /*|| idxPixel > 261120 || idxPixel%512 == 0 || idxPixel%511 == 0*/){
                    matriz[idxPixel] = 0;
                }
                else if(((matriz2[idxPixel-512] - matriz2[idxPixel+512])!=0) || ((matriz2[idxPixel+1] - matriz2[idxPixel-1])!=0)){
                limite=0;

                limite =( (matrizLS[idxPixel]-c1)*(matrizLS[idxPixel]-c1)-(matrizLS[idxPixel]-c0)*(matrizLS[idxPixel]-c0) );
                if(limite < 0) matriz[idxPixel] = 1;
                if(limite > 0) matriz[idxPixel] = 0;

                }
            }


            if(z%2 == 0){
                if(matriz2[idxPixel] == 0){
                    c2=c2+matrizLS[idxPixel];
                    m = m+1;
                }
                if(matriz2[idxPixel] > 0){
                    c3 = c3+matrizLS[idxPixel];
                    n = n+1;
                }
            }

            if(z%2 != 0){
                if(matriz[idxPixel] == 0){
                    c2=c2+matrizLS[idxPixel];
                    m = m+1;
                }
                //else{
                if(matriz[idxPixel] > 0){
                    c3 = c3+matrizLS[idxPixel];
                    n = n+1;
                }
            }

        }


        c0=c2/m;
        c1=c3/n;
        //printf("c0 = %ld/%ld = %ld   c1 = %ld/%ld = %ld \n",c2,m,c0,c3,n,c1);
    }
}
