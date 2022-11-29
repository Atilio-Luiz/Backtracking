/**
 * @file graceful_labeling.cpp
 * @author Atilio Gomes Luiz
 * @brief Program that generates all graceful labelings of a Wheel Graph with
 * the central vertex labeled with label 0.
 * 
 * A wheel graph, W_n, is the graph with n + 1 vertices and 2n edges formed
 * by joining a vertex v_0 to all the vertices (v_1,v_2,...,v_n) of an n-cycle.
 * Vertex v_0 is generally called the central vertex of the wheel and all the
 * other vertices are called the vertices of the outer cycle.
 * 
 * In this program, the wheel's central vertex has index 0 and all the
 * other vertices of the wheel are indexed by 1,2,...,n consecutively 
 * around the outer n-cycle.
 * 
 * The program receives as input a list of the edges of the wheel W_n and 
 * outputs a list in the form "[l0,l1,...,ln]", where li is the label 
 * of the vertex i, for 0 <= i <= n.
 * 
 * @version 0.1
 * @date 2022-11-29
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include <set>
#include <list>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <utility>
#include <cmath>
using namespace std;

// a graph is implemented as a unordered_map
#define Graph unordered_map<int, list<int>>

// global variable that stores the number of edges of the graph
int number_of_edges {0}; 

// global variable that count the number of graceful labelings
int total_of_labelings {0};

// -------------------------------------------------------
// prototypes
void create_wheel_graph(Graph&, int);
void print_adj_list(const Graph&);
void generate_graceful_labeling(Graph&);
int backtrack(Graph&, int, vector<int>&, int, set<int>&, vector<vector<int>>&);
int is_a_solution(const vector<int>&);
void print_solution(const vector<int>&);
void save_solution(const vector<int>&, vector<vector<int>>&);
void discard_complementary_labelings(vector<vector<int>>&, int n);
int label_is_safe(int, Graph&, int, vector<int>&, set<int>&);
// -----------------------------------------------------------

int main() 
{
    Graph G;
    create_wheel_graph(G, 4);
    cout << "order of the wheel graph G: " << G.size() << endl;
    cout << "Adjacency list:\n";
    print_adj_list(G);
    cout << "Graceful labelings:\n";
    generate_graceful_labeling(G);
    cout << "total = " << total_of_labelings << " labelings\n";
    return 0;
}

/**
 * @brief Create a wheel graph object with n + 1 vertices
 * 
 * @param graph the wheel graph with n + 1 vertices
 * @param n the number of vertices minus 1
 */
void create_wheel_graph(Graph& graph, int n) 
{
    vector<pair<int,int>> edges;
    for(int i = 1; i < n; ++i) 
        edges.push_back(make_pair(i,i+1));
    edges.push_back(make_pair(n,1));
    for(int i = 1; i <= n; ++i) 
        edges.push_back(make_pair(0,i));
    for(auto e : edges) {
        graph[e.first].push_back(e.second);
        graph[e.second].push_back(e.first);
        number_of_edges++;
    }
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
    vector<int> labeling(graph.size(), -1); // vector that saves the solution
    set<int> edgeLabels; // auxiliary set to save induced edge labels
    // vector that will save all the generated graceful labelings
    vector<vector<int>> grac_labelings;
    // we consider the central vertex v_0 with label 0 and, without loss of
    // generality, the vertex v_1 with label 2n.
    labeling[0] = 0;
    labeling[1] = 2 * ((int)graph.size()-1);
    edgeLabels.insert(2*((int)graph.size()-1));
    // calls recursive function to search for all graceful labelings
    backtrack(graph, number_of_edges, labeling, 2, edgeLabels, grac_labelings);
    // print only the labelings that are not complementary to each other
    discard_complementary_labelings(grac_labelings, graph.size()-1);
}

void save_solution(const vector<int>& labeling, vector<vector<int>>& allLabelings) {
    allLabelings.push_back(labeling);
}

int backtrack(Graph& G, int nEdges, vector<int>& labeling, int index, set<int>& edgeLabels, vector<vector<int>>& allLabelings) 
{
    if(is_a_solution(labeling)) {
        save_solution(labeling, allLabelings);
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

            backtrack(G, nEdges, labeling, index+1, edgeLabels, allLabelings);

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

void discard_complementary_labelings(vector<vector<int>>& allLabelings, int n) {
    vector<bool> complementary((int)allLabelings.size(), false);

    for(int i = 0; i < (int)allLabelings.size(); i++) {
        if(complementary[i] == false) {
            for(int j = i+1; j < (int)allLabelings.size(); j++) {
                bool complementary_flag {true};
                for(int k = 2; k <= ceil(((double)n)/2.0); ++k) {
                    if(allLabelings[i][k] != allLabelings[j][n+2-k] || allLabelings[j][k] != allLabelings[i][n+2-k]) {
                        complementary_flag = false;
                        //break;
                    }
                }
                if(complementary_flag == true) {
                    complementary[j] = true;
                }
            }
        }
    }
    // print all results
    for(int i = 0; i < (int)allLabelings.size(); i++) {
        if(complementary[i] == false) {
            total_of_labelings++;
            print_solution(allLabelings[i]);
        }
    }
}