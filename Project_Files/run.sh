#!/bin/bash

g++ main.cpp -o SpeedyGo_LINUX.exe -lmysqlcppconn -lcurl

if [ $? -eq 0 ]; then
    ./SpeedyGo_LINUX.exe
    echo "Exit Status: $?"
else
    echo "Compilation failed!"
fi
