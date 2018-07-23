

import numpy as np
from scipy.misc import imread
from matplotlib import pyplot as ppl
import os
import sys
from scipy.misc import imsave
from mayavi import mlab
import matplotlib.pyplot as ppl
import cv2

#def showSlice()
#   cv2.imshow('image',img)

#y=np.int_(np.ones((971,512,512)))

x=np.fromfile("saida1.raw", dtype='int', sep="")
x = x.reshape([51, 281,210])
np.save("saida1.npy",x)

x=np.fromfile("saida2.raw", dtype='uint32', sep="")
x = x.reshape([51, 281,210])
np.save("saida2.npy",x)

x=np.fromfile("saida3.raw", dtype='uint32', sep="")
x = x.reshape([51, 281,210])
np.save("saida3.npy",x)

x=np.fromfile("saida0.raw", dtype='uint32', sep="")
x = x.reshape([51, 281,210])
np.save("saida0.npy",x)

x=np.fromfile("saida_.raw", dtype='uint32', sep="")
x = x.reshape([205, 281,420])
np.save("saida_.npy",x)

x=np.fromfile("original.raw", dtype='uint32', sep="")
x = x.reshape([205, 281,420])
np.save("original.npy",x)

#cv2.imshow('',x[870])
#img=x[870]
#cv2.imshow('image',img)
#cv2.createTrackbar('Index', 'wndName', 0, images.shape[0] - 1, showSlice)

#cv2.waitKey(0)
#cv2.destroyAllWindows()
#y=np.int_(x)
#y = y.reshape([971, 512,512])


np.save("saida.npy",x)

#A = np.fromfile(filename, dtype='int16', sep="")

