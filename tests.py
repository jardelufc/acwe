
import morphsnakes

import numpy as np
from scipy.misc import imread
from matplotlib import pyplot as ppl
import os
import sys
from scipy.misc import imsave

def rgb2gray(img):
    """Convert a RGB image to gray scale."""
    return 0.2989*img[:,:,0] + 0.587*img[:,:,1] + 0.114*img[:,:,2]

def circle_levelset(shape, center, centerr,sqradius, scalerow=1.0):
    """Build a binary function with a circle as the 0.5-levelset."""
    grid = np.mgrid[list(map(slice, shape))].T - center
    phi = sqradius - np.sqrt(np.sum((grid.T)**2, 0))
    ul = np.float_(phi > 0)

    gridr = np.mgrid[list(map(slice, shape))].T - centerr
    phir = sqradius - np.sqrt(np.sum((gridr.T)**2, 0))
    ur = np.float_(phir > 0)
	
    #center1=(200, 77)
    #grid1 = np.mgrid[list(map(slice, shape))].T - center1
    #phi1 = sqradius - np.sqrt(np.sum((grid1.T)**2, 0))
    #u1 = np.float_(phi1 > 0)
	
    #center2=(350, 77)
    #grid2 = np.mgrid[list(map(slice, shape))].T - center2
    #phi2 = sqradius - np.sqrt(np.sum((grid2.T)**2, 0))
    #u2 = np.float_(phi2 > 0)

    #center3=(200, 445)
    #grid3 = np.mgrid[list(map(slice, shape))].T - center3
    #phi3 = sqradius - np.sqrt(np.sum((grid3.T)**2, 0))
    #u3 = np.float_(phi3 > 0)

    #center4=(350, 445)
    #grid4 = np.mgrid[list(map(slice, shape))].T - center4
    #phi4 = sqradius - np.sqrt(np.sum((grid4.T)**2, 0))
    #u4 = np.float_(phi4 > 0)
	
    #center5=(200, 256)
    #grid5 = np.mgrid[list(map(slice, shape))].T - center5
    #phi5 = sqradius - np.sqrt(np.sum((grid5.T)**2, 0))
    #u5 = np.float_(phi5 > 0)

    #center6=(350, 256)
    #grid6 = np.mgrid[list(map(slice, shape))].T - center6
    #phi6 = sqradius - np.sqrt(np.sum((grid6.T)**2, 0))
    #u6 = np.float_(phi6 > 0)

	
    u=ul+ur
    return u

def test_nodule():
    # Load the image.
    img = imread("testimages/mama07ORI.bmp")[...,0]/255.0
    
    # g(I)
    gI = morphsnakes.gborders(img, alpha=1000, sigma=5.48)
    
    # Morphological GAC. Initialization of the level-set.
    mgac = morphsnakes.MorphGAC(gI, smoothing=1, threshold=0.31, balloon=1)
    mgac.levelset = circle_levelset(img.shape, (100, 126), 20)
    
    # Visual evolution.
    ppl.figure()
    morphsnakes.evolve_visual(mgac, num_iters=45, background=img)

def test_starfish():
    # Load the image.
    imgcolor = imread("testimages/seastar2.png")/255.0
    img = rgb2gray(imgcolor)
    
    # g(I)
    gI = morphsnakes.gborders(img, alpha=1000, sigma=2)
    
    # Morphological GAC. Initialization of the level-set.
    mgac = morphsnakes.MorphGAC(gI, smoothing=2, threshold=0.3, balloon=-1)
    mgac.levelset = circle_levelset(img.shape, (163, 137), 135, scalerow=0.75)
    
    # Visual evolution.
    ppl.figure()
    morphsnakes.evolve_visual(mgac, num_iters=110, background=imgcolor)

def test_lakes():
    # Load the image.
    img = imread("lungs/aori_100.bmp")[...,0]
    #imgls = imread("lungs/aori_53ls.bmp")[...,0]

    #img = rgb2gray(imgcolor)
    
    # MorphACWE does not need g(I)
    
    # Morphological ACWE. Initialization of the level-set.
    macwe = morphsnakes.MorphACWE(img, smoothing=0, lambda1=1, lambda2=1)
    macwe.levelset = circle_levelset(img.shape, (256, 350), 77)
    
    # Visual evolution.
    ppl.figure()
    morphsnakes.evolve_visual(macwe, num_iters=200,background=img )

def test_confocal3d():
    
    # Load the image.
    img = np.load("testimages/confocal.npy")
    
    #img=np.int64(img)
    # Morphological ACWE. Initialization of the level-set.
    macwe = morphsnakes.MorphACWE(img, smoothing=0, lambda1=1, lambda2=2)
    macwe.levelset = circle_levelset(img.shape, (30, 50, 80), 25)
    
    # Visual evolution.
    morphsnakes.evolve_visual3d(macwe, num_iters=200)

def bmp2raw():



    path = 'lungs'

    #if levelset is not None:
    #    msnake.levelset = levelset
    #imgori = np.load("testimages/confocal.npy")
    #img=np.copy(imgori[109:390,40:250])

    #levelset = circle_levelset(img.shape, (30, 50, 80), 25)

    x=np.uint32(np.ones((205,512,512)))
    #np.int_(u)
    #np.ndarray.tofile(x,"todo.raw")

    n=0
    for file in sorted(os.listdir(path)):
       current = os.path.join(path, file)
       if os.path.isfile(current):
           #data = open(current, "rb")
           #img = imread(current)[...,0]/255.0
		   
		   
 		   # Load the image.
           temp=imread(current)[...,0]
           x[n] = temp
           n=n+1
           print (current)

    print(n)
    np.ndarray.tofile(x,"full.raw")
    #x = x.reshape([205, 281,420])


def doall():


   path = 'lungs'
   #pathout = 'lungs'


   for file in os.listdir(path):
       current = os.path.join(path, file)
       if os.path.isfile(current):
           #data = open(current, "rb")
           #img = imread(current)[...,0]/255.0
		   
		   
		   # Load the image.
           img = imread(current)[...,0]
           imgorifull=np.copy(img)
		   
           imgori=np.copy(img[109:390,40:460])
           img[img < 100] = 0
           img[img >= 100] = 255
           c=0
           i=0
           j=0
           #img[255] = 255

           for j in range(0,512):
              c=0
              i=0
              aux=0
              while (i<512): #for i in range (0,511):
                 if (img[i][j]==255):
                    
                    #aux=aux+1
                  
                    if(aux>20):
                       i=512
                    aux=aux+1
                 else:
                    aux=0
				 
                 i=i+1
                 c=c+1
                
		   	  
              #if(c>0):
              for z in range(0,c):
                 img[z][j] = 255
              c=511
              i=511
              aux=0
              while (i>=0): #for i in range (0,511):
                 if (img[i][j]==255):
                    
                    #aux=aux+1
                  
                    if(aux>20):
                       i=0
                    aux=aux+1
                 else:
                    aux=0
				 
                 i=i-1
                 c=c-1
                
		   	  
              #if(c>=0):
              for z in range(c,512):
                 img[z][j] = 255
              c=0

                
           img=img[109:390,40:460]
           #img=morphsnakes.SI(morphsnakes.IS((img)))
           #img=morphsnakes.IS(morphsnakes.SI((img)))
           #img=morphsnakes.SI(morphsnakes.IS((img)))
           imgl=img[0:282,0:210]
           xc=0
           yc=0
           n=0
           for i in range(0,280):
              for j in range(0,210):
                 if(imgl[i][j]==0):
                    xc=xc+i
                    yc=yc+j
                    n=n+1
           xc=xc/n
           yc=yc/n
           #print(xc)
           #print(yc)
           #print(n)
		   
           imgr=img[0:282,211:420]
           xcr=0
           ycr=0
           n=0
           for i in range(0,280):
              for j in range(0,209):
                 if(imgr[i][j]==0):
                    xcr=xcr+i
                    ycr=ycr+j
                    n=n+1
           xcr=xcr/n
           ycr=ycr/n
           #print(xcr)
           #print(ycr)
           #print(n)
           ycr=ycr+210
           
           #myls = circle_levelset(imgr.shape, (xcr, ycr), 10)
           #for i in range(0,280):
           #   for j in range(0,209):
           #      if(myls[i][j]>0):
           #         imgori[i][j+210]=255
           #myls=~myls
           #imgl=imgl+myls
		   #imgr=img[109:390,40:460]
           



           #imsave('p'+current,imgori)

           #imgls = imread("lungs/background.bmp")[...,0]

           macwe = morphsnakes.MorphACWE(imgori, smoothing=3, lambda1=1, lambda2=1)
           macwe.levelset = circle_levelset(imgori.shape, (xc, yc),(xcr, ycr), 25)
    
           # Visual evolution.
           #ppl.figure()
           #morphsnakes.evolve_visual(macwe, num_iters=400,background=img )
           for i in range(200):
              macwe.step()
           
           dres = np.array(np.gradient(macwe._u))
           abs_dres = np.abs(dres).sum(0)
           x=0
           myimgorifull=imgorifull[109:390,40:460]
		   
           for i in range(0,280):
              for j in range(0,419):
                 if(abs_dres[i][j]>0):
                    myimgorifull[i][j]=0
		   
		   
           
           imsave('p'+current,imgorifull)
           print (current)

def visual3d():
    """
    Visual evolution of a three-dimensional morphological snake.
    
    Parameters
    ----------
    msnake : MorphGAC or MorphACWE instance
        The morphological snake solver.
    levelset : array-like, optional
        If given, the levelset of the solver is initialized to this. If not
        given, the evolution will use the levelset already set in msnake.
    num_iters : int, optional
        The number of iterations.
    """
    from mayavi import mlab
    import matplotlib.pyplot as ppl

    path = 'lungs'

    #if levelset is not None:
    #    msnake.levelset = levelset
    #imgori = np.load("testimages/confocal.npy")
    #img=np.copy(imgori[109:390,40:250])

    #levelset = circle_levelset(img.shape, (30, 50, 80), 25)

    #x=np.int_(np.ones((205,281,420)))
    #np.int_(u)
    #np.ndarray.tofile(x,"todo.raw")

    n=0
    #for file in os.listdir(path):
    #   current = os.path.join(path, file)
    #   if os.path.isfile(current):
           #data = open(current, "rb")
           #img = imread(current)[...,0]/255.0
		   
		   
 		   # Load the image.
     #      temp=imread(current)[...,0]
     #      x[n] = temp[109:390,40:460]
     #      n=n+1


    x=np.fromfile("todo.raw", dtype='uint32', sep="")
    x = x.reshape([205, 281,420])
    #np.save("original.npy",x)

    #imga = imread("lungs/aori_100.bmp")[...,0]

    #x[0]=imga
    #fig = mlab.gcf()
    #mlab.clf()
    #src = mlab.pipeline.scalar_field(x)
    #mlab.pipeline.image_plane_widget(src, colormap='gray') #, plane_orientation='x_axes', colormap='gray'
    #cnt = mlab.contour3d(x, contours=[0.5])
    
    #@mlab.animate(ui=True)
    #def anim():
    #    for i in range(num_iters):
    #        msnake.step()
    #        cnt.mlab_source.scalars = msnake.levelset
    #        print("Iteration %s/%s..." % (i + 1, num_iters))
    #        yield
    
    #anim()
    #mlab.show()
    #img=np.int64(img)
    # Morphological ACWE. Initialization of the level-set.
    macwe = morphsnakes.MorphACWE(x, smoothing=1, lambda1=1, lambda2=1)
    macwe.levelset = circle_levelset(x.shape, (100, 100, 130),(100, 100, 290), 20)
    #morphsnakes.evolve_visual3d(macwe, num_iters=300)
    for i in range(100):
       print("Iteration %s/%s..." % (i + 1, 100))
       macwe.step()
    np.save("original1.npy",macwe._u)
    for i in range(100):
       print("Iteration %s/%s..." % (i + 1, 100))
       macwe.step()
    np.save("original2.npy",macwe._u)
    for i in range(100):
       print("Iteration %s/%s..." % (i + 1, 100))
       macwe.step()
    np.save("original3.npy",macwe._u)
    for i in range(100):
       print("Iteration %s/%s..." % (i + 1, 100))
       macwe.step()
    np.save("original4.npy",macwe._u)
    for i in range(100):
       print("Iteration %s/%s..." % (i + 1, 100))
       macwe.step()
    np.save("original5.npy",macwe._u)
    
    
    #np.ndarray.tofile(macwe._u,"levelset.raw")
    #x[0][0][0]=55
    #print (x[0][0][0])
    #np.ndarray.tofile(x,"todo.raw")

    #toshow = np.load("todo.npy")
	
    #fig = mlab.gcf()
    #mlab.clf()
    #src = mlab.pipeline.scalar_field(toshow)
    #mlab.pipeline.image_plane_widget(src, colormap='gray') #, plane_orientation='x_axes', colormap='gray'
    #cnt = mlab.contour3d(toshow, contours=[0.5])
    #mlab.show()
    # Return the last levelset.

if __name__ == '__main__':
    print("""""")
    #test_nodule()
    #test_starfish()
    #test_lakes()
    #doall()
    bmp2raw()
    #test_confocal3d()
    #visual3d()
    #ppl.show()
