#include <iostream>
#include <vector>
#include <chrono>

using namespace std;
using namespace chrono;

// Global variables
int N;

// col[row] = column where the queen is placed in that row
vector<int> col;

// Used columns and diagonals
vector<bool> colUsed;

// Diagonal 1: row - col + (N - 1)
// Diagonal 2: row + col
vector<bool> diag1Used;
vector<bool> diag2Used;

// Backtracking function
bool solveNQueens(int row)
{
    // All queens have been placed
    if (row == N)
        return true;

    // Try every column in the current row
    for (int c = 0; c < N; ++c)
    {
        int d1 = row - c + (N - 1);
        int d2 = row + c;

        // Check whether the position is safe
        if (!colUsed[c] &&
            !diag1Used[d1] &&
            !diag2Used[d2])
        {
            // Place queen
            col[row] = c;
            colUsed[c] = true;
            diag1Used[d1] = true;
            diag2Used[d2] = true;

            // Solve the next row
            if (solveNQueens(row + 1))
                return true;

            // Backtrack
            col[row] = -1;
            colUsed[c] = false;
            diag1Used[d1] = false;
            diag2Used[d2] = false;
        }
    }

    // No valid column found
    return false;
}

// Print the solution board
void printBoard()
{
    for (int r = 0; r < N; ++r)
    {
        for (int c = 0; c < N; ++c)
        {
            if (col[r] == c)
                cout << "Q ";
            else
                cout << ". ";
        }
        cout << '\n';
    }
}

int main()
{
    cout << "Enter N: ";

    if (!(cin >> N))
    {
        cout << "Invalid input. N must be an integer.\n";
        return 1;
    }

    if (N <= 0)
    {
        cout << "N must be a positive integer.\n";
        return 1;
    }

    // Initialize arrays
    col.assign(N, -1);
    colUsed.assign(N, false);

    // There are 2N - 1 possible diagonals
    diag1Used.assign(2 * N - 1, false);
    diag2Used.assign(2 * N - 1, false);

    // Measure only the solving function
    auto start = high_resolution_clock::now();

    bool found = solveNQueens(0);

    auto stop = high_resolution_clock::now();

    auto duration =
        duration_cast<microseconds>(stop - start);

    // Display results
    cout << "\nN = " << N << '\n';
    cout << "Solution Exists: "
         << (found ? "Yes" : "No") << '\n';

    cout << "Execution Time: "
         << duration.count()
         << " microseconds\n";

    // Display board if a solution exists
    if (found)
    {
        cout << "\nSolution Board:\n";
        printBoard();
    }
    else
    {
        cout << "\nNo solution exists for N = "
             << N << ".\n";
    }

    return 0;
}