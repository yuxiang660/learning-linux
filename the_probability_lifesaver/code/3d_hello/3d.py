#%%

import numpy as np
import matplotlib.pyplot as plt

x1 = np.linspace(0, 2, 20)
x2 = np.linspace(1, 3, 20)
X1, X2 = np.meshgrid(x1, x2)

fig = plt.figure(figsize = (9, 6))

X3 = 6 - X1 - X2
ax = fig.add_subplot(111, projection = '3d')
ax.plot_surface(X1, X2, X3, cmap ='viridis')

X3 = 3 - 2*X1 + X2
ax.plot_surface(X1, X2, X3, cmap ='summer')

X3 = 3*X1 + 2*X2 -4
ax.plot_surface(X1, X2, X3, cmap ='spring')

#ax.scatter(1, 2, 3, s = 200, color = 'black')
plt.show()