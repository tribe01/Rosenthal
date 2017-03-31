import sys
import matplotlib
import numpy as np
import matplotlib.pyplot as plt

data = np.genfromtxt('output_2D.csv', delimiter=',')
x = data[:,0]
y = data[:,1]
z = data[:,2]
x=np.unique(x)
y=np.unique(y)
X,Y=np.meshgrid(x,y)
Z=z.reshape(len(y),len(x))
HM=plt.pcolormesh(X,Y,Z)
HM.set_clim(vmin=1000, vmax=2000)
plt.title('Temperature Distribution (K)')
plt.colorbar()
plt.savefig("output.png")
plt.show()
