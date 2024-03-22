#include <iostream>
#include <mpi.h>
#include <omp.h>
#include <vector>
#include <algorithm> 
#include <chrono>
#include <iomanip> // voor std::setprecision

using std::cout;
using std::endl;
using std::vector;

// Function to print prime numbers
void print(vector<int> arr) {
    cout << "Prime numbers: ";
    for (int number : arr) {
        cout << number << " ";
    }
    cout << endl;
}

// Function to calculate prime numbers using Sieve of Eratosthenes algorithm
vector<int> get_prime_numbers(vector<int> array) {
    vector<int> prime_numbers;

    #pragma omp parallel for
    for (vector<int>::size_type i = 2; i < array.size(); ++i) {
        int k = array[i];
        if (k != 0) {
            prime_numbers.push_back(k);
            
            #pragma omp parallel for
            for (vector<int>::size_type j = i * 2; j < array.size(); j += i) {
                array[j] = 0;
            }
        }
    }

    return prime_numbers;
}

int main(int argc, char** argv) {
    auto start_time = std::chrono::high_resolution_clock::now();

    int id = -1, numProcesses = -1, length = -1;
    char myHostName[MPI_MAX_PROCESSOR_NAME];
    int size = 1e9;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &numProcesses);
    MPI_Get_processor_name(myHostName, &length);

    // omp_set_num_threads(4);
    cout << "numProcesses: " << numProcesses << endl;
    int chunkSize = (size / numProcesses);
    vector<int> array;

    // Divide work among MPI processes
    int start = id * chunkSize;
    int stop = (id == numProcesses - 1) ? size : (id + 1) * chunkSize;

    // Populate array with numbers
    for (int i = start; i <= stop; ++i) {
        array.push_back(i);
    }

    // Gather arrays from all MPI processes (not necessary for this version)
    // ...

    if (id == 0) {
        // Calculate prime numbers using OpenMP
        vector<int> prime_numbers = get_prime_numbers(array);

        // Print prime numbers
        cout << "List size: " << size << endl;
        cout << "Total prime numbers: " << prime_numbers.size() << endl;
        // print(prime_numbers);
    }

    MPI_Finalize();

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);

    // Convertet to seconds and round to 4 decimals
    double seconds = duration.count() / 1000000.0;
    if (seconds >= 1.0) {
        cout << "Execution time: " << std::setprecision(4) << std::fixed << seconds << " seconds" << endl;
    } else {
        cout << "Execution time: " << duration.count() << " microseconds" << endl;
    }

    return 0;
}
