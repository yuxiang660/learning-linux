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
    "Z": [3, 4, 7, 6.5, 10, 17],
}

def main():
    df = pd.DataFrame(data)
    x = np.array(df["X"])
    y = np.array(df["Y"])
    z = np.array(df["Z"])
    X,Y = np.meshgrid(x, y)
    fig1 = go.Figure(data=[
        go.Surface(x=x, y=y, z=X+2*Y),
    ])
    fig2 = px.scatter_3d(df, x='X', y='Y', z='Z')
    fig3 = go.Figure(data=fig1.data + fig2.data)
    fig3.write_html("./data.html")


if __name__ == "__main__":
    main()
