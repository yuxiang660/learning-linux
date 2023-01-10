import pandas as pd
import logging
import plotly.express as px
import plotly.graph_objects as go
import numpy as np

logging.basicConfig(level=logging.INFO,
                    format='[%(levelname)7s] | %(message)s')

def main():
    x = np.arange(-5, 5, 0.25)
    y = np.arange(-5, 5, 0.25)
    X, Y = np.meshgrid(x, y)
    Z = X**2 + Y**2
    fig = go.Figure(data=[
        go.Surface(x=x, y=y, z=Z),
    ])

    fig.update_layout(title='z=x^2+y^2',
                       scene=dict(
                           xaxis=dict(range=[-6, 6],),
                           yaxis=dict(range=[-6, 6],),
                           zaxis=dict(range=[-6, 6],),),
                       margin=dict(l=50, r=50, b=100, t=50))
    fig.write_html("./data.html")

def main2():
    import matplotlib.pyplot as plt
    X = np.arange(-5, 5, 0.25)
    Y = np.arange(-5, 5, 0.25)
    X, Y = np.meshgrid(X, Y)
    Z = X**2 + Y**2
    fig, ax = plt.subplots(subplot_kw={"projection": "3d"})
    ax.plot_surface(X, Y, Z)
    plt.show()

if __name__ == "__main__":
    main()
