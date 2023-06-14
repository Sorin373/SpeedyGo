<p align="center">
  <img src="Documantation/SpeedyGo_LOGO.png" alt="Project Logo or Banner" width="200" height="200">
</p>

For my final project in computer science for grade 11, I was required to develop a console application that involved working with graphs. Therefore, I decided to create **SpeedyGo**, an app which has the main purpose to generate the most optimal path to supply different deposits in Romania.

# Table of Contents

- [Installation](#Installation)
- [Features](#features)
- [Contact](#contact)

# Installation

In order to successfully set up and run the app, this section will walk you through all the necessary steps. 

- First and foremost, we need to ensure that you have correctly installed the following dependencies: __MySql-Connector-CPP__ for managing the database, __Nlohmann-json__ for working with Json files, and __Curl__ for transferring data using different network protocols.<br>Open your terminal and write the following cmds:<br><br>
```
$ sudo apt-get update
```
1. __MySql-Connector-CPP__
```
$ sudo apt-get install libmysqlcppconn-dev
```
2. __Nlohmann-json__
```
$ sudo apt-get install nlohmann-json3-dev
```
3. __Curl__
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

# Contact
- Gmail: sorin.andrei.tudose@gmail.com