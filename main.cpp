#include <iostream>
#include <unordered_set>
#include <vector>

using namespace std;

class Rational {
public:
    Rational(int n, int d) : num(n), denom(d) {}
    bool operator==(const Rational& other) const {
        return num == other.num && denom == other.denom;
    }
    int num;
    int denom;
};

namespace std {
    template<> struct hash<Rational> {
        size_t operator()(const Rational& r) const {
            return hash<int>()(r.num) ^ hash<int>()(r.denom);
        }
    };
}

int main() {
    vector<Rational> input_data = { {1, 2}, {3, 4}, {1, 3}, {3, 5}, {2, 3}, {3, 4}, {1, 2} };
    unordered_set<Rational> unique_data(input_data.begin(), input_data.end());

    int m = unique_data.size();
    vector<vector<Rational>> hash_table(m);
    int k = m; // Set k to be the size of the hash table
    int max_iterations = 1000; // Limit the number of iterations to prevent an infinite loop
    int iteration_count = 0;
    while (true) {
        bool no_collision = true;
        vector<vector<Rational>> new_hash_table(m);
        vector<int> ks(m);
        for (const auto& r : unique_data) {
            int hash_index = hash<Rational>()(r) % m;
            int new_hash_index = (k * hash_index) % m;
            if (new_hash_table[new_hash_index].empty()) {
                new_hash_table[new_hash_index].push_back(r);
                ks[hash_index] = k;
            }
            else if (ks[hash_index] == k) {
                new_hash_table[new_hash_index].push_back(r);
            }
            else {
                no_collision = false;
                break;
            }
        }
        if (no_collision) {
            hash_table = move(new_hash_table);
            break;
        }
        k++;
        iteration_count++;
        if (iteration_count >= max_iterations) {
            cerr << "Error: Failed to find a non-colliding hash table after " << max_iterations << " iterations." << endl;
            return 1;
        }
    }

    // Print the hash table
    for (int i = 0; i < m; i++) {
        cout << i << ": ";
        for (const auto& r : hash_table[i]) {
            cout << r.num << "/" << r.denom << " ";
        }
        cout << endl;
    }

    return 0;
}
