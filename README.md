SMART ROUTE FINDER SYSTEM
=========================

Description:
------------
This is a C++ console application that finds the best route between cities. 
It supports paths based on cost, time, or a combination of both (overall best path).

Files in the Project:
---------------------
1. main.cpp - Main C++ source code
2. city.txt - List of cities
3. path.txt - List of paths between cities with time and cost
4. main.exe - Compiled executable (optional)

Features:
---------
- Add new cities
- Add new paths between cities
- Delete existing paths
- Find best route based on cost, time, or overall value
- Display all available cities
- Dijkstra’s algorithm used for shortest path calculation

How to Run:
-----------
1. Compile main.cpp using g++ or any C++ compiler:
2. Run the program:
3. Follow the on-screen menu to add cities, paths, or find routes.

Notes:
------
- Make sure city.txt and path.txt are in the same folder as main.exe
- Paths must connect existing cities
- Input is case-insensitive (all city names are converted to lowercase)
