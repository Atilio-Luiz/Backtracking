/**
 * @file allSubsets.cpp
 * @author Atilio Gomes Luiz
 * @brief Program that generates of subsets of a given set with n elements, n <= 50
 * @version 0.1
 * @date 2022-08-21
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include <iostream>
using namespace std;

#define MAXCANDIDATES 2
#define MAX 50

int is_a_solution(int k, int n);
void process_solution(int a[], int k);
void construct_candidates(int c[], int* nc);

void backtrack(int a[], int k, int n) {
    int c[MAXCANDIDATES];
    int nc;
    
    if(is_a_solution(k, n)) {
        process_solution(a, k);
    } 
    else {
        k += 1;
        construct_candidates(c, &nc);
        for(int i = 0; i < nc; ++i) {
            a[k] = c[i];
            backtrack(a, k, n);
        }
    }
}

int is_a_solution(int k, int n) {
    return k == n;
}

void construct_candidates(int c[], int* nc) {
    c[0] = true;
    c[1] = false;
    *nc = 2;
}

void process_solution(int a[], int k) {
    cout << "{";
    for(int i = 1; i <= k; ++i) {
        if(a[i]) {
            cout << " " << i;
        }
    }
    cout << " }\n";
}

void generate_subsets(int n) {
    int a[MAX];
    backtrack(a, 0, n);
}

int main() {
    generate_subsets(3);
}
