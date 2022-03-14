#include <algorithm>
#include <iostream>
#include <vector>
#include <chrono>
#include <thread>

using std::cout, std::endl, std::vector, std::thread;
using namespace std::chrono;

void merge(int *haystack, int unit, int part, int start){
    for (int h = (0 + start); h < (part + start); h = h + unit * 2){
        int l = h;
        int r = std::min((part + start), (h + 2 * unit));
        int mid = h + unit;
        int p = l;
        int q = mid;
        while (p < mid && q < r){
            if (haystack[p] <= haystack[q]){
                p += 1;
            }
            else{
                int tmp = haystack[q];
                // This for loop is the equivalent of 
                // "haystack[p+1:q+1] = haystack[p:q]" in Python.
                for (int q_ = q; q_ > p; q_--){
                    haystack[q_] = haystack[q_-1];
                }          
                haystack[p] = tmp;

                p += 1;
                mid += 1;
                q += 1;
            }
        }
    }
}

void threaded_merge_sort(int *haystack, int num_threads, int size){
    int new_threads = num_threads;
    while(new_threads >= 1){
        int part = size / new_threads;
        int unit = 1;
        thread thr[new_threads];
        while (unit <= part){ // Merge as far as we're able to with threading
            int t = 0;
            // Initialize threads with their tasks
            for (int start = 0; start < size; start+= part){
                thr[t] = thread(merge,haystack, unit, part, start);
                t++;
            }
            // Activate threads and wait for them to finish
            for (t = 0; t < new_threads; t++){
                thr[t].join();
            }
            unit *= 2;
        }
        new_threads /=2;
    }
}

void merge_sort(int *haystack, int size){
    int unit = 1;

    while (unit <= size){
        merge(haystack, unit, size, 0);
        unit *= 2;
    }
}

int main(){
    int n = 100000;
    int num_threads = 8;

    srand(time(NULL));

    for (int threads = 1; threads < num_threads +1; threads*=2){
        int *haystack = (int *)malloc(sizeof(int) * n); // pointer to allocated memory of size n
        for (int i = 0; i < n; i++){ // fill with random numbers between 0 - 1000
            haystack[i] = rand() % 1000;
        }

        int *haystack2 = (int *)malloc(sizeof(int) * n);
        for (int i = 0; i < n; i++){
            haystack2[i] = rand() % 1000;
        }        

        cout << "Threads: " << threads << endl;
        cout << "Size: " << n << endl;

        auto start = high_resolution_clock::now();
        threaded_merge_sort(haystack, threads, n);
        auto stop = high_resolution_clock::now();
        milliseconds duration = duration_cast<milliseconds>(stop - start);
        cout << "Threaded mergesort: " << duration.count() << " milliseconds" << endl;

        auto start2 = high_resolution_clock::now();
        merge_sort(haystack2, n);
        auto stop2 = high_resolution_clock::now();
        milliseconds duration2 = duration_cast<milliseconds>(stop2 - start2);
        cout << "Mergesort: " << duration2.count() << " milliseconds\n" << endl;
    }
}