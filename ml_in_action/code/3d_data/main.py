import pandas as pd
import logging
import plotly.express as px

logging.basicConfig(level=logging.INFO,
                    format='[%(levelname)7s] | %(message)s')


data = {
    "X1": [1, 2, 3, 2.5, 4, 5],
    "X2": [1, 1, 2, 2.1, 3, 6],
    "Y": [3.1, 3.9, 8.2, 6.3, 10.2, 16.8],
}

def main():
    df = pd.DataFrame(data)
    logging.info(f'Data:\n{df}')
    fig = px.scatter_3d(df, x='X1', y='X2', z='Y')
    fig.show()


if __name__ == "__main__":
    main()
