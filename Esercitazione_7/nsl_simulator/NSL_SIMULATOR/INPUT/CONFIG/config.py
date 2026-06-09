import numpy as np

x, y, z = np.loadtxt("conf-1.xyz", skiprows=2, usecols=(1, 2, 3), unpack=True)

x=(x+0.5)/2
y=(y+ 0.5)/2 
z=(z+0.5)/2

with open("conf-low-entropy.dat", "w") as f:
    f.write(f"{len(x)}\n")
    f.write("# Modificato con Python\n")
    
    # Riscrivi le righe con "LJ" e i nuovi valori
    for i in range(len(x)):
        f.write(f"LJ  {x[i]:.6f}   {y[i]:.6f}   {z[i]:.6f}\n")