#include <algorithm>
#include <cassert>
#include <cmath>
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

void insertionsort(vector<int>& a, int st, int dr){
    for(int i = st + 1; i <= dr; i++){
        int val = a[i];
        int j = i - 1;
        while(j >= st && a[j] > val){
            a[j + 1] = a[j];
            j--;
        }
        a[j+ 1] = val;
    }
}

void sift_down(vector<int>& a, int st, int n, int i){
    while(true){
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        int largest = i;
        if(left  < n && a[st + left]  > a[st + largest]) largest = left;
        if(right < n && a[st + right] > a[st + largest]) largest = right;
        if(largest == i) break;
        swap(a[st + i], a[st + largest]);
        i = largest;
    }
}

void heapsort(vector<int>& a, int st, int dr){
    int n = dr - st + 1;
    for(int i = n / 2 - 1; i >= 0; i--){
        sift_down(a, st, n, i);
    }
    for(int i = n - 1; i > 0; i--){
        swap(a[st], a[st + i]);
        sift_down(a, st, i, 0);
    }
}

void introsort_rec(vector<int>& a, int st, int dr, int depth_limit){
    const int THRESHOLD = 16;
    if(dr - st + 1 < THRESHOLD){
        insertionsort(a, st, dr);
        return;
    }
    if(depth_limit == 0){
        heapsort(a, st, dr);
        return;
    }
    int p = partition(a, st, dr);
    introsort_rec(a, st, p - 1, depth_limit - 1);
    introsort_rec(a, p + 1, dr, depth_limit - 1);
}

void custom_sort(std::vector<int>& a){
    if(a.empty()) return;
    int n = (int)a.size();
    int depth_limit = 2 * (int)log2(n);
    introsort_rec(a, 0, n - 1, depth_limit);
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
