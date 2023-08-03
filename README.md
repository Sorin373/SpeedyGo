<p align="center">
  <img src="LOGO.png" alt="Project Logo or Banner" width="200" height="200">
</p>

For my final project in computer science for grade 11, I was required to develop a console application that involved working with graphs. Therefore, I decided to create **SpeedyGo**, an app which has the main purpose to generate the most optimal path to supply different deposits in Romania.

# Table of Contents

- [Installation](#Installation)
- [Features](#features)
- [Contact](#contact)
- [MySql](#mysql)
- [Graph](#graph)
- [Distances](#distances)
- [Contact](#contact)

# Installation

In order to successfully set up and run the app, this section will walk you through all the necessary steps.

- First and foremost, we need to ensure that you have correctly installed the following dependencies: **MySql-Connector-CPP** for managing the database, **Nlohmann-json** for working with Json files, and **Curl** for transferring data using different network protocols.<br>Open your terminal and write the following cmds:<br><br>

```bash
$ sudo apt-get update
```

1. **MySql-Connector-CPP**

```bash
$ sudo apt-get install libmysqlcppconn-dev
```

2. **Nlohmann-json**

```bash
$ sudo apt-get install nlohmann-json3-dev
```

3. **Curl**

```bash
$ sudo apt-get install curl
```

- After acquiring the zip file containing the project files, the subsequent action involves extracting them from the compressed archive. To proceed, navigate through the terminal to the directory where you have stored the file. Then, execute the provided command to run the bash script found within the file named "run.sh":

```bash
$ ./run.sh
```

# Features
The application not only utilizes Dijkstra's algorithm to generate the most efficient routes between two fixed vertices, but also offers the most optimal supply path to reach all required deposits in a single route. In addition, the following features are available within the app:

- Database manager using pre-defined funtions
- The application includes an integrated SQL query tool that enables you to execute nearly any SQL script.
- You can visualize the deposits that require supply and the products that are in deficit.
- Last but not least, the graph is created using realtime data provided by the Google Maps API.

# MySql

In order to use the application you first need to create a local MySql database witch will house the necessary data (you can use either commands or the MySql workbench).

1. Database initialisation

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

- Now you are ready to create the Database (replace my_schema with the desire Database name):

```sql
mysql> CREATE DATABASE my_schema;
```

- If you want to create additional users and grant them access to the new database, follow these steps:

```sql
mysql> CREATE USER 'new_user'@'localhost' IDENTIFIED BY 'user_password';
mysql> GRANT ALL PRIVILEGES ON my_schema.* TO 'new_user'@'localhost';
mysql> FLUSH PRIVILEGES;
```

Login as the new user:

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

2. Create required tables

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

- Create tables:

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

- Show all tables:

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
- Replace '/path/to/your/data.csv' with the path to you CSV file and fill in the correct table names.

```sql
mysql> SET GLOBAL local_infile=1;
Query OK, 0 rows affected (0.00 sec)
mysql> quit;
Bye
```

```sql
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
# Graph
- The application lays over an __undirected graph__ which is a representation of the distance and the time it takes to get from one node to another.
- The graph is filled with data using the __Google Matrix API__. If there is no API key available the program uses the __Haversine Formula__ to determine the distances, which are not that precise.

## Google Matrix API
- Create the HTTP request which contains the origin and the destination (coordonates). The application uses the __Curl__ library to achive this.
1. Create a callback function to handle the response data received from the HTTP request. It first calculates the total size of the received data. Than it converts the received data from __void*__ to a __char*__ using a __static_cast__. At the end it appends the converted data to the string object pointed to by buffer. This effectively stores the received data in the response_body string. The function takes four parameters:

- __void *content:__ A pointer to the received data from the server.
- __size_t element_size:__ The size of each data element received 
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
- 

## The Haversine Formula

# Distances
Not only is the application able to generate the most efficient path between two points (eg. **Bucharest --> Cluj*) which is done using __Dijkstra's alghorithm__, but it can also help create the most effective route to supply all required deposites in one trip using __Backtracking__.

## Dijkstra's alghorithm

## Backtracking

# Contact
- Gmail: sorin.andrei.tudose@gmail.com
