#include <algorithm>
#include <cassert>
#include <iostream>
#include <vector>
using namespace std;

int partition(vector<int>& a, int st, int dr){
    int mij = st + (dr - st)/2;
    if(a[mij] < a[st]) swap(a[st], a[mij]);
    if(a[dr]  < a[st])  swap(a[st], a[dr]);
    if(a[dr]  < a[mij]) swap(a[mij], a[dr]);

    swap(a[mij], a[dr]);

    int pivot = a[dr];
    int i = st - 1;
    for(int j = st; j < dr; j++){
        if(a[j] <= pivot){
            i++;
            swap(a[i], a[j]);
        }
    }
    swap(a[i + 1], a[dr]);
    return i + 1;
}

void quicksort_rec(vector<int>& a, int st, int dr){
    if(st >= dr)return;
    int p = partition(a, st, dr);
    quicksort_rec(a, st, p - 1);
    quicksort_rec(a, p + 1, dr);
}

void custom_sort(std::vector<int>& a){
    if(a.empty()) return;
    quicksort_rec(a, 0, (int)a.size() - 1);
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
