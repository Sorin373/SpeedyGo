<p align="center">
  <img src="Others/LOGO.png" alt="Project Logo or Banner" width="200" height="200">
</p>

For my final project in computer science for grade 11, I was required to develop a console application that involved working with graphs. Therefore, I decided to create **SpeedyGo**, an app whose main purpose is to generate the most optimal path to supply different deposits in Romania.

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

2. Run the installer by double-clicking the downloaded file (`speedygo-setup.exe`).

3. Follow the on-screen instructions to complete the installation.

4. Once installed, navigate to the installation folder (usually `C:\Program Files\SpeedyGo`) and find the `speedygo.exe` file.

5. Double-click `speedygo.exe` to launch the application.

## Unix (Debian)

1. Download the Debian package (`.deb` file) for SpeedyGo from the [GitHub Releases](https://github.com/sorin373/SpeedyGo/releases/tag/v1.0.0) page.
2. Install the package using the software installer
3. By default, SpeedyGo will be installed in the `/usr/bin` directory. To run the application, open your terminal here and execute:
```bash
$ ./SpeedyGo
```

## Unix (RPM-based)

1. Download the RPM package (`.rpm` file) for SpeedyGo from the [GitHub Releases](https://github.com/sorin373/SpeedyGo/releases/tag/v1.0.0) page.
2. Install the package using the rpm command:
```bash
$ sudo rpm -i SpeedyGo-1.0.0-Linux.rpm
```
3. By default, SpeedyGo will be installed in the `/usr/bin` directory. To run the application, open your terminal here and execute:
```bash
$ ./SpeedyGo
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
$ sudo apt-get update && sudo apt-get upgrade
```
1. **Install** [CMake](https://cmake.org/download/)
```bash
$ sudo snap install cmake --classic
```
2. **MySql-Connector-CPP**
```bash
$ sudo apt-get install libmysqlcppconn-dev
```
3. **Nlohmann-json**
```bash
$ sudo apt-get install nlohmann-json3-dev
```
4. **Curl**
```bash
$ sudo apt-get install curl
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
size_t _response_data_(void *content, size_t element_size, size_t elements, string *buffer)
{
    size_t total_size = element_size * elements;
    buffer->append(static_cast<char *>(content), total_size);
    return total_size;
}
```

2. The second step is to create the __HTTP GET__ request to the specified *URL*. It returns an HTTP_RESPONSE object containing the response body and response code.

```c++
HTTP_RESPONSE _http_request_(const string &url)
{
    CURL *curl = curl_easy_init();
    if (!curl)
    {
        cerr << setw(5) << " " << "Failed to initialize Curl!\n";
        return HTTP_RESPONSE{};
    }

    string response_body;
    long response_code = 0;

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, _response_data_);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_body);

    CURLcode res = curl_easy_perform(curl);

    if (res != CURLE_OK)
        cerr << curl_easy_strerror(res) << "\n";
    else
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);

    curl_easy_cleanup(curl);
    return HTTP_RESPONSE{response_body, response_code};
}
```
## The Haversine Formula

- The Haversine formula provides an approximation of the distance between two points on a sphere, such as the Earth.

1. This is the first part of the Haversine formula. It represents the square of half the chord length between the two points on the Earth's surface.
```txt
a = sin(dLat / 2) * sin(dLat / 2) + cos(toRadians(lat_1)) * cos(toRadians(lat_2)) * sin(dLon / 2) * sin(dLon / 2)
```
2. This is the second part of the Haversine formula. It calculates the angular distance between the two points in radians.
```txt
c = 2 * atan2(sqrt(a), sqrt(1 - a))
```

```c++
double calculare_distante(const double lat_1, const double long_1, const double lat_2, const double long_2)
{
    double dLat = toRadians(lat_2 - lat_1),
           dLon = toRadians(long_2 - long_1),
           a = sin(dLat / 2) * sin(dLat / 2) + cos(toRadians(lat_1)) * cos(toRadians(lat_2)) * sin(dLon / 2) * sin(dLon / 2),
           c = 2 * atan2(sqrt(a), sqrt(1 - a));
           
    return EARTH_RADIUS_KM * c;
}
```

# Distances
Not only is the application able to generate the most efficient path between two points (eg. **Bucharest -> Cluj*) which is done using __Dijkstra's alghorithm__, but it can also help create the most effective route to supply all required deposites in one trip using __Backtracking__.

## Dijkstra's alghorithm
1. This function takes a start node and two vectors (distanta and distanta_minima) as input. The distanta vector represents the distances from the start node to each node in the graph, while the distanta_minima vector stores the previous node on the shortest path to each node. The function implements Dijkstra's algorithm, which iteratively finds the shortest distance from the start node to all other nodes in the graph. It maintains a set of visited nodes and updates the distances and predecessors using a greedy approach. The function computes the shortest distances and stores them in the distanta vector and the shortest paths (previous nodes) in the distanta_minima vector.
```c++
void creare_solutie_distanta(int start, vector<double> &distanta, vector<int> &distanta_minima, bool afisare, bool creare_trasee)
{
    for (unsigned int i = 0; i < contor_noduri_graf; i++)
    {
        if (i != start)
        {
            if (afisare)
                cout << "Cea mai scurta distanta de la " << start << " la " << i << " este: " << distanta[i] << " : traseu: ";

            vector<int> traseu;

            int nod = i;

            while (nod != -1)
            {
                traseu.push_back(nod);
                nod = distanta_minima[nod];
            }

            reverse(traseu.begin(), traseu.end());

            if (creare_trasee)
                _traseu.inserareDateTraseu(start, i, distanta[i], traseu);

            if (afisare)
                for (unsigned int j = 0; j < traseu.size(); j++)
                    cout << traseu[j] << " ";

            if (afisare)
                cout << "\n";
        }
    }
}
```

2. This function takes the start node, the "distanta vector", and the "distanta_minima" vector as input. It also accepts two boolean flags, "afisare" and "creare_trasee". The function uses the calculated shortest distances and paths to display the results. For each node in the graph (excluding the start node), it prints the shortest distance from the start node to that node and the path taken to reach that node. The function retrieves the path by following the "distanta_minima" vector from the start node to the current node. It reverses the path to display it in the correct order. If "creare_trasee" is true, the function inserts the path into a data structure. The "afisare" flag controls whether the results are printed to the console.
```c++
void dijkstra(int start, vector<double> &distanta, vector<int> &distanta_minima)
{
    vector<bool> visited(contor_noduri_graf, false);
    distanta[start] = 0.0;

    for (unsigned int i = 0; i < contor_noduri_graf; i++)
    {
        int min_index = 0;
        double min_dist = numeric_limits<double>::infinity();

        for (unsigned int j = 0; j < contor_noduri_graf; j++)
            if (!visited[j] && distanta[j] < min_dist)
            {
                min_index = j;
                min_dist = distanta[j];
            }

        visited[min_index] = true;

        for (unsigned int j = 0; j < contor_noduri_graf; j++)
        {
            double distanta_noua = distanta[min_index] + matrice_drum[min_index][j].distanta;

            if (!visited[j] && matrice_drum[min_index][j].distanta > 0 && distanta_noua < distanta[j])
            {
                distanta[j] = distanta_noua;
                distanta_minima[j] = min_index;
            }
        }
    }
}
```

## Backtracking
When I analyzed how to build this part of the program, 2 posible different situations came to my mind. The first was the best case cenario where the graph is Hamiltonian, which means it consists of a Hamiltonian cycle representing the most efficient route and the other one was when the graph was acyclic (has no cycles). Therefore, I needed to build 2 Backtracking alghorithms for managing both Acyclic and Hamiltonian graphs. The functions were mostly similar to the point when I needed to create the solutions. In the first case the solution has to have the same amount of elements as the number of towns we need to supply. However if the graph is acyclic the elements in a solution will most definitely surpass that number, because in some situations we will need to visit a town twice in order to get to other towns.

# MySql Service

In order to use the application you first need to create a local MySql database witch will house the necessary data (you can use either commands or the [MySql Workbench](https://dev.mysql.com/downloads/workbench/)).

## Database initialisation

- If you have recently installed MySQL, you can execute other MySQL commands using the default user, "root." Access the MySQL command line by entering:

```sql
$ mysql -u root
```

- If you have assigned a password to the "root" user, use the following command, and it will prompt you to log in:

```sql
$ mysql -u root -p
```

- To enhance the security of the "root" user, you can set a password using the following command:

```sql
mysql> ALTER USER 'root'@'localhost' IDENTIFIED BY 'new_password';
```

- Now you are ready to create the Database (replace my_schema with the desired Database name):

```sql
mysql> CREATE DATABASE my_schema;
```

- If you want to create additional users and grant them access to the new database, follow these steps:

```sql
mysql> CREATE USER 'new_user'@'localhost' IDENTIFIED BY 'user_password';
mysql> GRANT ALL PRIVILEGES ON my_schema.* TO 'new_user'@'localhost';
mysql> FLUSH PRIVILEGES;
```

- Login as the new user:

```sql
$ mysql -u new_user -p
```

- You can also view all users by running the following command. However, it is recommended not to delete the system users as they serve specific purposes (the first 5 users):

```sql
mysql> SELECT user, host FROM mysql.user;
+------------------+-----------+
| user             | host      |
+------------------+-----------+
| debian-sys-maint | localhost |
| mysql.infoschema | localhost |
| mysql.session    | localhost |
| mysql.sys        | localhost |
| root             | localhost |
| new_user         | localhost |
+------------------+-----------+
6 rows in set (0,00 sec)
```

- Drop a user:

```sql
mysql> DROP USER 'my_user'@'localhost';
```

## Create the required tables

The tables have fixed structures and require specific predefined columns. However, you have the freedom to select the name of the table.

- Select the DB we created earlier:

```sql
mysql> SHOW DATABASES;
+--------------------+
| Database           |
+--------------------+
| my_schema          |
| information_schema |
| mysql              |
| performance_schema |
| sys                |
+--------------------+
5 rows in set (0,00 sec)
```

```sql
mysql> USE my_schema;
```

- Create the tables:

```sql
mysql> CREATE TABLE table_name_1 (
           ID_Oras INT PRIMARY KEY,
           Denumire_Oras VARCHAR(100) NOT NULL,
           latitudine DECIMAL(10, 7) NOT NULL,
           longitudine DECIMAL(10, 7) NOT NULL,
           Tip_Depozit VARCHAR(50)
       );
mysql> CREATE TABLE table_name_2 (
           ID_Produs INT,
           ID_Oras INT,
           Cantitate_Produs INT
       );
mysql> CREATE TABLE table_name_3 (
           ID_Produs INT PRIMARY KEY,
           Denumire_Produs VARCHAR(100),
           Categorie_Produs VARCHAR(50),
           Pret_Produs DECIMAL(10, 2)
       );
```

- Show all the tables:

```sql
mysql> SHOW TABLES;
+---------------------+
| Tables_in_my_schema |
+---------------------+
| table_name_1        |
| table_name_2        |
| table_name_3        |
+---------------------+
3 rows in set (0,00 sec)
```

- Add data (you need to execute this cmd for all the tables):
- Replace '/path/to/your/data.csv' with the path to your CSV file and fill in the correct table names.

```sql
mysql> SET GLOBAL local_infile=1;
Query OK, 0 rows affected (0.00 sec)
mysql> quit;
Bye
```

```bash
$ mysql --local-infile=1 -u root -p
```

```sql
mysql> USE my_schema;
mysql> LOAD DATA LOCAL INFILE '/path/to/your/table_name_1.csv'
       INTO TABLE table_name_1
       FIELDS TERMINATED BY ',' ENCLOSED BY '"'
       LINES TERMINATED BY '\n'
       IGNORE 1 LINES;
mysql> LOAD DATA LOCAL INFILE '/path/to/your/table_name_2.csv'
       INTO TABLE table_name_2
       FIELDS TERMINATED BY ',' ENCLOSED BY '"'
       LINES TERMINATED BY '\n'
       IGNORE 1 LINES;
mysql> LOAD DATA LOCAL INFILE '/path/to/your/table_name_3.csv'
       INTO TABLE table_name_3
       FIELDS TERMINATED BY ',' ENCLOSED BY '"'
       LINES TERMINATED BY '\n'
       IGNORE 1 LINES;
```

- Visualize the data (sample data utilized for testing - the CSV files are available in the repository if you wish to use them):

```sql
mysql> SELECT * FROM oras;
+---------+----------------+------------+-------------+-------------+
| ID_Oras | Denumire_Oras  | latitudine | longitudine | Tip_Depozit |
+---------+----------------+------------+-------------+-------------+
|       0 | Brasov         | 45.6579752 |  25.6011985 | local       |
|       1 | Bucuresti      | 44.4267674 |  26.1025384 | centralizat |
|       2 | Cluj           | 46.7712101 |  23.6236359 | local       |
|       3 | Constanta      | 44.1760175 |  28.6155619 | local       |
|       4 | Pitesti        | 44.8561453 |  24.8698545 | local       |
|       5 | Ramnicu Valcea | 45.0999934 |  24.3651926 | local       |
|       6 | Sibiu          | 45.7979713 |  24.1374603 | centralizat |
|       7 | Suceava        | 47.6443858 |  26.2575613 | local       |
|       8 | Targu Mures    | 46.5546502 |  24.5620975 | local       |
|       9 | Timisoara      | 45.7488716 |  21.2086793 | local       |
|      10 | Iasi           | 47.1598400 |  27.5877532 | local       |
+---------+----------------+------------+-------------+-------------+
11 rows in set (0,00 sec)
```

```sql
mysql> Select * from produs;
+-----------+----------------------+------------------+-------------+
| ID_Produs | Denumire_Produs      | Categorie_Produs | Pret_Produs |
+-----------+----------------------+------------------+-------------+
|         0 | Apa plata            | nealcoolice      |         2.5 |
|         1 | Suc de portocale     | nealcoolice      |           3 |
|         2 | Cola                 | nealcoolice      |           2 |
|         3 | Cafea                | nealcoolice      |         4.5 |
|         4 | Ceai verde           | nealcoolice      |         3.5 |
|         5 | Bere blonda          | alcoolice        |           5 |
|         6 | Vin rosu             | alcoolice        |          12 |
|         7 | Vodca                | alcoolice        |          20 |
|         8 | Whisky               | alcoolice        |          35 |
|         9 | Lichior de ciocolata | alcoolice        |          18 |
|        10 | Suc de mere          | nealcoolice      |         2.8 |
|        11 | Limonada             | nealcoolice      |         2.5 |
|        12 | Ceai de fructe       | nealcoolice      |         3.2 |
|        13 | Vin alb              | alcoolice        |        10.5 |
|        14 | Bere neagra          | alcoolice        |         6.5 |
|        15 | Gin                  | alcoolice        |          25 |
|        16 | Rom                  | alcoolice        |          22 |
|        17 | Vodca aromata        | alcoolice        |          21 |
|        18 | Apa minerala         | nealcoolice      |         2.3 |
|        19 | Suc de ananas        | nealcoolice      |         3.2 |
+-----------+----------------------+------------------+-------------+
20 rows in set (0,00 sec)
```
- If you want to run the MySQL server locally and only allow connections from the same machine (localhost), you can use localhost as the hostname when connecting to the MySQL server. By default, the MySQL server is configured to listen on 127.0.0.1, which is the loopback IP address for the local machine. As a result, the complete hostname will be "localhost:3306," with "3306" representing the port number.
- Check the port number:
```sql
mysql> SHOW VARIABLES LIKE 'port';
```
# Contact
I hope you enjoyed my application and found this project helpful.<br> If you want to get in touch with me you can do so through my personal Email: sorin.andrei.tudose@gmail.com.<br><br>Have a delightful day :D
