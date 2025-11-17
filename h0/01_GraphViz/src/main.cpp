/*
 * TODO: complete this file comment.
 */
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <stdlib.h>
#include <chrono>
#include "SimpleGraph.h"

using namespace std;

void Welcome();
ifstream OpenFile();
int getMirsecond();
SimpleGraph getGraph(ifstream& input);
void getAttract(SimpleGraph& Graph,vector<Node>& delta_node);
void getRepulsive(SimpleGraph& Graph,vector<Node>& delta_node);
void sum_All(SimpleGraph& Graph,vector<Node>& delta_node);

// Main method
int main() {
    Welcome();
    ifstream input;
    input = OpenFile();
    int second = getMirsecond();
    SimpleGraph Graph = getGraph(input);
    InitGraphVisualizer(Graph);
    // Graph.drawGraph(Graph);
    cout << Graph.nodes.size() << " " << Graph.edges.size() << endl;
    // Graph.drawGraph(Graph);
    // _sleep(10000);
    cout << "change : ";
    vector<Node> delta_node(Graph.nodes.size(),{0.0,0.0});
    getAttract(Graph,delta_node);
    getRepulsive(Graph,delta_node);
    sum_All(Graph,delta_node);
    // InitGraphVisualizer(Graph);
    DrawGraph(Graph);
    Graph.drawGraph(Graph);

    auto start = std::chrono::high_resolution_clock::now();
    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    int milliseconds = elapsed.count();
    while(true){
            start = std::chrono::high_resolution_clock::now();
            ifstream newinput = OpenFile();
            second = getMirsecond();
            SimpleGraph graph = getGraph(newinput);
            InitGraphVisualizer(graph);
            DrawGraph(graph);
        end = std::chrono::high_resolution_clock::now();
        elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(start - end);
        milliseconds = elapsed.count();
    }

    return 0;
}

/* Prints a message to the console welcoming the user and
 * describing the program. */
void Welcome() {
    cout << "Welcome to CS106L GraphViz!" << endl;
    cout << "This program uses a force-directed graph layout algorithm" << endl;
    cout << "to render sleek, snazzy pictures of various graphs." << endl;
    cout << endl;
}

// 打开文件，读取
ifstream OpenFile(){
    ifstream input;
    // string path = "res/";
    while(!input.is_open()){
        cout << "Enter a file: ";
        string filename;
        getline(cin,filename);
        // filename = path + filename;
        cout << "filename is " << filename << endl;
        input.open(filename);
    }
    return input;
}

// 读取输入时间
int getMirsecond(){
    int second = 0;
    if(second <= 0){
        cout << "Enter Mirsecond : ";
        string line;
        getline(cin,line);
        second = stoi(line);
    }
    return second;
}

// 初始化节点，节点发布在圆上
SimpleGraph getGraph(ifstream& input){
    string line;
    int n = 0;
    getline(input,line);
    n = stoi(line);
    cout << "numline is " << n << endl;
    SimpleGraph graph;
    vector<Node> nodes;
    vector<Edge> edges;
    const double kpi = 3.14159265358979323;
    for(int k = 0;k < n;k++){
        Node node;
        node.x = cos(2*kpi*k/n);
        node.y = sin(2*kpi*k/n);
        cout << node.x << " " << node.y << " ";
        nodes.push_back(node);
    }
    graph.nodes = nodes;
    cout << "size is " << nodes.size() << endl;
    cout << "edge is : " << endl;

    while(getline(input,line)){
        istringstream iss(line);
        Edge edge;
        edge.start = 0;
        edge.end = 0;
        iss >> edge.start;
        iss >> edge.end;
        cout << edge.start << " " << edge.end << endl;
        edges.push_back(edge);
    }
    // Edge e;
    // for(size_t i = 0;i < edges.size();i++){
    //     e = edges[i];
    //     cout << e.start << " " << e.end << endl;
    // }
    graph.edges = edges;
    return graph;
}



void getAttract(SimpleGraph& Graph,vector<Node>& delta_node){
    vector<Edge> edges = Graph.edges;
    vector<Node> nodes = Graph.nodes;
    const double kattract = 0.001;
    for(auto edge: edges){
        size_t start = edge.start;
        size_t end = edge.end;
        Node n0 = nodes[start];
        Node n1 = nodes[end];
        double x0 = n0.x, y0 = n0.y, x1 = n1.x, y1 = n1.y;
        auto Fattract = kattract * ((y1-y0)*(y1-y0) + (x1-x0)*(x1-x0));
        auto theta = atan2(y1 - y0, x1 - x0);
        auto delta_x0 = Fattract * cos(theta);
        auto delta_y0 = Fattract * sin(theta);
        auto delta_x1 = Fattract * cos(theta);
        auto delta_y1 = Fattract * sin(theta);
        delta_node[start].x += delta_x0;
        delta_node[start].y += delta_y0;
        delta_node[end].x -= delta_x1;
        delta_node[end].y -= delta_y1;
    }
}


void getRepulsive(SimpleGraph& Graph,vector<Node>& delta_node){
    vector<Node>& nodes = Graph.nodes;
    const double krepel = 0.001;
    for(size_t i = 0;i < nodes.size();i++){
        for(size_t j = i + 1;j < nodes.size();j++){
            double xi = nodes[i].x;
            double yi = nodes[i].y;
            double xj = nodes[j].x;
            double yj = nodes[j].y;
            auto Frepel = krepel / sqrt((yj-yi)*(yj-yi) + (xj-xi)*(xj-xi));
            auto theta = atan2(yj - yi, xj - xi);
            auto delta_x0 = Frepel * cos(theta);
            auto delta_y0 = Frepel * sin(theta);
            auto delta_x1 = Frepel * cos(theta);
            auto delta_y1 = Frepel * sin(theta);
            delta_node[i].x -= delta_x0;
            delta_node[i].y -= delta_y0;
            delta_node[j].x += delta_x1;
            delta_node[j].y += delta_y1;
        }
    }
}

void sum_All(SimpleGraph& Graph,vector<Node>& delta_node){
    for(size_t i = 0;i < delta_node.size();i++){
        cout << "brefore: " << Graph.nodes[i].x << " " << Graph.nodes[i].y << endl;
        Graph.nodes[i].x += delta_node[i].x;
        Graph.nodes[i].y += delta_node[i].y;
        cout << "after :   " << Graph.nodes[i].x << " " << Graph.nodes[i].y << endl;
    }
}





