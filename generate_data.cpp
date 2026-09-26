#include <bits/stdc++.h>
using namespace std;

void generateFile(const string &filename, int n)
{
    ofstream fout(filename);

    if (!fout)
    {
        cerr << "Error: cannot open " << filename << " for writing.\n";
        return;
    }

    for (int i = 0; i < n; ++i)
    {
        fout << rand() % 100000 << ' ';
    }
}

int main()
{
    srand(static_cast<unsigned>(time(nullptr)));

    const vector<int> sizes = {
        100, 1000, 10000, 50000, 100000};

    for (int n : sizes)
    {
        const string filename =
            "input" + to_string(n) + ".txt";

        generateFile(filename, n);

        cout << "Generated " << filename << '\n';
    }

    cout << "All input files generated (run once).\n";

    return 0;
}