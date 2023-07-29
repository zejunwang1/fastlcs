#include <iostream>
#include "fastlcs.h"

int main() {
    string s1 = "fastlcs是一个高效的LCS求解工具";
    string s2 = "fastlcs是一个用于求解两个经典LCS问题的高效C++库";

    cout << "The length of longest common subsequence: " << lcs_length(s1, s2) << endl;
    cout << "The length of longest common substring: " << lcsubstr_length(s1, s2) << endl;
    cout << "Edit distance: " << edit_distance(s1, s2) << endl;
    
    // The position of LCS in the original string
    cout << endl << endl;
    const auto& pos = lcs(s1, s2);    // vector<tuple<int, int, int>>
    for (const auto& p : pos) {
        cout << "Start position of part of subsequence in s1: " << get<0>(p) << endl;
        cout << "Start position of part of subsequence in s2: " << get<1>(p) << endl;
        cout << "The length of this part: " << get<2>(p) << endl;
    }
    
    // The position of LCS(substring) in the original string
    cout << endl << endl;
    const auto& pos_str = lcsubstr(s1, s2);    // tuple<int, int, int>
    cout << "Start position of substring in s1: " << get<1>(pos_str) << endl;
    cout << "Start position of substring in s2: " << get<2>(pos_str) << endl;
    cout << "The length of substring: " << get<0>(pos_str) << endl;
    
    // Compute between given string and a list of strings
    vector<string> list;
    for (int i = 0; i < 10; i++)
        list.emplace_back(s2);
    // vector<int>
    const auto& length1 = lcs_length_group(s1, list, true, 2);    // transform=true  num_threads=2
    const auto& length2 = lcsubstr_length_group(s1, list, 2);    // num_threads=2
    const auto& distance = edit_distance_group(s1, list, 2);    // num_threads=2
    
    return 0;
}

