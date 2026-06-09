import numpy as np

dai = 2*np.pi

posizioni1 = np.random.uniform(-1, 1, (34,2))

posizioni2 = np.zeros((34, 2))
for i in range (34):
    rand= np.random.uniform(0, 2*np.pi)
    posizioni2[i, 0]=np.cos(rand)
    posizioni2[i, 1]= np.sin(rand)
     
np.savetxt('circle.dat', posizioni2, fmt='%f', delimiter=' ')
np.savetxt('square.dat', posizioni1, fmt='%f', delimiter=' ')