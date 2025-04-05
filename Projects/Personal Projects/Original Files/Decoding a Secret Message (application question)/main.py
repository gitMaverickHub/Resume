# To run this program, pandas must be imported. To do this you'll need a virtual environment. Refer to https://www.youtube.com/watch?v=yG9kmBQAtW4&list=PLHQIBcQp_xR12cE1dpUUyTlpCx0qphvND&index=7 
# The terminal will output BOECWXH

import pandas as pd

def decoding_a_secret_message(url: str):
    df = pd.read_html(url, encoding='utf-8')
    df = df[0].drop(0)
    df[[0,2]] = df[[0,2]].apply(pd.to_numeric)
    df_sorted = df.sort_values([df.columns[2], df.columns[0]], ascending=[False,True])
    curr_row = 0
    final_row = len(df_sorted.values)
    while curr_row < final_row:
        new_line = False
        line = ""
        curr_space_x = 0
        while new_line == False:
            curr_row_x = df_sorted.values[curr_row][0]
            if curr_space_x == curr_row_x:
                curr_char = df_sorted.values[curr_row][1]
                line += curr_char
                prev_row = curr_row
                curr_row += 1
                prev_row_y = df_sorted.values[prev_row][2]
                curr_row_y = -1
                if curr_row < final_row:
                    curr_row_y = df_sorted.values[curr_row][2]
                if curr_row_y != prev_row_y:
                    new_line = True
            else:
                line += " "
            curr_space_x += 1
        print(line)
      
url = "https://docs.google.com/document/d/e/2PACX-1vQGUck9HIFCyezsrBSnmENk5ieJuYwpt7YHYEzeNJkIb9OSDdx-ov2nRNReKQyey-cwJOoEKUhLmN9z/pub"
decoding_a_secret_message(url)
