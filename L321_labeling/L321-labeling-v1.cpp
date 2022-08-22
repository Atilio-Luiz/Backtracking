/**
 * @file L321-labeling-v1.cpp
 * @author Atilio Gomes Luiz
 * @brief A program that generates all L(3,2,1)-labeling of a given graph with labels
 * from { 0, 1, ..., maxLabel }, where the integer maxLabel is defined manually 
 * in the main() function.
 * The graph is constructed from a list of edges contained in the text file "edges.txt"
 * @version 0.1
 * @date 2022-08-22
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include <list>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <iostream>
using namespace std;

// a graph is implemented as a unordered_map
#define Graph unordered_map<int, list<int>>

// prototypes --------------------------------------------
void create_graph_from_file(Graph&, const string&);
void generate_labeling(Graph&, const int&);
// -------------------------------------------------------

int main() 
{
    Graph G;
    int maximum_label { 7 }; // the maximum label used to construct the labeling
    create_graph_from_file(G, "edges.txt"); 
    generate_labeling(G, maximum_label); 
    return 0;
}

/**
 * @brief Create a graph from a list of edges contained in a file
 * 
 * @param graph 
 * @param filename 
 */
void create_graph_from_file(Graph& graph, const string& filename) 
{
    fstream afile;
    afile.open(filename, ios::in );
    while(afile.good()) {
        int v1, v2;
        afile >> v1 >> v2;
        graph[v1].push_back(v2);
        graph[v2].push_back(v1);
    }
    afile.close();
}

bool is_a_solution(const vector<int>& labeling) 
{
    for(const int& v : labeling) {
        if(v == -1) return false;
    }
    return true;
}

void print_solution(const vector<int>& labeling) 
{
    cout << "[";
    for(int i{0}; i < (int)labeling.size()-1; ++i) {
        cout << labeling[i] << ",";
    }
    cout << labeling[labeling.size()-1] << "]\n";
}

// A label is safe if it satisfies the L(3,2,1)-labelling restrictions
bool label_is_safe(const int& l, Graph& G, const int& index, const vector<int>& labeling) 
{
    // check vertices at distance 1 from G[index]
    for(const int& neighbor_1 : G[index]) {
		if(labeling[neighbor_1] != -1 && abs(l - labeling[neighbor_1]) < 3) {
			return false;
        }
        // check vertices at distance 2 from G[index]
        for(const int& neighbor_2 : G[neighbor_1]) {
            if(labeling[neighbor_2] != -1 && abs(l - labeling[neighbor_2]) < 2) {
                return false;
            }
            // check vertices at distance 3 from G[index]
            for(const int& neighbor_3 : G[neighbor_2]) {
                if(labeling[neighbor_3] != -1 && abs(l - labeling[neighbor_3]) < 1) {
                    return false;
                }
            }
        }
    }
    return true; 
}

void backtrack(Graph& G, vector<int>& labeling, const int& index, const int& maxLabel) 
{
    if(is_a_solution(labeling)) {
        print_solution(labeling);
    }
    else {
        for(int l = 0; l <= maxLabel; ++l) {
            if(label_is_safe(l, G, index, labeling)) {
                labeling[index] = l;
                backtrack(G, labeling, index+1, maxLabel);
                labeling[index] = -1; //backtrack
            }
        }
    }
}

void generate_labeling(Graph& graph, const int& maxLabel) 
{
    vector<int> labeling(graph.size(), -1);     // solution vector
    backtrack(graph, labeling, 0, maxLabel);    // backtracking procedure
}