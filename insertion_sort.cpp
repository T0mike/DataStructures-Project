#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>

void insertion_sort(std::vector<int>& v)
{
    int temp;
    for(int i = 1; i < v.size() ; i++)
    {
        temp = v[i];
        int j = i - 1;
        while(j >= 0 && v[j] > temp)
        {
            v[j+1] = v[j];
            j--;
        }
        v[j + 1] = temp;
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
    insertion_sort(v);

    for(int i = 0; i < n; i++)
        std::cout << v[i] << " ";

    assert(std::is_sorted(v.begin(), v.end()));
}