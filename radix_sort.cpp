#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>

void counting_sort(std::vector<int>& v, int exp)
{
    int n = v.size();
    std::vector<int> count(10, 0);
    std::vector<int> output(n);

    for(int i = 0; i < n; i++)
    {
        int cifra = (v[i] / exp) % 10;
        count[cifra]++;
    }

    for(int i = 1; i < 10; i++)
        count[i] += count[i - 1];

    for(int i = n - 1; i >= 0; i--)
    {
        int cifra = (v[i] / exp) % 10;
        int poz = count[cifra] - 1;
        output[poz] = v[i];
        count[cifra]--;
    }

    for(int i = 0; i < n; i++)
        v[i] = output[i];
}

void custom_sort(std::vector<int> &v)
{
    int mxVal = *std::max_element(v.begin(), v.end());
    for(int exp = 1; mxVal / exp > 0; exp *= 10)
        counting_sort(v, exp);
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