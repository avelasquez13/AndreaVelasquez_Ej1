import matplotlib.pyplot as plt
import matplotlib.image as mpimg
import numpy as np

f = open('potencial.dat', 'r')
datoslist = f.readlines()
f.close()

datosnp = np.array(datoslist)
datos = np.array_split(datosnp, 3)

potencial = np.array_split(datos[0], 250)
Ex = np.array_split(datos[1], 250)
Ey = np.array_split(datos[2], 250)

for i in range(len(potencial)):
    potencial[i] = np.asfarray(potencial[i])

for i in range(len(Ex)):
    Ex[i] = np.asfarray(Ex[i])

for i in range(len(Ey)):
    Ey[i] = np.asfarray(Ey[i])


plt.imshow(np.array(potencial).T, extent = [0, 5, 0, 5])

x = np.linspace(0, 5, 250)
y = np.linspace(5, 0, 250)
plt.streamplot(x, y, np.array(Ex).T, np.array(Ey).T, color = 'k', density = 1.3)

plt.ylim([0, 5])

plt.title('Potencial y campo de placas paralelas')
plt.savefig('placas.pdf')

