# fastlcs: an efficient tool for solving LCS

**fastlcs** is a super fast c++ library for solving two classic LCS problems as below.

- [The longest common subsequence](https://en.wikipedia.org/wiki/Longest_common_subsequence) problem is to find the longest subsequence common to all sequences in a set of sequences (often just two sequences). Unlike substrings, subsequences are not required to occupy consecutive positions within the original sequences.

- [The longest common substring](https://en.wikipedia.org/wiki/Longest_common_substring) problem is to find a longest string that is a substring of two or more strings.

- [The Levenshtein distance](https://en.wikipedia.org/wiki/Levenshtein_distance) between two strings is the minimum number of single-character edits (insertions, deletions or substitutions) required to change one string into the other.

We support the following functions:

- **lcs_length**: compute the length of the longest common subsequence of two strings.

- **lcs**: get the position of the longest common subsequence in the original string.

- **lcsubstr_length**: compute the length of the longest common substring of two strings.

- **lcsubstr**: get the position of the longest common substring in the original string.

- **edit_distance**: compute the levenshtein distance between two strings.

- **lcs_length_group**: compute the length of the longest common subsequence between given string and the string in a list of strings. Return a list of lengths.

- **lcsubstr_length_group**: compute the length of the longest common substring between given string and the string in a list of strings. Return a list of lengths.

- **edit_distance_group**: compute the levenshtein distance between given string and the string in a list of strings. Return a list of distances.

In **lcs_length** and **lcs_length_group**, we support transforming the problem into solving the length of longest increasing subsequence by setting the third parameter of the function to true, processing faster in most cases. 

Also, fastlcs supports multi-thread processing feature in these functions: lcs_length_group/lcsubstr_length_group/edit_distance_group.

## Install

```shell
pip install git+https://github.com/zejunwang1/fastlcs
```

Alternatively, 

```shell
git clone https://github.com/zejunwang1/fastlcs
cd fastlcs/
python setup.py install
```

## C++ example

```cpp
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
```

Compile with cmake: 

```shell
mkdir build
cd build
cmake ..
make
./example
```

```context
The length of longest common subsequence: 13
The length of longest common substring: 10
Edit distance: 17


Start position of part of subsequence in s1: 0
Start position of part of subsequence in s2: 0
The length of this part: 10
Start position of part of subsequence in s1: 13
Start position of part of subsequence in s2: 18
The length of this part: 3


Start position of substring in s1: 0
Start position of substring in s2: 0
The length of substring: 10
```

## Python example

```python
import fastlcs

s1 = "fastlcs是一个高效的LCS求解工具"
s2 = "fastlcs是一个用于求解两个经典LCS问题的高效C++库"

print("The length of longest common subsequence: ", fastlcs.lcs_length(s1, s2, transform=True))
print("The length of longest common substring: ", fastlcs.lcsubstr_length(s1, s2))
print("Edit distance: ", fastlcs.edit_distance(s1, s2))

# The position of LCS in the original string
pos = fastlcs.lcs(s1, s2)    # list of tuple
for p in pos:
    print("Start position of part of subsequence in s1: ", p[0])
    print("Start position of part of subsequence in s2: ", p[1])
    print("The length of this part: ", p[2])

# The position of LCS(substring) in the original string
pos = fastlcs.lcsubstr(s1, s2)    # tuple
print("Start position of substring in s1: ", pos[1])
print("Start position of substring in s2: ", pos[2])
print("The length of substring: ", pos[0])

# Compute between given string and a list of strings
s2_list = [s2] * 10
len1 = fastlcs.lcs_length_group(s1, s2_list, transform=True, num_threads=2)
len2 = fastlcs.lcsubstr_length_group(s1, s2_list, num_threads=2)
dist = fastlcs.edit_distance_group(s1, s2_list, num_threads=2)
```

```context
The length of longest common subsequence:  13
The length of longest common substring:  10
Edit distance:  17
Start position of part of subsequence in s1:  0
Start position of part of subsequence in s2:  0
The length of this part:  10
Start position of part of subsequence in s1:  13
Start position of part of subsequence in s2:  18
The length of this part:  3
Start position of substring in s1:  0
Start position of substring in s2:  0
The length of substring:  10
```

## License

This project is released under [MIT license](https://github.com/zejunwang1/fastlcs/blob/main/LICENSE)


