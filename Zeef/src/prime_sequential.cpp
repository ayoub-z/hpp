#include <iostream>
#include <vector>
#include <cmath>
#include <iostream>
#include <chrono>
#include <iomanip> // voor std::setprecision

int main(int argc, char** argv) {

    auto start_time = std::chrono::high_resolution_clock::now();
    const int n = 1000000000; // The total numbers we want to calculate prime numbers for 
    std::vector<int> prime(n, 1); // Initialize a vector with n number of elements, each being number 1

    int start_index = 2;
    int end_index = n;

    
    for (int k = start_index; k*k <= n; ++k) {        
        if (prime[k]) {
            // mark all multiples of i as non-pr2ime
            for (int j = k*2; j < n; j += k) {
                prime[j] = 0;
            }
        }
    }

    int local_count = 0;
    for (int i = start_index; i < end_index; ++i) {
        if (prime[i]) {
            local_count++;
        }
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);

    std::cout << "\nTotal amount of numbers: " << n << std::endl;
    std::cout << "Found prime numbers: " << local_count << std::endl; 

    // Convertet to seconds and round to 4 decimals
    double seconds = duration.count() / 1000000.0;
    if (seconds >= 1.0) {
        std::cout << "Execution time: " << std::setprecision(4) << std::fixed << seconds << " seconds" << std::endl;
    } else {
        std::cout << "Execution time: " << duration.count() << " microseconds" << std::endl;
    }

    return 0;
}
