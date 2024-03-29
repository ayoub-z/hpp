#include <algorithm>
#include <iostream>
#include <vector>
#include <chrono>

using std::cout, std::endl, std::vector;
using namespace std::chrono;


vector<int> merge_sort(vector<int> xs){
	int unit = 1;

	while (unit <= int(xs.size())){
		for (int h = 0; h < int(xs.size()); h = h + unit * 2){
			int l = h;
			int r = std::min(int(xs.size()), (h + 2 * unit));
			int mid = h + unit;
			int p = l;
			int q = mid;
			while (p < mid && q < r){
				if (xs[p] <= xs[q]){
					p += 1;
				}
				else{
					int tmp = xs[q];

					// This for loop is the equivalent of "xs[p + 1: q + 1] = xs[p:q]" in Python.
					// for (int p_ = p; p_ < q; p_++){
					for (int q_ = q; q_ > p; q_--){
						xs[q_] = xs[q_-1];
					}	
					xs[p] = tmp;

					p += 1;
					mid += 1;
					q += 1;
				}
			}
		}
		unit *= 2;
	}
	return xs;
}

int main(){

	int tests = 8;
	int n = 1000;

	for (int test = 1; test < tests +1; test++){
		std::srand(unsigned(std::time(nullptr)));
		std::vector<int> arr(n);
		std::generate(arr.begin(), arr.end(), std::rand);

		cout << "Test: " << test << endl;
		cout << "Vector size: " << arr.size() << endl;
		auto start = high_resolution_clock::now();
		vector<int> result = merge_sort(arr);

		auto stop = high_resolution_clock::now();
		milliseconds duration = duration_cast<milliseconds>(stop - start);

		cout << "Total runtime: " << duration.count() << " milliseconds\n" << endl;
		n *= 2;
	}
}