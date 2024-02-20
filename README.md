# fastlcs: an effective tool for solving LCS problems

fastlcs is an effective tool for solving classic LCS problems as below.

- [The longest common subsequence](https://en.wikipedia.org/wiki/Longest_common_subsequence) problem is to find the longest subsequence common to all sequences in a set of sequences (often just two sequences). Unlike substrings, subsequences are not required to occupy consecutive positions within the original sequences.

- [The longest common substring](https://en.wikipedia.org/wiki/Longest_common_substring) problem is to find a longest string that is a substring of two or more strings.

- [The Levenshtein distance](https://en.wikipedia.org/wiki/Levenshtein_distance) between two strings is the minimum number of single-character edits (insertions, deletions or substitutions) required to change one string into the other.

We implemented the following functions:

- *lcs_len*: Calculate the length of the longest common subsequence of two strings using dynamic programming.
- *lcs_len_map*: Transform LCS length problem into solving LIS ([Longest Increasing Subsequence](https://en.wikipedia.org/wiki/Longest_increasing_subsequence)) length.
- *lcs_dp*: Calculate the location Information of the longest common subsequence of two strings using dynamic programming.
- *lcs_hirschberg*: Calculate the location Information of the longest common subsequence of two strings using [Hirschberg's algorithm](https://en.wikipedia.org/wiki/Hirschberg%27s_algorithm). It provides a **linear-space** solution.
- *lcs_myers*: Calculate the location Information of the longest common subsequence of two strings using [Myer's diff algorithm](https://neil.fraser.name/writing/diff/myers.pdf). It provides a **linear-space** solution.
- *longest_match / longest_match_diag*: Calculate the length and location Information of the longest common substring of two strings using dynamic programming.
- *edit_distance*: Calculate the Levenshtein distance between two strings using dynamic programming.
- *edit_distance_k*: Given a maximum edit distance, calculate the bounded Levenshtein distance between two strings using [Ukkonen's algorithm](https://www.cs.helsinki.fi/u/ukkonen/InfCont85.PDF). It is much more performant than edit distance for longer strings.

Assume string *a* has length *m*, string *b* has length *n*, the time and space complexity of different algorithms are as follows.

| Algorithm          | Time Complexity  | Space Complexity |
|:------------------ | ---------------- | ---------------- |
| lcs_len            | O(m*n)           | O(min(m, n))     |
| lcs_dp             | O(m*n)           | O(m*n)           |
| lcs_hirschberg     | O(m*n)           | O(min(m, n))     |
| lcs_myers          | O((m+n)*D)       | O(m+n)           |
| longest_match      | O(m*n)           | O(min(m, n))     |
| longest_match_diag | O(m*n)           | O(1)             |
| edit_distance      | O(m*n)           | O(min(m, n))     |
| edit_distance_k    | O(min(m, n) * k) | O(k)             |

## C++

```cpp
#include <iostream>
#include "lcs.h"

using namespace std;
using namespace lcs;

int main() {
  string s1 = "通过以上分析可见,南京财经大学、中央财经大学和上海立信会计金融学院三所高校税收专业的专业必修课课程设计大同小异,而三个学校专业选修课和实践课课程设置差距较大。";
  string s2 = "通过对南京财经大学、中央财经大学和上海立信会计金融学院三所高校税收专业的专业必修课、专业选修课和实践教学课的设置进行研究,为本专业课程的调整提供参考。";

  // lcs_len
  code_t len = strlcs_len(s1, s2);
  cout << "LCS length by lcs_len: " << len << '\n';

  // lcs_len_map
  len = strlcs_len_map(s1, s2);
  cout << "LCS length by lcs_len_map: " << len << '\n';

  // lcs_dp
  auto res_dp = strlcs_dp(s1, s2);
  cout << "LCS location information by lcs_dp:\n";
  for (auto& t : res_dp)
    cout << get<0>(t) << " " << get<1>(t) << " " << get<2>(t) << '\n';
 
  // lcs_hirschberg
  auto res_hirschberg = strlcs_hirschberg(s1, s2);
  cout << "LCS location information by lcs_hirschberg:\n";
  for (auto& t : res_hirschberg)
    cout << get<0>(t) << " " << get<1>(t) << " " << get<2>(t) << '\n';

  // lcs_myers
  auto res_myers = strlcs_myers(s1, s2);
  cout << "LCS location information by lcs_myers:\n";
  for (auto& t : res_myers)
    cout << get<0>(t) << " " << get<1>(t) << " " << get<2>(t) << '\n';
  
  // longest_match or longest_match_diag
  auto t = longest_match(s1, s2); // auto t = lcsubstr_diag(s1, s2);
  cout << "LCS substring information by longest_match:\n";
  cout << get<0>(t) << " " << get<1>(t) << " " << get<2>(t) << '\n';
  
  // edit_distance
  code_t distance = str_edit_distance(s1, s2);
  cout << "Levenshtein distance by edit_distance: " << distance << '\n';
  
  // edit_distance_k
  distance = str_edit_distance_k(s1, s2, 40);
  cout << "Levenshtein distance by edit_distance_k (k = 40): " << distance << '\n';
  distance = str_edit_distance_k(s1, s2, 20);
  cout << "Levenshtein distance by edit_distance_k (k = 20): " << distance << '\n';
}
```

Compile with g++:

```shell
g++ example.cpp src/lcs.cpp -o example -I ./src/ -O3 -march=native
```

```context
LCS length by lcs_len: 52
LCS length by lcs_len_map: 52
LCS location information by lcs_dp:
0 0 2
9 3 38
61 42 8
69 52 1
70 65 2
78 74 1
LCS location information by lcs_hirschberg:
0 0 2
9 3 37
47 40 1
61 42 8
70 52 1
72 54 2
78 74 1
LCS location information by lcs_myers:
0 0 2
9 3 38
61 42 8
70 52 1
72 54 2
78 74 1
LCS substring information by longest_match:
9 3 38
Levenshtein distance by edit_distance: 38
Levenshtein distance by edit_distance_k (k = 40): 38
Levenshtein distance by edit_distance_k (k = 20): 20
```

## Python

### Installation

```shell
pip install git+https://github.com/zejunwang1/fastlcs
```

Alternatively,

```shell
git clone https://github.com/zejunwang1/fastlcs
cd fastlcs/
pip install .
# python setup.py install
```

### example

```python
# coding=utf-8

from fastlcs import lcs_len, lcs_len_map
from fastlcs import lcs_dp, lcs_hirschberg, lcs_myers
from fastlcs import longest_match, longest_match_diag
from fastlcs import edit_distance, edit_distance_k

s1 = "通过以上分析可见,南京财经大学、中央财经大学和上海立信会计金融学院三所高校税收专业的专业必修课课程设计大同小异,而三个学校专业选修课和实践课课程设置差距较大。"
s2 = "通过对南京财经大学、中央财经大学和上海立信会计金融学院三所高校税收专业的专业必修课、专业选修课和实践教学课的设置进行研究,为本专业课程的调整提供参考。"

print("LCS length by lcs_len: ", lcs_len(s1, len(s1), s2, len(s2)))
print("LCS length by lcs_len_map: ", lcs_len_map(s1, len(s1), s2, len(s2)))

print("LCS location information by lcs_dp:")
pos = lcs_dp(s1, len(s1), s2, len(s2))
for instance in pos:
    print("{}\t{}\t{}".format(instance[0], instance[1], instance[2]))

print("LCS location information by lcs_hirschberg:")
pos = lcs_hirschberg(s1, len(s1), s2, len(s2))
for instance in pos:
    print("{}\t{}\t{}".format(instance[0], instance[1], instance[2]))

print("LCS location information by lcs_myers:")
pos = lcs_myers(s1, len(s1), s2, len(s2))
for instance in pos:
    print("{}\t{}\t{}".format(instance[0], instance[1], instance[2]))

print("LCS substring information by longest_match:")
pos = longest_match(s1, len(s1), s2, len(s2)) 
# pos = longest_match_diag(s1, len(s1), s2, len(s2))
print("{}\t{}\t{}".format(pos[0], pos[1], pos[2]))

print("Levenshtein distance: ", edit_distance(s1, len(s1), s2, len(s2)))
print("Levenshtein distance with k-bounded (k = 40): ", 
    edit_distance_k(s1, len(s1), s2, len(s2), 40))
print("Levenshtein distance with k-bounded (k = 20): ", 
    edit_distance_k(s1, len(s1), s2, len(s2), 20))
```

```context
LCS length by lcs_len:  52
LCS length by lcs_len_map:  52
LCS location information by lcs_dp:
0	0	2
9	3	38
61	42	8
69	52	1
70	65	2
78	74	1
LCS location information by lcs_hirschberg:
0	0	2
9	3	37
47	40	1
61	42	8
70	52	1
72	54	2
78	74	1
LCS location information by lcs_myers:
0	0	2
9	3	38
61	42	8
70	52	1
72	54	2
78	74	1
LCS substring information by longest_match:
9	3	38
Levenshtein distance:  38
Levenshtein distance with k-bounded (k = 40):  38
Levenshtein distance with k-bounded (k = 20):  20
```

## License

This project is released under [MIT license](https://github.com/zejunwang1/fastlcs/blob/main/LICENSE)


