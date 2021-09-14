#include <iostream>
#include <ctime>
#include <cstdlib>
#include <cassert>
#include <algorithm>
#include <chrono>

using namespace std;


void generate(int a[], int n) {
    for (int i = 0; i < n; ++i) {
        a[i] = rand();
    }
    sort(a, a+n);
}

void check() {
    const int num_checks = 100;
    const int max_size = 200000;
    const int num_searches = 2000000;

    int a[max_size], n;
    for (int run = 0; run < num_checks; ++run) {
        n = max_size;
        generate(a, n);

        auto t_classic = chrono::high_resolution_clock::now();
        int s_classic = 0;
        for (int i = 0; i < num_searches; ++i) {
            int existing = a[rand() % n];
            int most_likely_missing = rand();

            s_classic += classic_binary_search(a, n, existing);
            s_classic += classic_binary_search(a, n, most_likely_missing);
        }
        cout << "Classic #" << run << " - " << s_classic << ": "
            << chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now()-t_classic).count() << endl;


        auto t_cpp = chrono::high_resolution_clock::now();
        int s_cpp = 0;
        for (int i = 0; i < num_searches; ++i) {
            int existing = a[rand() % n];
            int most_likely_missing = rand();

            s_cpp += binary_search(a, a+n, existing);
            s_cpp += binary_search(a, a+n, most_likely_missing);
        }
        cout << "C++ #" << run << " - " << s_cpp << ": "
             << chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now()-t_cpp).count() << endl;

        auto t_fun = chrono::high_resolution_clock::now();
        int s_fun = 0;
        for (int i = 0; i < num_searches; ++i) {
            int existing = a[rand() % n];
            int most_likely_missing = rand();

            s_fun += fun_binary_search(a, n, existing);
            s_fun += fun_binary_search(a, n, most_likely_missing);
        }
        cout << "Fun #" << run << " - " << s_fun << ": "
             << chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now()-t_fun).count() << endl;


        assert(s_fun == s_cpp);
        assert(s_fun == s_classic);
        cout << "----" << endl;
    }
}

/*
 * Finds val in a.
 * Returns true if found, false if not found.
 */
bool classic_binary_search(const int a[], const int &n, const int &val) {
    int left = 0, right = n - 1;
    while (left <= right) {
        int mid = left + ((right - left) / 2);

        if (a[mid] == val) {
            return true;
        }

        if (a[mid] < val) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }

    return a[left - 1] == val;
}

/*
 * Finds val in a.
 * Returns true if found, false if not found.
 */
bool fun_binary_search(const int a[], const int &n, const int &val) {
    int step = 1;
    while (step < n) {
        step <<= 1;
    }

    int i = 0;
    while (step) {
        if (i + step < n && a[i + step] <= val)
            i += step;
        step >>= 1;
    }

    return a[i] == val;
}

int main() {
    srand((unsigned)time(0));
    check();
    return 0;
}
