import pandas as pd
import googlemaps
import json

data = pd.read_excel(r'data.xlsx', 'Sheet1')

API_key = 'AIzaSyAw40qQY3qLHrcCo2y3ZhuGgKDx5Vy5Pvw'
gmaps = googlemaps.Client(key=API_key)

towns = list(set(data['Denumire_Oras'].tolist()))

distance_matrix = {}

for i, origin in enumerate(towns):
    for j, destination in enumerate(towns):
        if i == j:
            distance_matrix[f"{origin}_{destination}"] = 0
        elif f"{destination}_{origin}" not in distance_matrix:
            origin_coords = data.loc[data['Denumire_Oras'] == origin, 'coordinates'].iloc[0]
            destination_coords = data.loc[data['Denumire_Oras'] == destination, 'coordinates'].iloc[0]
            result = gmaps.distance_matrix(origin_coords, destination_coords, mode='driving')[
                "rows"][0]["elements"][0]["distance"]["value"]
            distance_matrix[f"{origin}_{destination}"] = result / 1000
            distance_matrix[f"{destination}_{origin}"] = result / 1000

with open('distance_matrix.json', 'w') as f:
    json.dump(distance_matrix, f)