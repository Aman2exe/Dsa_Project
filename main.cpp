#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>
#include <vector>
#include <map>
#include <queue>
#include <limits>
#include <sstream>
#include <algorithm>
#include <set>
using namespace std;

struct Edge {
    string to;
    int val;
};

string toLowerManual(string s) {
    for (char &ch : s)
        if (ch >= 'A' && ch <= 'Z')
            ch = ch - 'A' + 'a';
    return s;
}

void showAllCities() {
    ifstream file("city.txt");
    if (!file.is_open()) {
        cout << "\033[1;31m[Error]\033[0m city.txt not found!\n";
        return;
    }
    cout << "\033[1;36mAvailable Cities:\033[0m\n";
    string name;
    bool empty = true;
    while (file >> name) {
        cout << "  -> " << name << "\n";
        empty = false;
    }
    if (empty)
        cout << "  (No cities added yet!)\n";
    cout << endl;
    file.close();
}

void loadGraph(map<string, vector<Edge>>& graph, string mode) {
    ifstream file("path.txt");
    if (!file.is_open()) {
        cout << "\033[1;31mError: path.txt not found!\033[0m\n";
        Sleep(1500);
        return;
    }
    string a, b;
    int t, c;
    while (file >> a >> b >> t >> c) {
        a = toLowerManual(a);
        b = toLowerManual(b);
        int value;
        if (mode == "cost") value = c;
        else if (mode == "time") value = t;
        else value = static_cast<int>(t * 0.6 + c * 0.4);

        graph[a].push_back({b, value});
        graph[b].push_back({a, value});
    }
    file.close();
}

struct FullPath {
    vector<string> route;
    int totalValue;
};


void dfsAllPaths(string current, string dest, map<string, vector<Edge>>& graph,
                 map<string, bool>& visited, vector<string>& path, int cost,
                 vector<FullPath>& allPaths) {
    if (current == dest) {
        allPaths.push_back({path, cost});
        return;
    }

    visited[current] = true;
    for (auto& e : graph[current]) {
        if (!visited[e.to]) {
            path.push_back(e.to);
            dfsAllPaths(e.to, dest, graph, visited, path, cost + e.val, allPaths);
            path.pop_back(); 
        }
    }
    visited[current] = false;
}

vector<FullPath> removeDuplicatePaths(const vector<FullPath>& allPaths) {
    vector<FullPath> uniquePaths;
    set<string> seen;

    for (const auto& p : allPaths) {
        string pathString;
        for (const auto& city : p.route)
            pathString += city + "->";
        pathString.pop_back(); 

        if (!seen.count(pathString)) {
            seen.insert(pathString);
            uniquePaths.push_back(p);
        }
    }

    return uniquePaths;
}

void findTopKPaths(map<string, vector<Edge>> graph, string start, string end, int K) {
    vector<FullPath> allPaths;
    map<string, bool> visited;
    vector<string> path = {start};

    dfsAllPaths(start, end, graph, visited, path, 0, allPaths);

    if (allPaths.empty()) {
        cout << "\033[1;31mNo route found between " << start << " and " << end << "!\033[0m\n";
        return;
    }

    sort(allPaths.begin(), allPaths.end(), [](const FullPath& a, const FullPath& b) {
        return a.totalValue < b.totalValue;
    });
    allPaths = removeDuplicatePaths(allPaths);
    cout << "\n\033[1;34mBest ways to travel from " << start << " to " << end << ":\033[0m\n";  
    int shown = min(K, (int)allPaths.size());
    for (int i = 0; i < shown; i++) {
        cout << "\033[1;36m" << i + 1 << ".\033[0m ";
        for (int j = 0; j < allPaths[i].route.size(); j++) {
            cout << allPaths[i].route[j];
            if (j < allPaths[i].route.size() - 1) cout << " -> ";
        }
        cout << "  (\033[1;32m" << allPaths[i].totalValue << "\033[0m)\n";
    }
}

void costpath() {
    system("cls");
    cout << "\033[1;34m=== BEST COST PATH FINDER ===\033[0m\n\n";
    showAllCities();

    string start, end;
    cout << "\033[1;33mEnter starting city:\033[0m ";
    cin >> start;
    cout << "\033[1;33mEnter destination city:\033[0m ";
    cin >> end;
    start = toLowerManual(start);
    end = toLowerManual(end);

    map<string, vector<Edge>> graph;
    loadGraph(graph, "cost");
    if (graph.empty()) {
        cout << "\033[1;31mNo paths found! Please add paths first.\033[0m\n";
        Sleep(1500);
        return;
    }

    cout << "\033[1;36mFinding best cost routes...\033[0m\n";
    Sleep(1000);
    findTopKPaths(graph, start, end, 5);
    system("pause");
}

void timepath() {
    system("cls");
    cout << "\033[1;34m=== BEST TIME PATH FINDER ===\033[0m\n\n";
    showAllCities();

    string start, end;
    cout << "\033[1;33mEnter starting city:\033[0m ";
    cin >> start;
    cout << "\033[1;33mEnter destination city:\033[0m ";
    cin >> end;
    start = toLowerManual(start);
    end = toLowerManual(end);

    map<string, vector<Edge>> graph;
    loadGraph(graph, "time");
    if (graph.empty()) {
        cout << "\033[1;31mNo paths found! Please add paths first.\033[0m\n";
        Sleep(1500);
        return;
    }

    cout << "\033[1;36mFinding fastest routes...\033[0m\n";
    Sleep(1000);
    findTopKPaths(graph, start, end, 5);
    system("pause");
}

void overallbest() {
    system("cls");
    cout << "\033[1;34m=== BEST OVERALL PATH FINDER ===\033[0m\n\n";
    showAllCities();

    string start, end;
    cout << "\033[1;33mEnter starting city:\033[0m ";
    cin >> start;
    cout << "\033[1;33mEnter destination city:\033[0m ";
    cin >> end;
    start = toLowerManual(start);
    end = toLowerManual(end);

    map<string, vector<Edge>> graph;
    loadGraph(graph, "overall");
    if (graph.empty()) {
        cout << "\033[1;31mNo paths found! Please add paths first.\033[0m\n";
        Sleep(1500);
        return;
    }

    cout << "\033[1;36mCalculating best overall routes...\033[0m\n";
    Sleep(1000);
    findTopKPaths(graph, start, end, 5);
    system("pause");
}

void Alltypeofpath() {
    int choice;
    while (true) {
        cout << "\033[1;35mSelect the type of route you want to find:\033[0m\n";
        cout << "1. Best cost path\n";
        cout << "2. Best timing path\n";
        cout << "3. Best overall path\n";
        cout << "4. Exit\n";
        cout << "Enter your choice: ";
        string input;
        cin >> input;
        choice = input[0] - '0';
        system("cls");
        switch (choice) {
            case 1: costpath(); break;
            case 2: timepath(); break;
            case 3: overallbest(); break;
            case 4: return;
            default: cout << "\033[1;31mInvalid choice!\033[0m\n"; Sleep(1000);
        }
        system("cls");
    }
}

bool cityExists(const string &city) {
    ifstream file("city.txt");
    if (!file.is_open()) return false;
    string name;
    while (file >> name) if (name == city) return true;
    return false;
}

void newcity() {
    cout << "\033[1;34mEnter the new city name:\033[0m ";
    string city;
    cin >> city;
    city = toLowerManual(city);

    ifstream infile("city.txt");
    string name;
    while (infile >> name)
        if (name == city) {
            cout << "\033[1;33mCity already exists!\033[0m\n";
            Sleep(1500);
            return;
        }
    infile.close();

    ofstream file("city.txt", ios::app);
    file << city << "\n";
    file.close();
    cout << "\033[1;32mCity added successfully!\033[0m\n";
    Sleep(1500);
}

void newpath() {
    showAllCities();
    string a, b;
    cout << "\033[1;33mEnter start city:\033[0m ";
    cin >> a;
    cout << "\033[1;33mEnter destination city:\033[0m ";
    cin >> b;
    a = toLowerManual(a);
    b = toLowerManual(b);

    if (!cityExists(a) || !cityExists(b)) {
        cout << "\033[1;31mOne or both cities not found!\033[0m\n";
        Sleep(1500);
        return;
    }

    int time, cost;
    cout << "Enter travel time: ";
    cin >> time;
    cout << "Enter travel cost: ";
    cin >> cost;

    ofstream file("path.txt", ios::app);
    file << a << " " << b << " " << time << " " << cost << "\n";
    file.close();
    cout << "\033[1;32mPath added successfully!\033[0m\n";
    Sleep(1500);
}

void deletepath() {
    showAllCities();
    string a, b;
    cout << "Enter start city: ";
    cin >> a;
    cout << "Enter destination city: ";
    cin >> b;
    a = toLowerManual(a);
    b = toLowerManual(b);

    ifstream infile("path.txt");
    vector<string> lines;
    string line;
    bool found = false;
    while (getline(infile, line)) {
        if (line.empty()) continue;
        string c1, c2; int t, cost;
        stringstream ss(line);
        ss >> c1 >> c2 >> t >> cost;
        if ((c1 == a && c2 == b) || (c1 == b && c2 == a))
            found = true;
        else
            lines.push_back(line);
    }
    infile.close();

    ofstream outfile("path.txt");
    for (auto &l : lines) outfile << l << "\n";
    outfile.close();

    if (found) cout << "\033[1;32mPath deleted successfully!\033[0m\n";
    else cout << "\033[1;31mPath not found!\033[0m\n";
    Sleep(1500);
}

void changeinthepath() {
    int choice;
    while (true) {
        cout << "\033[1;35m--- MODIFY PATH MENU ---\033[0m\n";
        cout << "1. Add new city\n";
        cout << "2. Add new path\n";
        cout << "3. Delete existing path\n";
        cout << "4. Exit\n";
        cout << "Enter your choice: ";
        string input;
        cin >> input;
        if(input.size()>=2 || input[0]<'0' || input[0]>'9'){
            cout<<"Invalid Input"<<endl ;
            Sleep(1000);
            continue ;
        }
        choice = input[0] - '0';
        system("cls");
        switch (choice) {
            case 1: newcity(); break;
            case 2: newpath(); break;
            case 3: deletepath(); break;
            case 4: return;
            default: cout << "\033[1;31mInvalid Input!\033[0m\n"; Sleep(1000);
        }
        system("cls");
    }
}

void printHeader() {
    cout << "\033[1;36m----------------------------------------------\033[0m\n";
    cout << "\033[1;36m-                                            -\033[0m\n";
    cout << "\033[1;32m-       SMART ROUTE FINDER SYSTEM            -\033[0m\n";
    cout << "\033[1;36m-                                            -\033[0m\n";
    cout << "\033[1;36m----------------------------------------------\033[0m\n";
}

int main() {
    printHeader();
    Sleep(2000);
    system("cls");
    while (true) {
        cout << "\033[1;35m--- MAIN MENU ---\033[0m\n";
        cout << "1. Find a path\n";
        cout << "2. Modify paths/cities\n";
        cout << "3. Exit\n";
        cout << "Enter your choice: ";
        string input;
        cin >> input;
        if(input.size()>=2 || (input[0]<'0' && input[0]>'9')){
            cout<<"Invalid Input"<<endl ;
            Sleep(1000);
            continue ;
        }
        int choice = input[0] - '0';
        system("cls");
        switch (choice) {
            case 1: Alltypeofpath(); break;
            case 2: changeinthepath(); break;
            case 3:
                cout << "\033[1;32mThanks for using Smart Route Finder!\033[0m\n";
                Sleep(1500);
                return 0;
            default:
                cout << "\033[1;31mInvalid choice!\033[0m\n";
                Sleep(1000);
        }
        system("cls");
    }
}
