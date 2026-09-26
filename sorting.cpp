#include <bits/stdc++.h>
using namespace std;
using namespace std::chrono;

// =====================================================
// Bubble Sort
// =====================================================

void bubbleSort(vector<int> &a)
{
    const int n = static_cast<int>(a.size());

    for (int i = 0; i < n - 1; ++i)
    {
        bool swapped = false;

        for (int j = 0; j < n - i - 1; ++j)
        {
            if (a[j] > a[j + 1])
            {
                swap(a[j], a[j + 1]);
                swapped = true;
            }
        }

        // If no swap occurred, array is already sorted
        if (!swapped)
            break;
    }
}

// =====================================================
// Merge Sort
// =====================================================

void mergeParts(vector<int> &a, int l, int m, int r)
{

    vector<int> left(
        a.begin() + l,
        a.begin() + m + 1);

    vector<int> right(
        a.begin() + m + 1,
        a.begin() + r + 1);

    size_t i = 0;
    size_t j = 0;

    int k = l;

    while (i < left.size() && j < right.size())
    {

        if (left[i] <= right[j])
        {
            a[k++] = left[i++];
        }
        else
        {
            a[k++] = right[j++];
        }
    }

    while (i < left.size())
    {
        a[k++] = left[i++];
    }

    while (j < right.size())
    {
        a[k++] = right[j++];
    }
}

void mergeSortRec(vector<int> &a, int l, int r)
{

    if (l >= r)
        return;

    const int m = l + (r - l) / 2;

    mergeSortRec(a, l, m);

    mergeSortRec(a, m + 1, r);

    mergeParts(a, l, m, r);
}

void mergeSort(vector<int> &a)
{

    if (a.size() > 1)
    {
        mergeSortRec(
            a,
            0,
            static_cast<int>(a.size()) - 1);
    }
}

// =====================================================
// Quick Sort
// =====================================================

int partitionArray(
    vector<int> &a,
    int low,
    int high)
{

    const int pivot = a[high];

    int i = low - 1;

    for (int j = low; j < high; ++j)
    {

        if (a[j] < pivot)
        {
            ++i;
            swap(a[i], a[j]);
        }
    }

    swap(a[i + 1], a[high]);

    return i + 1;
}

void quickSortRec(
    vector<int> &a,
    int low,
    int high)
{

    while (low < high)
    {

        const int pi =
            partitionArray(a, low, high);

        /*
         * Recursively process the smaller partition first.
         * This reduces recursion stack depth.
         */
        if (pi - low < high - pi)
        {

            quickSortRec(
                a,
                low,
                pi - 1);

            low = pi + 1;
        }
        else
        {

            quickSortRec(
                a,
                pi + 1,
                high);

            high = pi - 1;
        }
    }
}

void quickSort(vector<int> &a)
{

    if (a.size() > 1)
    {

        quickSortRec(
            a,
            0,
            static_cast<int>(a.size()) - 1);
    }
}

// =====================================================
// Read Input File
// =====================================================

vector<int> readFile(
    const string &filename,
    int n)
{

    ifstream fin(filename);

    if (!fin)
    {
        throw runtime_error(
            "Cannot open input file: " + filename);
    }

    vector<int> a(n);

    for (int i = 0; i < n; ++i)
    {

        if (!(fin >> a[i]))
        {

            throw runtime_error(
                "Invalid or incomplete data in: " +
                filename);
        }
    }

    return a;
}

// =====================================================
// Measure Average Execution Time
// =====================================================

template <typename Func>
double measureAvgTime(
    Func sortFunc,
    const vector<int> &data,
    int runs)
{

    long long total = 0;

    for (int r = 0; r < runs; ++r)
    {

        // Copy is outside the timer
        vector<int> v = data;

        auto start =
            high_resolution_clock::now();

        // Only sorting is measured
        sortFunc(v);

        auto stop =
            high_resolution_clock::now();

        auto duration =
            duration_cast<microseconds>(
                stop - start);

        total += duration.count();

        // Verify that sorting was successful
        if (!is_sorted(v.begin(), v.end()))
        {

            throw runtime_error(
                "Sorting verification failed.");
        }
    }

    return static_cast<double>(total) / runs;
}

// =====================================================
// Main
// =====================================================

int main()
{

    const vector<int> sizes = {
        100,
        1000,
        10000,
        50000,
        100000};

    const int RUNS = 5;

    // -------------------------------------------------
    // Create results.csv
    // -------------------------------------------------

    ofstream results("results.csv");

    if (!results)
    {

        cerr << "Error: cannot create results.csv\n";

        return 1;
    }

    results
        << "InputSize,"
        << "BubbleSort,"
        << "MergeSort,"
        << "QuickSort\n";

    // -------------------------------------------------
    // Table Header
    // -------------------------------------------------

    cout
        << left
        << setw(12)
        << "Input Size"
        << setw(18)
        << "Bubble Sort(us)"
        << setw(18)
        << "Merge Sort(us)"
        << setw(18)
        << "Quick Sort(us)"
        << '\n';

    cout << string(66, '-') << '\n';

    // -------------------------------------------------
    // Run Experiment
    // -------------------------------------------------

    try
    {

        for (int n : sizes)
        {

            const string filename =
                "input" + to_string(n) + ".txt";

            // Read original data
            const vector<int> data =
                readFile(filename, n);

            // Bubble Sort
            const double bubbleTime =
                measureAvgTime(
                    [](vector<int> &v)
                    {
                        bubbleSort(v);
                    },
                    data,
                    RUNS);

            // Merge Sort
            const double mergeTime =
                measureAvgTime(
                    [](vector<int> &v)
                    {
                        mergeSort(v);
                    },
                    data,
                    RUNS);

            // Quick Sort
            const double quickTime =
                measureAvgTime(
                    [](vector<int> &v)
                    {
                        quickSort(v);
                    },
                    data,
                    RUNS);

            // Print results
            cout
                << left
                << setw(12)
                << n
                << setw(18)
                << fixed
                << setprecision(2)
                << bubbleTime
                << setw(18)
                << mergeTime
                << setw(18)
                << quickTime
                << '\n';

            // Save to CSV
            results
                << n << ','
                << bubbleTime << ','
                << mergeTime << ','
                << quickTime
                << '\n';
        }
    }
    catch (const exception &e)
    {

        cerr
            << "Error: "
            << e.what()
            << '\n';

        return 1;
    }

    cout
        << "\nResults saved to results.csv\n";

    return 0;
}