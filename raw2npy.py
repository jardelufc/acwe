

import numpy as np
from scipy.misc import imread
from matplotlib import pyplot as ppl
import os
import sys
from scipy.misc import imsave
from mayavi import mlab
import matplotlib.pyplot as ppl

x=np.fromfile("saida.raw", dtype='int', sep="")
x = x.reshape([102, 281,210])

np.save("saida.npy",x)

#A = np.fromfile(filename, dtype='int16', sep="")

