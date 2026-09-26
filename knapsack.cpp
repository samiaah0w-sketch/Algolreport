#include <bits/stdc++.h>
using namespace std;

// =====================================================
// Item Structure
// =====================================================

struct Item
{
    int id;
    int weight;
    int profit;
    double ratio;
};

// =====================================================
// Compute Profit / Weight Ratio
// =====================================================

void computeRatios(vector<Item> &items)
{

    cout << fixed << setprecision(2);

    cout << "\nItem | Weight | Profit | Ratio(P/W)\n";
    cout << "------------------------------------\n";

    for (auto &item : items)
    {

        if (item.weight <= 0)
        {
            throw runtime_error(
                "Item weight must be greater than 0.");
        }

        item.ratio =
            static_cast<double>(item.profit) /
            item.weight;

        cout << "I" << item.id
             << "   | "
             << item.weight
             << "      | "
             << item.profit
             << "     | "
             << item.ratio
             << '\n';
    }
}

// =====================================================
// Sort Items by Ratio
// =====================================================

void sortByRatio(vector<Item> &items)
{

    sort(
        items.begin(),
        items.end(),
        [](const Item &a, const Item &b)
        {
            if (a.ratio != b.ratio)
                return a.ratio > b.ratio;

            // Tie-breaking for deterministic ordering
            return a.id < b.id;
        });
}

// =====================================================
// Fractional Knapsack - Greedy
// =====================================================

void fractionalKnapsack(
    vector<Item> items,
    double capacity)
{

    sortByRatio(items);

    double totalProfit = 0.0;
    double totalWeight = 0.0;
    double remaining = capacity;

    cout << "\n--- Fractional Knapsack (Greedy) ---\n";

    cout << "Order considered (by ratio desc): ";

    for (const auto &item : items)
    {
        cout << "I" << item.id << " ";
    }

    cout << "\n";

    for (const auto &item : items)
    {

        if (remaining <= 0.0)
            break;

        // Take the complete item
        if (item.weight <= remaining)
        {

            totalWeight += item.weight;
            totalProfit += item.profit;

            remaining -= item.weight;

            cout << "Take FULL Item "
                 << item.id
                 << " (w="
                 << item.weight
                 << ", p="
                 << item.profit
                 << ")\n";
        }

        // Take only the required fraction
        else
        {

            double fraction =
                remaining / item.weight;

            totalWeight += remaining;

            totalProfit +=
                item.profit * fraction;

            cout << "Take FRACTION "
                 << fraction
                 << " of Item "
                 << item.id
                 << " (w="
                 << item.weight
                 << ", p="
                 << item.profit
                 << ")\n";

            remaining = 0.0;
        }
    }

    cout << fixed << setprecision(2);

    cout << "Total Weight Used = "
         << totalWeight
         << '\n';

    cout << "Total Profit = "
         << totalProfit
         << '\n';
}

// =====================================================
// 0/1 Knapsack - Greedy
// =====================================================

void knapsack01Greedy(
    vector<Item> items,
    int capacity)
{

    sortByRatio(items);

    int totalProfit = 0;
    int totalWeight = 0;
    int remaining = capacity;

    vector<int> selected;

    cout << "\n--- 0/1 Knapsack "
            "(Greedy, ratio-based) ---\n";

    cout << "Order considered (by ratio desc): ";

    for (const auto &item : items)
    {
        cout << "I" << item.id << " ";
    }

    cout << "\n";

    for (const auto &item : items)
    {

        if (item.weight <= remaining)
        {

            totalWeight += item.weight;
            totalProfit += item.profit;

            remaining -= item.weight;

            selected.push_back(item.id);

            cout << "SELECT Item "
                 << item.id
                 << " (w="
                 << item.weight
                 << ", p="
                 << item.profit
                 << ")\n";
        }

        else
        {

            cout << "SKIP Item "
                 << item.id
                 << " (w="
                 << item.weight
                 << ", p="
                 << item.profit
                 << ") - does not fit\n";
        }
    }

    cout << "Selected Items: ";

    if (selected.empty())
    {
        cout << "None";
    }
    else
    {
        for (int id : selected)
        {
            cout << "I" << id << " ";
        }
    }

    cout << "\n";

    cout << "Total Weight = "
         << totalWeight
         << '\n';

    cout << "Total Profit (Greedy) = "
         << totalProfit
         << '\n';
}

// =====================================================
// 0/1 Knapsack - Dynamic Programming
// =====================================================

void knapsack01DP(
    const vector<Item> &items,
    int capacity)
{

    const int n =
        static_cast<int>(items.size());

    // dp[i][c] =
    // maximum profit using first i items
    // with capacity c

    vector<vector<int>> dp(
        n + 1,
        vector<int>(capacity + 1, 0));

    // Build DP table
    for (int i = 1; i <= n; ++i)
    {

        const int weight =
            items[i - 1].weight;

        const int profit =
            items[i - 1].profit;

        for (int c = 0; c <= capacity; ++c)
        {

            // Do not take current item
            dp[i][c] = dp[i - 1][c];

            // Take current item if it fits
            if (weight <= c)
            {

                dp[i][c] = max(
                    dp[i][c],
                    dp[i - 1][c - weight] + profit);
            }
        }
    }

    // -------------------------------------------------
    // Backtracking
    // -------------------------------------------------

    vector<int> selected;

    int remainingCapacity = capacity;

    int totalWeight = 0;

    for (int i = n; i >= 1; --i)
    {

        // Current item was selected
        if (
            dp[i][remainingCapacity] !=
            dp[i - 1][remainingCapacity])
        {

            selected.push_back(
                items[i - 1].id);

            totalWeight +=
                items[i - 1].weight;

            remainingCapacity -=
                items[i - 1].weight;
        }
    }

    // Reverse because backtracking
    // starts from the last item
    reverse(
        selected.begin(),
        selected.end());

    // -------------------------------------------------
    // Display Result
    // -------------------------------------------------

    cout << "\n--- 0/1 Knapsack "
            "(Dynamic Programming) ---\n";

    cout << "Selected Items: ";

    if (selected.empty())
    {
        cout << "None";
    }
    else
    {
        for (int id : selected)
        {
            cout << "I" << id << " ";
        }
    }

    cout << "\n";

    cout << "Total Weight = "
         << totalWeight
         << '\n';

    cout << "Total Profit (DP, Optimal) = "
         << dp[n][capacity]
         << '\n';
}

// =====================================================
// Main
// =====================================================

int main()
{

    try
    {

        // =================================================
        // TEST CASE 1
        // Fractional Knapsack
        // =================================================

        cout << "=================================================\n";
        cout << "TEST CASE 1: Fractional Knapsack "
                "(Greedy optimal)\n";
        cout << "=================================================\n";

        {
            vector<Item> items = {
                {1, 10, 60, 0.0},
                {2, 20, 100, 0.0},
                {3, 30, 120, 0.0}};

            const double capacity = 50.0;

            computeRatios(items);

            fractionalKnapsack(
                items,
                capacity);
        }

        // =================================================
        // TEST CASE 2
        // 0/1 Knapsack where Greedy Works
        // =================================================

        cout << "\n=================================================\n";
        cout << "TEST CASE 2: 0/1 Knapsack "
                "where Greedy WORKS\n";
        cout << "=================================================\n";

        {
            vector<Item> items = {
                {1, 2, 10, 0.0},
                {2, 3, 5, 0.0},
                {3, 5, 15, 0.0}};

            const int capacity = 5;

            // Copy used only for ratio display
            vector<Item> itemsCopy = items;

            computeRatios(itemsCopy);

            // Greedy
            knapsack01Greedy(
                itemsCopy,
                capacity);

            // Dynamic Programming
            knapsack01DP(
                items,
                capacity);
        }

        // =================================================
        // TEST CASE 3
        // 0/1 Knapsack where Greedy Fails
        // =================================================

        cout << "\n=================================================\n";
        cout << "TEST CASE 3: 0/1 Knapsack "
                "where Greedy FAILS\n";
        cout << "=================================================\n";

        {
            vector<Item> items = {
                {1, 10, 60, 0.0},
                {2, 20, 100, 0.0},
                {3, 30, 120, 0.0}};

            const int capacity = 50;

            // Copy for ratio calculation
            vector<Item> itemsCopy = items;

            computeRatios(itemsCopy);

            // Greedy
            knapsack01Greedy(
                itemsCopy,
                capacity);

            // Dynamic Programming
            knapsack01DP(
                items,
                capacity);
        }
    }

    catch (const exception &e)
    {

        cerr << "\nError: "
             << e.what()
             << '\n';

        return 1;
    }

    return 0;
}