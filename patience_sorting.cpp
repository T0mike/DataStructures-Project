#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>
#include <queue>

void custom_sort(std::vector<int>& v)
{
    int n = v.size();
    std::vector<std::vector<int>> teancuri;
    
    for(int i = 0; i < n; i++)
    {
        bool ok = false;
        for(int j = 0; j < teancuri.size(); j++)
        {
            if(teancuri[j].back() >= v[i])
            {
                teancuri[j].push_back(v[i]);
                ok = true;
                break;
            }
        }
        if(!ok)
            teancuri.push_back({v[i]});
    }
    
    std::priority_queue<std::pair<int,int>, std::vector<std::pair<int,int>>, std::greater<std::pair<int,int>>> coada;
    
    for(int i = 0; i < teancuri.size(); i++)
        coada.push({teancuri[i].back(), i});
    
    int pos = 0;
    while(!coada.empty())
    {
        std::pair<int,int> varf = coada.top();
        coada.pop();
        int val = varf.first;
        int teanc = varf.second;
        v[pos++] = val;
        teancuri[teanc].pop_back();
        if(!teancuri[teanc].empty())
            coada.push({teancuri[teanc].back(), teanc});
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
    std::cout << std::endl;
    assert(std::is_sorted(v.begin(), v.end()));
}