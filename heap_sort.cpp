#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>

void heapify_down(std::vector<int>& v, int i, int n)
{
    int st = 2 * i + 1;
    int dr = 2 * i + 2;
    int mx = i;
    
    if(st < n && v[st] > v[mx])
        mx = st;
    if(dr < n && v[dr] > v[mx])
        mx = dr;
    if(mx != i)
    {
        std::swap(v[i], v[mx]);
        heapify_down(v, mx, n);
    }
}

void custom_sort(std::vector<int>& v)
{
    int n = v.size();

    //construiesc max-heap
    for(int i = n / 2 - 1; i >= 0; i--)
        heapify_down(v, i, n);
    
    //sortam crescator folosind maximul pe care il mutam la final
    for (int i = n - 1; i > 0; i--)
    {
        std::swap(v[0], v[i]);
        heapify_down(v, 0, i);
    }
}

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    std::cin >> n;
    std::vector<int> v(n);
    for(int i = 0; i < n; i++)
        std::cin >> v[i];
    custom_sort(v);
    for(int i = 0; i < n; i++)
        std::cout << v[i] << " ";

    assert(std::is_sorted(v.begin(), v.end()));
}