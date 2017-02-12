import numpy as np
import matplotlib.pyplot as plt

datosnp = np.loadtxt('potencial.dat')
datos = np.array_split(datosnp, 3) 

potencial = datos[0]
Ex = datos[1]
Ey = datos[2]


plt.imshow(potencial, extent = [0, 5, 0, 5])

x = np.linspace(0, 5, 250)
y = np.linspace(5, 0, 250)
plt.streamplot(x, y, Ex, Ey, color = 'k', density = 1.3)

plt.ylim([0, 5])

plt.title('Potencial y campo de placas paralelas')
plt.savefig('placas.pdf')
