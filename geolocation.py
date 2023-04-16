import pandas as pd
import googlemaps

data = pd.read_excel(r'data.xlsx', 'Sheet1')

API_key = 'AIzaSyAw40qQY3qLHrcCo2y3ZhuGgKDx5Vy5Pvw'
gmaps = googlemaps.Client(key=API_key)

towns = list(set(data['Denumire_Oras'].tolist()))

distance_matrix = pd.DataFrame(columns=towns, index=towns)

for i, origin in enumerate(towns):
    for j, destination in enumerate(towns):
        if i == j:
            distance_matrix.iloc[i, j] = 0
        elif pd.isnull(distance_matrix.iloc[j, i]):
            origin_coords = data.loc[data['Denumire_Oras']
                                     == origin, 'coordinates'].iloc[0]
            destination_coords = data.loc[data['Denumire_Oras']
                                          == destination, 'coordinates'].iloc[0]
            result = gmaps.distance_matrix(origin_coords, destination_coords, mode='driving')[
                "rows"][0]["elements"][0]["distance"]["value"]
            distance_matrix.iloc[i, j] = result / 1000
            distance_matrix.iloc[j, i] = result / 1000

distance_matrix.to_excel('distance_matrix.xlsx')
