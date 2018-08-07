#include "acwe.h"
 

int main(int argc, char **argv)
{


        TImage ImageLS, Image;

   	int x;
	float similar;
	time_t timer;
  	struct tm y2k = {0};
  	double seconds,seconds2, tempo; 
        int iteracoes,smoothing;
        char szCommand[200];
	
	// read max number if iterations from command line parameters
        //sscanf(argv[3],"%d",&iteracoes);
	// read number of divisions of for splitting each lung from command line parameters
        //sscanf(argv[4],"%d",&x);
	// read number of smoothing (closing/opening morph) passes for each iteration, from command line parameters
        //sscanf(argv[5],"%d",&smoothing);
 	
        // starts timer
   	time(&timer);  /* get current time; same as: timer = time(NULL)  */
  	seconds = difftime(timer,mktime(&y2k));

        // load the input image in mhd-raw format from the disk, segments it an return the segmented image in Image struct
        //acwex(x,&argv[1][0], &Image, iteracoes,smoothing);

        // binary segmentation
        acwebin(&argv[1][0], &Image);

  	time(&timer);  /* get current time; same as: timer = time(NULL)  */
        // calculate and print time passed during acwex execution
  	seconds2 = difftime(timer,mktime(&y2k)); 
    	printf ("%lf seconds\n", seconds2-seconds);

	/*similar=dsc(matriz,matrizjoint,205*420*281);
    	printf ("dsc=%lf\n", similar);*/

        // save the segmented image do the disk
	savemhdraw(argv[2],&Image);
        
        // free the memory used for storing the segmented image
	freeimage(&Image);
	

        // uses python script for converting from raw to npy
        sprintf(szCommand,"python3 raw2npy.py %s.raw %s.npy %d %d %d",argv[2],argv[2],Image.k0,Image.n0,Image.m0); 
        printf("%s\n",szCommand);
        system(szCommand);
        // uses python script based mayavi lib for showing the contoured segmented lungs 
        sprintf(szCommand,"python3 mostrarnpy.py %s.npy",argv[2]); 
        printf("%s\n",szCommand);
        system(szCommand);

	return 1;
	
}

