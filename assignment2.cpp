#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <omp.h>

using namespace std;

// Sequential bubble sort
void bubble_sort_seq(vector<int>& arr) {
    int n = arr.size();
    bool swapped = true;
    while (swapped) {
        swapped = false;
        for (int i = 1; i < n; i++) {
            if (arr[i] < arr[i-1]) {
                swap(arr[i], arr[i-1]);
                swapped = true;
            }
        }
    }
}

// Parallel bubble sort
void bubble_sort_par(vector<int>& arr) {
    int n = arr.size();
    bool swapped = true;
    while (swapped) {
        swapped = false;
	#pragma omp parallel for
        for (int i = 1; i < n; i += 2) {
            if (arr[i] < arr[i-1]) {
                swap(arr[i], arr[i-1]);
                swapped = true;
            }
        }
	#pragma omp parallel for
        for (int i = 2; i < n; i += 2) {
            if (arr[i] < arr[i-1]) {
                swap(arr[i], arr[i-1]);
                swapped = true;
            }
        }
    }
}

// Sequential merge sort
void merge_seq(vector<int>& arr, int l, int m, int r) {
    vector<int> left(m-l+1), right(r-m);
    for (int i = 0; i < left.size(); i++) {
        left[i] = arr[l+i];
    }
    for (int i = 0; i < right.size(); i++) {
        right[i] = arr[m+1+i];
    }

    int i = 0, j = 0, k = l;
    while (i < left.size() && j < right.size()) {
        if (left[i] <= right[j]) {
            arr[k] = left[i];
            i++;
        }
        else {
            arr[k] = right[j];
            j++;
        }
        k++;
    }

    while (i < left.size()) {
        arr[k] = left[i];
        i++;
        k++;
    }

    while (j < right.size()) {
        arr[k] = right[j];
        j++;
        k++;
    }
}

void merge_sort_seq(vector<int>& arr, int l, int r) {
    if (l < r) {
        int m = (l + r) / 2;
        merge_sort_seq(arr, l, m);
        merge_sort_seq(arr, m+1, r);
        merge_seq(arr, l, m, r);
    }
}

// Parallel merge sort
void merge_par(vector<int>& arr, int l, int m, int r) {
    vector<int> left(m-l+1), right(r-m);
    for (int i = 0; i < left.size(); i++) {
        left[i] = arr[l+i];
    }
    for (int i = 0; i < right.size(); i++) {
        right[i] = arr[m+1+i];
    }

    int i = 0, j = 0, k = l;
    while (i < left.size() && j < right.size()) {
        if (left[i] <= right[j]) {
            arr[k] = left[i];
            i++;
        }
        else {
            arr[k] = right[j];
            j++;
        }
        k++;
    }

    while (i < left.size()) {
        arr[k] = left[i];
        i++;
        k++;
    }

    while (j < right.size()) {
        arr[k] = right[j];
        j++;
        k++;
    }
}

void merge_sort_par(vector<int>& arr, int l, int r) {
    if (l < r) {
        int m = (l + r) / 2;
	#pragma omp parallel sections
        	{
		#pragma omp section
            		merge_sort_par(arr, l, m);
		#pragma omp section
            		merge_sort_par(arr, m+1, r);
        	}
        merge_par(arr, l, m, r);
    }
}

int main() {
    int n = 1000;
    vector<int> arr(n);
    srand(time(NULL));
    for (int i = 0; i < n; i++) {
        arr[i] = rand() % n;
    }

    // Sequential bubble sort
    vector<int> arr_seq = arr;
    auto start = chrono::high_resolution_clock::now();
    bubble_sort_seq(arr_seq);
    auto end = chrono::high_resolution_clock::now();
    cout << "Sequential Bubble Sort Time: " << chrono::duration_cast<chrono::microseconds>(end - start).count() << "ms" << endl;

    // Parallel bubble sort
    vector<int> arr_par = arr;
    start = chrono::high_resolution_clock::now();
    bubble_sort_par(arr_par);
    end = chrono::high_resolution_clock::now();
    cout << "Parallel Bubble Sort Time: " << chrono::duration_cast<chrono::microseconds>(end - start).count() << "ms" << endl;

    // Sequential merge sort
    arr_seq = arr;
    start = chrono::high_resolution_clock::now();
    merge_sort_seq(arr_seq, 0, n-1);
    end = chrono::high_resolution_clock::now();
    cout << "Sequential Merge Sort Time: " << chrono::duration_cast<chrono::microseconds>(end - start).count() << "ms" << endl;

    // Parallel merge sort
    arr_par = arr;
    start = chrono::high_resolution_clock::now();
    merge_sort_par(arr_par, 0, n-1);
    end = chrono::high_resolution_clock::now();
    cout << "Parallel Merge Sort Time: " << chrono::duration_cast<chrono::microseconds>(end - start).count() << "ms" << endl;

    // Speedup calculation
    double seq_bubble_sort_time = chrono::duration_cast<chrono::microseconds>(end - start).count();
    double seq_merge_sort_time = chrono::duration_cast<chrono::microseconds>(end - start).count();
    double par_bubble_sort_time = chrono::duration_cast<chrono::microseconds>(end - start).count();
    double par_merge_sort_time = chrono::duration_cast<chrono::microseconds>(end - start).count();
    double bubble_speedup = seq_bubble_sort_time / par_bubble_sort_time;
    double merge_speedup = seq_merge_sort_time / par_merge_sort_time;

    cout << "Bubble Sort Speedup: " << bubble_speedup << endl;
    cout << "Merge Sort Speedup: " << merge_speedup << endl;

    return 0;
}

 
   


