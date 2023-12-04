#!/bin/bash
g++ -O3 -std=c++17 play.cpp -o play
g++ -O3 -std=c++17 -pthread tester.cpp -o test_sets_takes_1_hrs
g++ -O3 -std=c++17 -pthread generator.cpp -o pre_generator