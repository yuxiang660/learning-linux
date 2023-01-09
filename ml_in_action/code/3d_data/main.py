import pandas as pd
import logging
import plotly.express as px
import plotly.graph_objects as go
import numpy as np

logging.basicConfig(level=logging.INFO,
                    format='[%(levelname)7s] | %(message)s')

# Z = X + 2*Y
data = {
    "X": [1, 2, 3, 2.5, 4, 5],
    "Y": [1, 1, 2, 2, 3, 6],
    "Z": [3, 4, 7, 6.5, 10, 20],
}


def main():
    df = pd.DataFrame(data)
    fig1 = px.scatter_3d(df, x='X', y='Y', z='Z')

    x = np.arange(0, 10, 1)
    y = np.arange(0, 10, 1)
    X, Y = np.meshgrid(x, y)
    fig2 = go.Figure(data=[
        go.Surface(x=x, y=y, z=X+2*Y),
    ])

    fig3 = go.Figure(data=fig1.data + fig2.data)
    fig3.update_layout(title='Linear',
                       scene=dict(
                           xaxis=dict(range=[-5, 30],),
                           yaxis=dict(range=[-5, 30],),
                           zaxis=dict(range=[-5, 30],),),
                       margin=dict(l=50, r=50, b=100, t=50))
    fig3.write_html("./data.html")


if __name__ == "__main__":
    main()
