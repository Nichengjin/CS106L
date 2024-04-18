/*
 * TODO: complete this file comment.
 */
#include <iostream>
#include "SimpleGraph.h"

#include <string>
#include <fstream>
#include <sstream>
#include <cmath>
#include <chrono>
#include <thread>

using namespace std;
//using chrono needs to compile with c++20 std.
//both nodes and edges are stored as vector in graph.

using namespace std;
/*Constant Values*/
const double kPi = 3.14159265358979323;
double kattract = 0.01;
double krepel = 0.01;

/*Global variables*/

ifstream file;
int PointNum;
SimpleGraph graph;
/*
double dx1;
double dx2;

double dy1;
double dy2;
*/

/*Func declaration*/
void Welcome();
void ReadFile(ifstream &file);
void AddPointAndEdge(ifstream &file, SimpleGraph &graph);

int GetRunTime();
void AttractForce(Edge &edge);
void RepulsiveForce(Node &p1, Node &p2);

int flag = 1;

// Main method

//present bug: the commented part seems not to work properly. I'll try to fix that.
int main() {
    Welcome();
    while (flag){
    InitGraphVisualizer(graph);
    cout << "The graph has been initialized." << endl;
    ReadFile(file);
    AddPointAndEdge(file,graph);

    std::chrono::microseconds loop_condition(GetRunTime());
    auto start = std::chrono::high_resolution_clock::now();

    cout << "Start time gotten. Start to draw." << endl;

// it is weird that the attractforce seems nowt to have any effect on the graph.
    /*
    while (std::chrono::duration_cast<chrono::microseconds>(std::chrono::high_resolution_clock::now()- start) < loop_condition){
        //do the loop.
        for (auto iter = graph.edges.begin(); iter != graph.edges.end(); ++iter) {
            AttractForce(*iter);
        }
        for (const auto &e : graph.edges) {
            AttractForce(e);
        }

        for (auto iter1 = graph.nodes.begin(); iter1 != graph.nodes.end(); ++iter1) {
            for (auto iter2 = iter1; iter2 != graph.nodes.end(); ++iter2){
                RepulsiveForce(*iter1, *iter2);
            }
        }
    }
    */
    for(int i = 0; i < 1000; i++) {
        //kattract *= 0.99;
        //krepel *= 0.99;
        for (auto iter = graph.edges.begin(); iter != graph.edges.end(); ++iter) {
            AttractForce(*iter);
            DrawGraph(graph);

        }
        cout << i << " ";
        std::this_thread::sleep_for(std::chrono::milliseconds(10));

        for (auto iter1 = graph.nodes.begin(); iter1 != graph.nodes.end(); ++iter1) {
            auto iter2 = iter1;
            iter2++;
            for (; iter2 != graph.nodes.end(); ++iter2){

                RepulsiveForce(*iter1, *iter2);
                DrawGraph(graph);
                //std::this_thread::sleep_for(std::chrono::milliseconds(5));
            }
        }
        cout << i << " ";
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }


    //graph.nodes[0].x++;

    DrawGraph(graph);
    cout << "Do you want to read from another file? Y/n" << endl;
    char decision;
    cin >> decision;
    getchar();
    if (toupper(decision) == 'N'){
        flag = 0;
    }

    }

    cout << "Thanks for using." << endl;

    return 0;
}

/* Prints a message to the console welcoming the user and
 * describing the program. */
void Welcome() {
    cout << "Welcome to CS106L GraphViz!" << endl;
    cout << "This program uses a force-directed graph layout algorithm" << endl;
    cout << "to render sleek, snazzy pictures of various graphs." << endl;

    cout << "Hello! I'm viraag N and i am learning cs106l!" << endl;
    cout << endl;
}

void ReadFile(ifstream &file){
    while (1){
        cout << "Please enter the file name you want to read and create a graph for:" << endl;
        string filename;
        getline(cin,filename); //get the filename from cin.
        file.open(filename.c_str());

        if (file.is_open()){
            cout << "file is successfully opened." << endl;
            return;
        } else{
            //throw runtime_error("Cannot hammer the fucking file.");
            cout << "error. the file may not exist. re-input the name." << endl;
            file.clear();
        }
    }
}

int GetRunTime(){
    while (true){
        cout << "Please input a integer representing the total runtime of this program in microseconds:" << endl;
        string input;
        getline(cin, input);
        stringstream in(input);

        int result;
        if (in >> result){
            char remaining;
            if (in >> remaining){
                cout << "Unexpected input:" << remaining << endl;
            } else{
                cout << "Successfully read runtime:" << result << endl;
                return result;
            }
        }
        cout << "Please input a correct integer." << endl;
    }
}

void AddPointAndEdge(ifstream &file, SimpleGraph &graph){
    string line;
    getline(file,line); //read the point num
    stringstream ss(line);

    if (ss >> PointNum){
        cout << "Point Number readed successfully from file. Point num:" << PointNum << endl;
    }

    //now create the points with position

    for (int i = 0; i < PointNum; i++){
        Node Point(cos(2*kPi*i/PointNum), sin(2*kPi*i/PointNum));
        graph.nodes.push_back(Point);
    }

    //now add edges for the nodes

    while(getline(file, line)){
        stringstream ss(line);
        size_t p0, p1;
        if (ss >> p0 >> p1){
            cout << "Read Point success. Point for this edge:" << p0 << p1 << endl;
            Edge edge(p0,p1);
            graph.edges.push_back(edge);
        }
    }
}

void AttractForce(Edge &edge){
    Node &p1 = graph.nodes[edge.start];
    Node &p2 = graph.nodes[edge.end];

    double Fattract = kattract * ((p2.y - p1.y)*(p2.y - p1.y) + (p2.x - p1.x)*(p2.x - p1.x));
    double theta = atan2(p2.y - p1.y, p2.x - p1.x);

    p1.x += Fattract * cos(theta);
    p1.y += Fattract * sin(theta);

    p2.x -= Fattract * cos(theta);
    p2.y -= Fattract * sin(theta);
}

void RepulsiveForce(Node &p1, Node &p2){
    double Frepel = krepel / sqrt((p2.y - p1.y)*(p2.y - p1.y) + (p2.x - p1.x)*(p2.x - p1.x));
    double theta = atan2(p2.y - p1.y, p2.x - p1.x);
    p1.x -= Frepel * cos(theta);
    p1.y -= Frepel * sin(theta);

    p2.x += Frepel * cos(theta);
    p2.y += Frepel * sin(theta);
}


