#!/bin/bash

g++ main.cpp -o main -lmysqlcppconn -lcurl

if [ $? -eq 0 ]; then
    ./main
    echo "Exit Status: $?"
else
    echo "Compilation failed!"
fi
