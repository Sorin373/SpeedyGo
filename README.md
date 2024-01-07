<p align="center">
  <img src="assets/LOGO.png" alt="Project Logo or Banner" width="200" height="200">
</p>

For my final project in computer science for grade 11, I was required to develop a console application that involved working with graphs. Therefore, I decided to create **SpeedyGo**, an app whose main purpose is to generate the most optimal path to supply different deposits in Romania.

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://github.com/sorin373/SpeedyGo/blob/main/LICENSE)

# Table of Contents

- [Installation](#installation)
    - [Windows](#windows)
    - [Unix (Debian)](#unix-debian)
    - [Unix (RPM-based)](#unix-rpm-based)
    - [Configuring Google API Services](#configuring-google-api-services)
- [Building the executable binaries](#building-the-executable-binaries)
    - [Windows](#windows-1)
    - [Debian](#debian)
- [Features](#features)
    - [Graph](#graph)
        - [Google Matrix API](#google-matrix-api)
        - [The Haversine Formula](#the-haversine-formula)
    - [Distances](#distances)
        - [Dijkstra's alghorithm](#dijkstras-alghorithm)
        - [Backtracking](#backtracking)
- [MySql](#mysql-service)
    - [Database initialisation](#database-initialisation)
    - [Create the required tables](#create-the-required-tables)
- [Contact](#contact)

# Installation

## Windows

1. Download the Windows installer for SpeedyGo from the [GitHub Releases](https://github.com/sorin373/SpeedyGo/releases/tag/v1.0.0) page.
2. Run the installer by double-clicking the downloaded file (`SpeedyGo-1.0.0-win64.exe`).
3. Follow the on-screen instructions to complete the installation.
4. Once installed, navigate to the installation folder (usually `C:\Program Files\SpeedyGo`) and find the `SpeedyGo.exe` file.
5. Double-click `SpeedyGo.exe` to launch the application.

## Unix (Debian)

1. Download the Debian package (`.deb` file) for SpeedyGo from the [GitHub Releases](https://github.com/sorin373/SpeedyGo/releases/tag/v1.0.0) page.
2. Install the package using the software installer
3. By default, SpeedyGo will be installed in the `/usr/bin` directory. To run the application, open your terminal here and execute:
```bash
./SpeedyGo
```

## Unix (RPM-based)

1. Download the RPM package (`.rpm` file) for SpeedyGo from the [GitHub Releases](https://github.com/sorin373/SpeedyGo/releases/tag/v1.0.0) page.
2. Install the package using the rpm command:
```bash
sudo rpm -i SpeedyGo-1.0.0-Linux.rpm
```
3. By default, SpeedyGo will be installed in the `/usr/bin` directory. To run the application, open your terminal here and execute:
```bash
./SpeedyGo
```

## Configuring Google API Services

To enable Google API services in SpeedyGo, follow these steps:

1. **Create a `config.json` file**: You can obtain the API key by visiting the [Google Developer Console](https://console.developers.google.com/).

```json
{
    "API_KEY": "your_api_key"
}
```

2. **Place `config.json` in the `utils` folder**:
    - **Windows**: If you are using Windows, locate the `utils` folder within the installation directory where you installed SpeedyGo. Place the `config.json` file in this folder.

    - **Unix (Debian/Ubuntu)**: On Debian/Ubuntu systems, the `utils` folder is typically located in `/usr/bin`. Place the `config.json` file in this directory.

    - **Unix (RPM-based)**: On RPM-based systems, like CentOS or Fedora, the `utils` folder is typically located in `/usr/local/bin`. Place the `config.json` file in this directory.

3. **Manual Configuration (Optional)**: If you prefer not to use a `config.json` file, you can manually configure the Google API credentials within the SpeedyGo application.

With these configurations in place, you're now ready to make use of SpeedyGo with Google API services! Enjoy the enhanced functionality.

# Building the executable binaries

In order to successfully set up and build the app, this section will walk you through all the necessary steps.

- First and foremost, we need to ensure that you have correctly installed the following dependencies: **MySql-Connector-CPP** for managing the database, **Nlohmann-json** for working with Json files, and **Curl** for transferring data using different network protocols.

## Windows

- On Windows, you'll need to set up and initialize [Vcpkg](https://vcpkg.io/en/getting-started) and [CMake](https://cmake.org/download/)

- Add the following lines of code in the file **settings.json** (replace **path_to** with the correct path):
```json
"cmake.configureArgs": [
        "-DCMAKE_TOOLCHAIN_FILE=path_to\\vcpkg\\scripts\\buildsystems\\vcpkg.cmake",
        "-DVCPKG_TARGET_TRIPLET=x64-windows"
    ]
```

- [MSVC](https://learn.microsoft.com/en-us/cpp/build/reference/compiler-options?view=msvc-170) compiler is needed to build this application. You can download the [workload](https://learn.microsoft.com/en-us/cpp/build/vscpp-step-0-installation?view=msvc-170) in the [Visual Studio Installer](https://visualstudio.microsoft.com/vs/features/cplusplus/).

## Debian

```bash
sudo apt-get update && sudo apt-get upgrade
```
1. **Install** [CMake](https://cmake.org/download/)
```bash
sudo snap install cmake --classic
```
2. **MySql-Connector-CPP**
```bash
sudo apt-get install libmysqlcppconn-dev
```
3. **Nlohmann-json**
```bash
sudo apt-get install nlohmann-json3-dev
```
4. **Curl**
```bash
sudo apt-get install curl
```

- After acquiring the zip file containing the project files, open the folder in Visual Studio Code and use the CMake commands to build the application (or install the [CMake](https://marketplace.visualstudio.com/items?itemName=twxs.cmake) and [CMake Tools](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cmake-tools) extensions).

# Features
Not only does the application use Dijkstra's algorithm to generate the most efficient routes between two fixed vertices, but also offers the most optimal supply path to reach all required deposits in a single route. In addition, the following features are available within the app:

- Database manager using pre-defined funtions
- The application includes an integrated SQL query tool that enables you to execute nearly any SQL script.
- You can visualize the deposits that require supply and the products that are in deficit.
- Last but not least, the graph is created using realtime data provided by the Google Maps API.

# Graph
- The application lays over an __undirected graph__ which is a representation of the distance and the time it takes to get from one node to another.
- The graph is filled with data using the __Google Matrix API__. If there is no API key available the program uses the __Haversine Formula__ to determine the distances, which are not that precise.

## Google Matrix API
- Create the HTTP request which contains the origin and the destination (coordonates). The application uses the __Curl__ library to achive this.
1. Create a callback function to handle the response data received from the HTTP request. It first calculates the total size of the received data. Than it converts the received data from __void*__ to a __char*__ using a __static_cast__. At the end it appends the converted data to the string object pointed to by buffer. This effectively stores the received data in the response_body string. The function takes four parameters:

- __void *content:__ A pointer to the received data from the server.
- __size_t element_size:__ The size of each received data element.
- __size_t elements:__ The number of data elements received.
- __string *buffer:__ A pointer to a string object where the received data should be stored.

```c++
size_t _response_data_(void *content, size_t element_size, size_t elements, std::string *buffer)
{
    size_t total_size = element_size * elements;
    buffer->append((char *)(content), total_size);
    return total_size;
}
```

2. The second step is to create the __HTTP GET__ request to the specified *URL*. It returns an HTTP_RESPONSE object containing the response body and response code.

```c++
HTTP_RESPONSE _http_request_(const std::string &url)
{
    CURL *curl = curl_easy_init();
    
    std::string response_body = "";
    long response_code = 0;

    if (!curl)
    {
        std::cerr << std::setw(5) << " "
                  << "Failed to initialize Curl!\n";
        return HTTP_RESPONSE{};
    }

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, _response_data_);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_body);

    CURLcode res = curl_easy_perform(curl);

    if (res != CURLE_OK)
        std::cerr << curl_easy_strerror(res) << "\n";
    else
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);

    curl_easy_cleanup(curl);
    return HTTP_RESPONSE{response_body, response_code};
}
```
## The Haversine Formula

- The Haversine formula provides an approximation of the distance between two points on a sphere, such as the Earth.

1. This is the first part of the Haversine formula. It represents the square of half the chord length between the two points on the Earth's surface.

2. This is the second part of the Haversine formula. It calculates the angular distance between the two points in radians.

$$
a = \sin^2\left(\frac{\Delta\text{Lat}}{2}\right) + \cos(\text{Lat}_1) \cdot \cos(\text{Lat}_2) \cdot \sin^2\left(\frac{\Delta\text{Lon}}{2}\right)
$$

$$
c = 2 \cdot \text{atan2}\left(\sqrt{a}, \sqrt{1 - a}\right)
$$

$$
d = R \cdot c
$$

```c++
double distanceCalculator(const double Latitude_City_1, const double Longitude_City_1, const double lat_2, const double long_2)
{
    double dLat = toRadians(lat_2 - Latitude_City_1),
           dLon = toRadians(long_2 - Longitude_City_1),
           a = sin(dLat / 2) * sin(dLat / 2) + cos(toRadians(Latitude_City_1)) * cos(toRadians(lat_2)) * sin(dLon / 2) * sin(dLon / 2),
           c = 2 * atan2(sqrt(a), sqrt(1 - a));

    return EARTH_RADIUS_KM * c;
}
```

# Distances
Not only is the application able to generate the most efficient path between two points (eg. **Bucharest -> Cluj**) which is done using __Dijkstra's alghorithm__, but it can also help create the most effective route to supply all required deposites in one trip using __Backtracking__.

## Dijkstra's alghorithm
1. This function takes a start node and two vectors (`distance` and `pathVector`) as input. The `distance` vector represents the distances from the start node to each node in the graph, while the `pathVector` vector stores the previous node on the shortest path to each node. The function implements Dijkstra's algorithm, which iteratively finds the shortest distance from the start node to all other nodes in the graph. It maintains a set of visited nodes and updates the distances and predecessors using a greedy approach. The function computes the shortest distances and stores them in the `distance` vector and the shortest paths (previous nodes) in the `pathVector` vector.
```c++
void Dijkstra::dijkstra(const int start, std::vector<double> &distance, std::vector<int> &pathVector)
{
    std::vector<bool> visited(VERTEX_COUNT, false);
    distance[start] = 0.0;

    for (unsigned int i = 0; i < VERTEX_COUNT; i++)
    {
        int min_index = 0;
        double min_dist = std::numeric_limits<double>::infinity();

        for (unsigned int j = 0; j < VERTEX_COUNT; j++)
            if (!visited[j] && distance[j] < min_dist)
            {
                min_index = j;
                min_dist = distance[j];
            }

        visited[min_index] = true;

        for (unsigned int j = 0; j < VERTEX_COUNT; j++)
        {
            double newDistance = distance[min_index] + adjacencyMatrix[min_index][j].distance;

            if (!visited[j] && adjacencyMatrix[min_index][j].distance > 0 && newDistance < distance[j])
            {
                distance[j] = newDistance;
                pathVector[j] = min_index;
            }
        }
    }
}
```

2. This function takes the start node, the `distance` vector, and the `pathVector` vector as input. It also accepts two boolean flags, `debug` and `createRoutes`. The function uses the calculated shortest distances and paths to display the results. For each node in the graph (excluding the start node), it prints the shortest distance from the start node to that node and the path taken to reach that node. The function retrieves the path by following the `pathVector` vector from the start node to the current node. It reverses the path to display it in the correct order. If `createRoutes` is true, the function inserts the path into a data structure. The `debug` flag controls whether the results are printed to the console.
```c++
void Dijkstra::generateDistanceSolution(const int start, std::vector<double> &distance, std::vector<int> &pathVector, bool debug, bool createRoutes)
{
    for (unsigned int i = 0; i < VERTEX_COUNT; i++)
    {
        if (i != start)
        {
            if (debug) // Debugging option
                std::cout << "The shortest distance from " << start << " to " << i << " is: " << distance[i] << " : optimalRoute: ";

            std::vector<int> optimalRoute;
            int node = i;

            while (node != -1)
            {
                optimalRoute.push_back(node);
                node = pathVector[node];
            }

            reverse(optimalRoute.begin(), optimalRoute.end());

            if (createRoutes)
                route.getData(start, i, distance[i], optimalRoute);

            if (debug) // Debugging option
                for (unsigned int j = 0; j < optimalRoute.size(); j++)
                    std::cout << optimalRoute[j] << " ";

            if (debug) // Debugging option
                std::cout << "\n";
        }
    }
}
```

## Backtracking
When I analyzed how to build this part of the program, two posible situations came to my mind. The first was the best case cenario where the graph is **Hamiltonian**, which means it consists of a **Hamiltonian cycle** representing the most efficient route and the other one was when the graph was **acyclic** (has no cycles). Therefore, I needed to build 2 Backtracking alghorithms for managing both **Acyclic** and **Hamiltonian** graphs. The functions were mostly similar to the point when I needed to create the solutions. In the first case the solution has to have the same amount of elements as the number of towns we need to supply. However if the graph is acyclic the elements in a solution will most definitely surpass that number, because in some situations we will need to visit a town twice in order to get to other towns.

# MySql Service

In order to use the application you first need to create a local MySql database witch will house the necessary data (you can use either commands or the [MySql Workbench](https://dev.mysql.com/downloads/workbench/)).

## Database initialisation

- If you have recently installed MySQL, you can execute other MySQL commands using the default user, `root`. Access the MySQL command line by entering:

```shell
mysql -u root
```

- If you have assigned a password to the `root` user, use the following command, and it will prompt you to log in:

```bash
mysql -u root -p
```

- To enhance the security of the `root` user, you can set a password using the following command:

```sql
ALTER USER 'root'@'localhost' IDENTIFIED BY 'new_password';
```

- Now you are ready to create the Database (replace my_schema with the desired Database name):

```sql
CREATE DATABASE my_schema;
```

- If you want to create additional users and grant them access to the new database, follow these steps:

```sql
CREATE USER 'new_user'@'localhost' IDENTIFIED BY 'user_password';
GRANT ALL PRIVILEGES ON my_schema.* TO 'new_user'@'localhost';
FLUSH PRIVILEGES;
```

- Login as the new user:

```bash
mysql -u new_user -p
```

- You can also view all users by running the following command. However, it is recommended not to delete the system users as they serve specific purposes (the first 5 users):

```sql
SELECT user, host FROM mysql.user;
```

- Drop a user:
```sql
DROP USER 'my_user'@'localhost';
```

## Create the required tables

The tables have fixed structures and require specific predefined columns. However, you have the freedom to select the name of the table.

- Select the database we created earlier:

```sql
SHOW DATABASES;
USE my_schema;
```

- Create the tables:

```sql
CREATE TABLE table_name_1 (
        ID_Oras INT PRIMARY KEY,
        Denumire_Oras VARCHAR(100) NOT NULL,
        latitudine DECIMAL(10, 7) NOT NULL,
        longitudine DECIMAL(10, 7) NOT NULL,
        Tip_Depozit VARCHAR(50)
    );

CREATE TABLE table_name_2 (
        ID_Produs INT,
        ID_Oras INT,
        Cantitate_Produs INT
    );
       
CREATE TABLE table_name_3 (
        ID_Produs INT PRIMARY KEY,
        Denumire_Produs VARCHAR(100),
        Categorie_Produs VARCHAR(50),
        Pret_Produs DECIMAL(10, 2)
    );
```

- Add data (Replace `/path/to/your/data.csv` with the path to your CSV file and fill in the correct table names)

```sql
SET GLOBAL local_infile=1;
quit;
```

- Go back to the MySQL command line

```bash
mysql --local-infile=1 -u root -p
```

```sql
USE my_schema;

LOAD DATA LOCAL INFILE '/path/to/your/table_name_1.csv'
INTO TABLE table_name_1
FIELDS TERMINATED BY ',' ENCLOSED BY '"'
LINES TERMINATED BY '\n'
IGNORE 1 LINES;

LOAD DATA LOCAL INFILE '/path/to/your/table_name_2.csv'
INTO TABLE table_name_2
FIELDS TERMINATED BY ',' ENCLOSED BY '"'
LINES TERMINATED BY '\n'
IGNORE 1 LINES;

LOAD DATA LOCAL INFILE '/path/to/your/table_name_3.csv'
INTO TABLE table_name_3
FIELDS TERMINATED BY ',' ENCLOSED BY '"'
LINES TERMINATED BY '\n'
IGNORE 1 LINES;
```

#

- If you want to run the MySQL server locally and only allow connections from the same machine (localhost), you can use localhost as the hostname when connecting to the MySQL server. By default, the MySQL server is configured to listen on `127.0.0.1`, which is the loopback IP address for the local machine. As a result, the complete hostname will be `localhost:3306`, with `3306` representing the port number.
- Check the port number:
```sql
SHOW VARIABLES LIKE 'port';
```
# Contact
I hope you enjoyed my application and found this project helpful.<br> If you want to get in touch with me you can do so through my personal email: <br>**sorin.andrei.tudose@gmail.com**.<br><br>Have a delightful day!
