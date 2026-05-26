#include <algorithm>
#include <cassert>
#include <iostream>
#include <vector>
using namespace std;

void custom_sort(std::vector<int>& a){
    int n = a.size();
    int i = 1;
    while(i < n){
        if(i == 0 || a[i - 1] <= a[i]){
            i++;
        }else{
            swap(a[i - 1], a[i]);
            i--;
        }
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
