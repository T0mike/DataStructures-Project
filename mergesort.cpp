#include <algorithm>
#include <cassert>
#include <iostream>
#include <vector>
using namespace std;

void merge(vector<int>& a, int st, int mij, int dr){
    vector<int> aux;
    aux.reserve(dr - st + 1);
    int i = st, j = mij + 1;
    while(i <= mij && j <= dr){
        if(a[i] <= a[j])aux.push_back(a[i++]);
        else aux.push_back(a[j++]);
    }

    while(i <= mij) aux.push_back(a[i++]);
    while(j <= dr) aux.push_back(a[j++]);
    for(int k = 0; k < (int)aux.size(); k++){
        a[st + k] = aux[k];
    }
}

void mergesort_rec(vector<int>& a, int st, int dr){
    if(st >= dr) return;
    int mij = (st + dr) / 2;
    mergesort_rec(a, st, mij);
    mergesort_rec(a, mij + 1, dr);
    merge(a, st, mij, dr);
}

void custom_sort(std::vector<int>& a){
    if(a.empty()) return;
    mergesort_rec(a, 0, (int)a.size() - 1);
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    std::cin >> n;
    std::vector<int> a(n);
    for (int& x : a) {
        std::cin >> x;
    }

    custom_sort(a);

    assert(std::ranges::is_sorted(a));
    return 0;
}
