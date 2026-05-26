#include <algorithm>
#include <cassert>
#include <iostream>
#include <vector>
using namespace std;

void custom_sort(std::vector<int>& a){
    int n = a.size();
    for(int i = 0; i < n - 1; i++){
        int schimb = 0;
        for(int j = 0; j < n - i - 1; j++){
            if(a[j] > a[j+1]){
                swap(a[j], a[j+1]);
                schimb = 1;
            }
        }
        if(!schimb) break;
    }
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
