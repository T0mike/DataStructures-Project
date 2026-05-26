#include <algorithm>
#include <climits>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <vector>
using namespace std;

static void usage(const char* prog) {
    cerr << "Usage: " << prog << " <type> <n> [seed] [--out <prefix>]\n";
    cerr << "Tipuri:\n";
    cerr << "  random         valori uniforme pe int32 (INT_MIN..INT_MAX)\n";
    cerr << "  sorted         crescator (best case pentru bubble/gnome/insertion)\n";
    cerr << "  reverse        descrescator (worst case pentru bubble/gnome/insertion)\n";
    cerr << "  equal          toate egale (kill pentru Lomuto quick)\n";
    cerr << "  few_distinct   doar 10 valori distincte (duplicate masive)\n";
    cerr << "  nearly_sorted  sortat + ~1%% swap-uri random\n";
    cerr << "  sawtooth       run-uri crescatoare de lungime 100 (testeaza patience)\n";
    cerr << "  organ_pipe     creste pana la mijloc, apoi scade\n";
    cerr << "Fara --out: scrie inputul la stdout.\n";
    cerr << "Cu --out <prefix>: scrie <prefix>.in si <prefix>.ok.\n";
}

int main(int argc, char* argv[]) {
    if (argc < 3) { usage(argv[0]); return 1; }

    string type = argv[1];
    long long n_ll = atoll(argv[2]);
    if (n_ll < 0 || n_ll > 100000000LL) {
        cerr << "n in afara intervalului [0, 1e8]\n";
        return 1;
    }
    int n = (int)n_ll;

    unsigned int seed = 42u;
    string outPrefix;
    int argi = 3;
    if (argi < argc && argv[argi][0] != '-') {
        seed = (unsigned int)atoll(argv[argi]);
        argi++;
    }
    while (argi < argc) {
        if (strcmp(argv[argi], "--out") == 0 && argi + 1 < argc) {
            outPrefix = argv[argi + 1];
            argi += 2;
        } else {
            cerr << "Argument necunoscut: " << argv[argi] << "\n";
            return 1;
        }
    }

    mt19937 rng(seed);
    vector<int> a(n);

    if (type == "random") {
        uniform_int_distribution<int> dist(INT_MIN, INT_MAX);
        for (int& x : a) x = dist(rng);
    } else if (type == "sorted") {
        for (int i = 0; i < n; i++) a[i] = i;
    } else if (type == "reverse") {
        for (int i = 0; i < n; i++) a[i] = n - 1 - i;
    } else if (type == "equal") {
        uniform_int_distribution<int> vdist(-1000000, 1000000);
        int v = vdist(rng);
        for (int& x : a) x = v;
    } else if (type == "few_distinct") {
        const int K = 10;
        vector<int> values(K);
        uniform_int_distribution<int> vdist(INT_MIN, INT_MAX);
        for (int& v : values) v = vdist(rng);
        uniform_int_distribution<int> pick(0, K - 1);
        for (int& x : a) x = values[pick(rng)];
    } else if (type == "nearly_sorted") {
        for (int i = 0; i < n; i++) a[i] = i;
        int swaps = max(1, n / 100);
        uniform_int_distribution<int> dist(0, max(0, n - 1));
        for (int i = 0; i < swaps; i++) swap(a[dist(rng)], a[dist(rng)]);
    } else if (type == "sawtooth") {
        const int runLen = 100;
        for (int i = 0; i < n; i++) a[i] = i % runLen;
    } else if (type == "organ_pipe") {
        int half = n / 2;
        for (int i = 0; i < half; i++) a[i] = i;
        for (int i = half; i < n; i++) a[i] = n - 1 - i;
    } else {
        cerr << "Tip necunoscut: " << type << "\n";
        usage(argv[0]);
        return 1;
    }

    auto writeArray = [&](ostream& os, bool withN) {
        if (withN) os << n << '\n';
        for (int x : a) os << x << '\n';
    };

    if (outPrefix.empty()) {
        ios::sync_with_stdio(false);
        cout.tie(nullptr);
        writeArray(cout, true);
    } else {
        {
            ofstream fin(outPrefix + ".in");
            writeArray(fin, true);
        }
        sort(a.begin(), a.end());
        {
            ofstream fok(outPrefix + ".ok");
            writeArray(fok, true);
        }
        cerr << "Generat " << outPrefix << ".in si " << outPrefix << ".ok (n=" << n << ", tip=" << type << ", seed=" << seed << ")\n";
    }

    return 0;
}
