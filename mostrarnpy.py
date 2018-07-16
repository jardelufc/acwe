

import numpy as np
from scipy.misc import imread
from matplotlib import pyplot as ppl
import os
import sys
from scipy.misc import imsave
from mayavi import mlab
import matplotlib.pyplot as ppl

toshow = np.load("esquerdoinferior.npy")
	
fig = mlab.gcf()
mlab.clf()
src = mlab.pipeline.scalar_field(toshow)
mlab.pipeline.image_plane_widget(src, colormap='gray') #, plane_orientation='x_axes', colormap='gray'
cnt = mlab.contour3d(toshow, contours=[0.5])
mlab.show()  
