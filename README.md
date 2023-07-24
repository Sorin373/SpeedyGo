<p align="center">
  <img src="LOGO.png" alt="Project Logo or Banner" width="200" height="200">
</p>

For my final project in computer science for grade 11, I was required to develop a console application that involved working with graphs. Therefore, I decided to create **SpeedyGo**, an app which has the main purpose to generate the most optimal path to supply different deposits in Romania.

# Table of Contents

- [Installation](#Installation)
- [Features](#features)
- [Contact](#contact)
- [MySql](#mysql)

# Installation

In order to successfully set up and run the app, this section will walk you through all the necessary steps.

- First and foremost, we need to ensure that you have correctly installed the following dependencies: **MySql-Connector-CPP** for managing the database, **Nlohmann-json** for working with Json files, and **Curl** for transferring data using different network protocols.<br>Open your terminal and write the following cmds:<br><br>

```
$ sudo apt-get update
```

1. **MySql-Connector-CPP**

```
$ sudo apt-get install libmysqlcppconn-dev
```

2. **Nlohmann-json**

```
$ sudo apt-get install nlohmann-json3-dev
```

3. **Curl**

```
$ sudo apt-get install curl
```

- After acquiring the zip file containing the project files, the subsequent action involves extracting them from the compressed archive. To proceed, navigate through the terminal to the directory where you have stored the file. Then, execute the provided command to run the bash script found within the file named "run.sh":

```
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

1. Database initalisation

If you have recently installed MySQL, you can execute other MySQL commands using the default user, "root." Access the MySQL command line by entering:

```
$ mysql -u root
```

If you have assigned a password to the "root" user, use the following command, and it will prompt you to log in:

```
$ mysql -u root -p
```

To enhance the security of the "root" user, you can set a password using the following command:

```
mysql> ALTER USER 'root'@'localhost' IDENTIFIED BY 'new_password';
```

Now you are ready to create the Database (replace my_schema with the desire Database name):

```
mysql> CREATE DATABASE my_schema;
```

If you want to create additional users and grant them access to the new database, follow these steps:

```
mysql> CREATE USER 'new_user'@'localhost' IDENTIFIED BY 'user_password';
mysql> GRANT ALL PRIVILEGES ON my_schema.* TO 'new_user'@'localhost';
mysql> FLUSH PRIVILEGES;
```

Login as the new user:

```
$ mysql -u new_user -p
```

You can also view all users by running the following command. However, it is recommended not to delete the system users as they serve specific purposes (the first 5 users):

```
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

Drop a user:

```
mysql> DROP USER 'my_user'@'localhost';
```

2. Create required tables

The tables have fixed structures and require specific predefined columns. However, you have the freedom to select the name of the table.

- Select the DB we created earlier:

```
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

```
mysql> USE my_schema;
```

- Create tables:

```
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

```
mysql> SHOW TABLES;
+--------------------+
| Tables_in_my_schema |
+--------------------+
| table_name_1        |
| table_name_2        |
| table_name_3        |
+--------------------+
3 rows in set (0,00 sec)
```

- Add data (you need to execute this cmd for all the tables):
- Replace '/path/to/your/data.csv' with the path to you CSV file and fill in the correct table name.

```
mysql> SET GLOBAL local_infile=1;
Query OK, 0 rows affected (0.00 sec)
mysql> quit;
Bye
```

```
$ mysql --local-infile=1 -u root -p
```

```
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

```
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

```
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
<details>
<summary>Click to view the data (sample data used for testing)</summary>
```
mysql> select * from depozit;
+-----------+---------+------------------+
| ID_Produs | ID_Oras | Cantitate_Produs |
+-----------+---------+------------------+
|        16 |       7 |               28 |
|        18 |       1 |                2 |
|         9 |       2 |               15 |
|         9 |       6 |                0 |
|        10 |       5 |               30 |
|        17 |       5 |               27 |
|         6 |       0 |               34 |
|        16 |       9 |               16 |
|        11 |       1 |                8 |
|        16 |      10 |               19 |
|        17 |       0 |               26 |
|        14 |       3 |               11 |
|        14 |       5 |               39 |
|         1 |       7 |                6 |
|        19 |       9 |               23 |
|         4 |       5 |               25 |
|        17 |       9 |                3 |
|        11 |       8 |               29 |
|        10 |       2 |               19 |
|         0 |       5 |               20 |
|        13 |       7 |               21 |
|        19 |       8 |                7 |
|         6 |       3 |                7 |
|         4 |       1 |               19 |
|         2 |       4 |                4 |
|        12 |       4 |                0 |
|         7 |       0 |               39 |
|        16 |       6 |               17 |
|         8 |       9 |               32 |
|         7 |       2 |               18 |
|        16 |       0 |               10 |
|        14 |       6 |               14 |
|         8 |       7 |               22 |
|         0 |       2 |                9 |
|         7 |       3 |               44 |
|        19 |       5 |               45 |
|        13 |       9 |               31 |
|         4 |       9 |               49 |
|         1 |       9 |               41 |
|        17 |       6 |               17 |
|         0 |       6 |               11 |
|         0 |       4 |               45 |
|        12 |       2 |               40 |
|        14 |      10 |               32 |
|        18 |       5 |               34 |
|        17 |       1 |                8 |
|         6 |       8 |               13 |
|         5 |       7 |               45 |
|         9 |      10 |               13 |
|         1 |       8 |                4 |
|        11 |       2 |                0 |
|         8 |       1 |               36 |
|         8 |       5 |               27 |
|        15 |       7 |               46 |
|        11 |       0 |               29 |
|        18 |       0 |               12 |
|        11 |       7 |                9 |
|         2 |       9 |               14 |
|         3 |       1 |                9 |
|         9 |       3 |               10 |
|         6 |       5 |               40 |
|        19 |       7 |                7 |
|         1 |       4 |               32 |
|         8 |       4 |                3 |
|        10 |       4 |                6 |
|         9 |       9 |               35 |
|         4 |      10 |               18 |
|        13 |       3 |               16 |
|         4 |       7 |               33 |
|         5 |      10 |                1 |
|        16 |       5 |               21 |
|         5 |       2 |               37 |
|        18 |       3 |               13 |
|         3 |       3 |               15 |
|         7 |       9 |               40 |
|        16 |       4 |               24 |
|        18 |       9 |               43 |
|        19 |       2 |               26 |
|         2 |       0 |               46 |
|         7 |       7 |                6 |
|         9 |       7 |                0 |
|         7 |       1 |               21 |
|        16 |       2 |                0 |
|        14 |       8 |                0 |
|        14 |       7 |               14 |
|         7 |       4 |                4 |
|         3 |       8 |               14 |
|         5 |       8 |               28 |
|         5 |       4 |                9 |
|        17 |       7 |               46 |
|         0 |       8 |               47 |
|         9 |       8 |               37 |
|        11 |       3 |               20 |
|         6 |       1 |               41 |
|        16 |       1 |               43 |
|         3 |       5 |               49 |
|         7 |       6 |               32 |
|         1 |       2 |               36 |
|         1 |       5 |               27 |
|        18 |       7 |               11 |
|         8 |       2 |               14 |
|         2 |      10 |               25 |
|        19 |       6 |                8 |
|        17 |       8 |                3 |
|        18 |       6 |               38 |
|        19 |      10 |               33 |
|        10 |       6 |                7 |
|         0 |       0 |               28 |
|         3 |       0 |               32 |
|         4 |       2 |               46 |
|         1 |      10 |               40 |
|        12 |       0 |               46 |
|         6 |       7 |               18 |
|         7 |       5 |               46 |
|        18 |       2 |               42 |
|        12 |       6 |               10 |
|        17 |       4 |               20 |
|        10 |       8 |               20 |
|         5 |       5 |               49 |
|         1 |       3 |               30 |
|        14 |       2 |               21 |
|         2 |       1 |               21 |
|        14 |       0 |               11 |
|        19 |       0 |               17 |
|         3 |       4 |               13 |
|         5 |       6 |                6 |
|        19 |       4 |               25 |
|         3 |       7 |                4 |
|        15 |       1 |               13 |
|        11 |      10 |               48 |
|        12 |       3 |               30 |
|         7 |       8 |               11 |
|         0 |       1 |               29 |
|        11 |       6 |               30 |
|         8 |       8 |                1 |
|        15 |       9 |               46 |
|        15 |      10 |               34 |
|        13 |       4 |               47 |
|         1 |       6 |               36 |
|        15 |       4 |               37 |
|         0 |       7 |                4 |
|         9 |       0 |                2 |
|         2 |       2 |               18 |
|         8 |      10 |               20 |
|        16 |       3 |               43 |
|         9 |       5 |                3 |
|         6 |       2 |                3 |
|        15 |       0 |               33 |
|         6 |      10 |               41 |
|        11 |       5 |               20 |
|        15 |       6 |               38 |
|         2 |       8 |               34 |
|        19 |       1 |               34 |
|         2 |       7 |               30 |
|        11 |       4 |               36 |
|         8 |       6 |               50 |
|        13 |      10 |                5 |
|        19 |       3 |               12 |
|         9 |       1 |               17 |
|         4 |       8 |               19 |
|        17 |      10 |                3 |
|        10 |      10 |                1 |
|         0 |       9 |                4 |
|        10 |       1 |               16 |
|        12 |       1 |               44 |
|        13 |       2 |               21 |
|         2 |       3 |                7 |
|        13 |       5 |               11 |
|         5 |       0 |               13 |
|        11 |       9 |               48 |
|         3 |       9 |               10 |
|        17 |       3 |               48 |
|         4 |       3 |               39 |
|         1 |       1 |               11 |
|        18 |       4 |               44 |
|         8 |       0 |                8 |
|        18 |       8 |               50 |
|        16 |       8 |                3 |
|        15 |       3 |               38 |
|        10 |       0 |               28 |
|        10 |       9 |               38 |
|         5 |       1 |               14 |
|         6 |       4 |               19 |
|         4 |       0 |               28 |
|         6 |       9 |               35 |
|        13 |       6 |               12 |
|         4 |       4 |               46 |
|        15 |       8 |               30 |
|        13 |       0 |                5 |
|        18 |      10 |               43 |
|         5 |       9 |                1 |
|         0 |      10 |               45 |
|         2 |       6 |               17 |
|        13 |       8 |               44 |
|        12 |      10 |               48 |
|        12 |       8 |               18 |
|        14 |       1 |               29 |
|        15 |       2 |               16 |
|         1 |       0 |               17 |
|        10 |       7 |               16 |
|         0 |       3 |                5 |
|        12 |       9 |               14 |
|        15 |       5 |                2 |
|         3 |       6 |               39 |
|        10 |       3 |               42 |
|         3 |       2 |               38 |
|         2 |       5 |               25 |
|        14 |       9 |               17 |
|        17 |       2 |               33 |
|         6 |       6 |               36 |
|        13 |       1 |               45 |
|         7 |      10 |               26 |
|         8 |       3 |               42 |
|         5 |       3 |               41 |
|        14 |       4 |               36 |
|         9 |       4 |               19 |
|        12 |       5 |               26 |
|         3 |      10 |               20 |
|        12 |       7 |               43 |
|         4 |       6 |               34 |
+-----------+---------+------------------+
220 rows in set (0,00 sec)
```
</details>
# Contact

- Gmail: sorin.andrei.tudose@gmail.com
