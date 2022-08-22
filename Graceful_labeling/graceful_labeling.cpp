/**
 * @file graceful_labeling.cpp
 * @author Atilio Gomes Luiz
 * @brief Program that generates all graceful labelings of a graph if one exists
 * @version 0.1
 * @date 2022-08-21
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include <set>
#include <list>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <iostream>
using namespace std;

// a graph is implemented as a unordered_map
#define Graph unordered_map<int, list<int>>

// global variable that stores the number of edges of the graph
int number_of_edges {0}; 

// -------------------------------------------------------
// prototypes
void create_graph_from_file(Graph&, const string&);
void print_adj_list(const Graph&);
void generate_graceful_labeling(Graph&);
int backtrack(Graph&, int, vector<int>&, int, set<int>&);
int is_a_solution(const vector<int>&);
void print_solution(const vector<int>&);
int label_is_safe(int, Graph&, int, vector<int>&, set<int>&);
// -------------------------------------------------------

int main() 
{
    Graph G;
    create_graph_from_file(G, "edges.txt");
    cout << "order of G: " << G.size() << endl;
    print_adj_list(G);
    generate_graceful_labeling(G);
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
    afile.open(filename, ios::out | ios::in );
    while(afile.good()) {
        int v1, v2;
        afile >> v1 >> v2;
        graph[v1].push_back(v2);
        graph[v2].push_back(v1);
        number_of_edges++;
    }
    afile.close();
}

void print_adj_list(const Graph& graph) 
{
    for(auto p : graph) {
        cout << p.first << ": ";
        for(auto v : p.second) {
            cout << v << " ";
        }
        cout << endl;
    }
}

void generate_graceful_labeling(Graph& graph) 
{
    vector<int> labeling(graph.size(), -1); // solution vector
    set<int> edgeLabels; // auxiliary set to save edge labels
    backtrack(graph, number_of_edges, labeling, 0, edgeLabels);
}

int backtrack(Graph& G, int nEdges, vector<int>& labeling, int index, set<int>& edgeLabels) 
{
    if(is_a_solution(labeling)) {
        print_solution(labeling);
        return true;
    }

    for(int l = 0; l <= nEdges; ++l) {
        if(label_is_safe(l, G, index, labeling, edgeLabels)) {
            labeling[index] = l;

            for(const int& neighbor : G[index]) {
                if(labeling[neighbor] != -1) {
                    edgeLabels.insert(abs(labeling[index]-labeling[neighbor]));
                }
            }

            backtrack(G, nEdges, labeling, index+1, edgeLabels);

            // backtrack
            labeling[index] = -1;
            for(int neighbor : G[index]) {
                if(labeling[neighbor] != -1) {
                    edgeLabels.erase(abs(l-labeling[neighbor]));
                }
            }
        }
    }

    return false;
}

int is_a_solution(const vector<int>& labeling) 
{
    for(int v : labeling) {
        if(v == -1) {
            return false;
        }
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

// A label is safe if it was not assigned to a previous vertex 
// and if it does not generate a repeated edge label.
int label_is_safe(int l, Graph& G, int index, vector<int>& labeling, set<int>& edgeLabels) 
{
    for(int x : labeling) {
        if(l == x) return false;
    }
    set<int> auxList;
    for(const int& neighbor : G[index]) {
        if(labeling[neighbor] != -1) {
            int value = abs(l-labeling[neighbor]);
            if(edgeLabels.count(value) > 0 || auxList.count(value) > 0) {
                return false;
            }
            auxList.insert(value);
        }
    }
    return true;
}


