#include <iostream>
#include <fstream>
#include <chrono>
#include <vector>
#include <algorithm>
#include <set>
#include <list>
#include <iomanip>
using namespace std;

// const int SZ = 20000, COLS = 3, ROWS = 4, TESTS = 4;
const int STRUCTURES = 3;
const int ROWS = 4, COLS = 3;
const int W1 = 10;
const int TRIALS = 15;

int main() {
    int results[TRIALS][ROWS][COLS];
    string cd;

    for (int j = 0; j < TRIALS; j++) {
        vector<string> data_vector;
        list<string> data_list;
        set<string> data_set;
        // testing for READ operations
        for (int i = 0; i < STRUCTURES; i++) {
            ifstream fin("codes.txt");
            auto start = chrono::high_resolution_clock::now();
            switch(i) {
                case 0: {  // read into a vector
                    while (fin >> cd)
                    data_vector.push_back(cd);
                    auto end = chrono::high_resolution_clock::now();
                    auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
                    results[j][0][i] = duration.count();
                    break;
                }
                case 1: {  // read into a list
                    while (fin >> cd)
                            data_list.push_back(cd);
                    auto end = chrono::high_resolution_clock::now();
                    auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
                    results[j][0][i] = duration.count();
                    break;
                }
                case 2: {  // read into a set
                    while (fin >> cd)
                            data_set.insert(cd);
                    auto end = chrono::high_resolution_clock::now();
                    auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
                    results[j][0][i] = duration.count();
                    break;
                }
            }
            fin.close();
        }

        // testing for SORT operations
        for (int i = 0; i < STRUCTURES; i++) {
            auto start = chrono::high_resolution_clock::now();
            switch(i) {
                case 0: {  // sort a vector
                    sort(data_vector.begin(), data_vector.end());
                    auto end = chrono::high_resolution_clock::now();
                    auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
                    results[j][1][i] = duration.count();
                    break;
                }
                case 1: {  // sort a list
                    data_list.sort();
                    auto end = chrono::high_resolution_clock::now();
                    auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
                    results[j][1][i] = duration.count();
                    break;
                }
                case 2: {  // can't sort a set, so set to -1
                    results[j][1][i] = -1;
                    break;
                }
            }
        }

        // testing for INSERT operations
        for (int i = 0; i < STRUCTURES; i++) {
            int ind_v = data_vector.size() / 2;
            int ind_l = data_list.size() / 2;
            auto start = chrono::high_resolution_clock::now();
            switch(i) {
                case 0: {  // insert into a vector
                    data_vector.insert(data_vector.begin() + ind_v, "TESTCODE");
                    auto end = chrono::high_resolution_clock::now();
                    auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
                    results[j][2][i] = duration.count();
                    break;
                }
                case 1: {  // insert into a list
                    auto it = data_list.begin();
                    advance(it, ind_l);
                    data_list.insert(it, "TESTCODE");
                    auto end = chrono::high_resolution_clock::now();
                    auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
                    results[j][2][i] = duration.count();
                    break;
                }
                case 2: {  // insert into a set
                    data_set.insert("TESTCODE");
                    auto end = chrono::high_resolution_clock::now();
                    auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
                    results[j][2][i] = duration.count();
                    break;
                }
            }
        }

        // testing for DELETE operations
        for (int i = 0; i < STRUCTURES; i++) {
            // select a target value in the vector 
            int ind = data_vector.size() / 2;
            string target_v = data_vector[ind];

            // select a target value in the list
            auto it1 = data_list.begin();
            advance(it1, ind);
            string target_l = *it1;

            // select a target value in the set
            auto it2 = data_set.begin();
            advance(it2, ind);
            string target_s = *it2;
            
            auto start = chrono::high_resolution_clock::now();
            switch(i) {
                case 0: {  // delete by value from vector
                    data_vector.erase(remove(data_vector.begin(), data_vector.end(), target_v));
                    auto end = chrono::high_resolution_clock::now();
                    auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
                    results[j][3][i] = duration.count();
                    break;
                }
                case 1: {  // delete by value from list
                    cout << "List" << data_list.size() << endl;
                    data_list.remove(target_l);
                    auto end = chrono::high_resolution_clock::now();
                    auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
                    results[j][3][i] = duration.count();
                    break;
                }
                case 2: {  // delete by value from set
                    data_set.erase(target_s);    
                    auto end = chrono::high_resolution_clock::now();
                    auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
                    results[j][3][i] = duration.count();
                    break;
                }
            }
        }
    }
    

    string labels[] = {"Read", "Sort", "Insert", "Delete"};
    cout << "Number of simulations: " << TRIALS;
    cout << setw(W1) << "Operation" << setw(W1) << "Vector" << setw(W1) << "List"
         << setw(W1) << "Set" << endl;
    for (int i = 0; i < 4; i++) {
        cout << setw(W1) << labels[i];
        for (int j = 0; j < COLS; j++) {
            //get total for each operation
            int total = 0;
            int count = 0;
            bool not_negative = false;
            for (int k = 0; k < TRIALS; k++) {
                if (results[k][i][j] != -1) {
                    total += results[k][i][j];
                    count++;
                    not_negative = true;
                }
            }
            if (not_negative) {
                cout << setw(W1) << total / count;
            }
            else {
                cout << setw(W1) << -1;
            }
            not_negative = false;
        }
        cout << endl;
    }
    return 0;
}