fastlcs: header-only library for solving LCS problems
=====================================================

fastlcs is a **header-only** library for solving classic LCS problems as
below.

-  `The longest common
   subsequence <https://en.wikipedia.org/wiki/Longest_common_subsequence>`__
   problem is to find the longest subsequence common to all sequences in
   a set of sequences (often just two sequences). Unlike substrings,
   subsequences are not required to occupy consecutive positions within
   the original sequences.

-  `The longest common
   substring <https://en.wikipedia.org/wiki/Longest_common_substring>`__
   problem is to find a longest string that is a substring of two or
   more strings.

-  `The Levenshtein
   distance <https://en.wikipedia.org/wiki/Levenshtein_distance>`__
   between two strings is the minimum number of single-character edits
   (insertions, deletions or substitutions) required to change one
   string into the other.

We implemented the following functions:

-  *lcs_len_dp*: Calculate the length of the longest common subsequence
   of two strings using dynamic programming.
-  *lcs_len_map*: Transform LCS length problem into solving LIS
   (`Longest Increasing
   Subsequence <https://en.wikipedia.org/wiki/Longest_increasing_subsequence>`__)
   length.
-  *lcs_dp*: Calculate the location Information of the longest common
   subsequence of two strings using dynamic programming.
-  *lcs_hirschberg*: Calculate the location Information of the longest
   common subsequence of two strings using `Hirschberg’s
   algorithm <https://en.wikipedia.org/wiki/Hirschberg%27s_algorithm>`__.
   It provides a **linear-space** solution.
-  *lcsubstr_dp / lcsubstr_diag*: Calculate the length and location
   Information of the longest common substring of two strings using
   dynamic programming.
-  *edit_distance*: Calculate the Levenshtein distance between two
   strings using dynamic programming.
-  *edit_distance_k*: Given a maximum edit distance, calculate the
   bounded Levenshtein distance between two strings using `Ukkonen’s
   algorithm <https://www.cs.helsinki.fi/u/ukkonen/InfCont85.PDF>`__. It
   is much more performant than edit distance for longer strings.

Assume string *a* has length *m*, string *b* has length *n*, the time
and space complexity of different algorithms are as follows.

=============== ================= ================
Algorithm       Time Complexity   Space Complexity
=============== ================= ================
lcs_len_dp      O(m*n)            O(min(m, n))
lcs_dp          O(m*n)            O(m*n)
lcs_hirschberg  O(m*n)            O(min(m, n))
lcsubstr_dp     O(m*n)            O(min(m, n))
lcsubstr_diag   O(m*n)            O(1)
edit_distance   O(m*n)            O(min(m, n))
edit_distance_k O(min(m, n) \* k) O(k)
=============== ================= ================

C++
---

.. code:: cpp

   #include "lcs.h"

   using namespace fastlcs;

   int main() {
     string s1 = "通过以上分析可见,南京财经大学、中央财经大学和上海立信会计金融学院三所高校税收专业的专业必修课课程设计大同小异,而三个学校专业选修课和实践课课程设置差距较大。";
     string s2 = "通过对南京财经大学、中央财经大学和上海立信会计金融学院三所高校税收专业的专业必修课、专业选修课和实践教学课的设置进行研究,为本专业课程的调整提供参考。";

     // lcs_len_dp
     uint32_t len = lcs_len_dp(s1, s2);
     cout << "LCS length by lcs_len_dp: " << len << '\n';

     // lcs_len_map
     len = lcs_len_map(s1, s2);
     cout << "LCS length by lcs_len_map: " << len << '\n';

     // lcs_dp
     uint32_t size = 0;
     Tuple* pos1 = lcs_dp(s1, s2, size);
     cout << "LCS location information by lcs_dp:\n";
     for (uint32_t i = 0; i < size; ++i)
       cout << pos1[i].b1 << " " << pos1[i].b2 << " " << pos1[i].len << '\n';
     if (pos1)
       free(pos1);
    
     // lcs_hirschberg
     Tuple* pos2 = lcs_hirschberg(s1, s2, size);
     cout << "LCS location information by lcs_hirschberg:\n";
     for (uint32_t i = 0; i < size; ++i)
       cout << pos2[i].b1 << " " << pos2[i].b2 << " " << pos2[i].len << '\n';
     if (pos2)
       free(pos2);
     
     // lcsubstr_dp or lcsubstr_diag
     Tuple t1 = lcsubstr_dp(s1, s2); // Tuple t1 = lcsubstr_diag(s1, s2);
     cout << "LCS substring information by lcsubstr_dp:\n";
     cout << t1.b1 << " " << t1.b2 << " " << t1.len << '\n';
     
     // edit_distance
     uint32_t distance = edit_distance(s1, s2);
     cout << "Levenshtein distance by edit_distance: " << distance << '\n';
     
     // edit_distance_k
     distance = edit_distance_k(s1, s2, 40);
     cout << "Levenshtein distance by edit_distance_k (k = 40): " << distance << '\n';
     distance = edit_distance_k(s1, s2, 20);
     cout << "Levenshtein distance by edit_distance_k (k = 20): " << distance << '\n';
   }

Compile with g++:

.. code:: shell

   g++ example.cpp -o example -O3 -march=native -funroll-loops

.. code:: context

   LCS length by lcs_len_dp: 52
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
   LCS substring information by lcsubstr_dp:
   9 3 38
   Levenshtein distance by edit_distance: 38
   Levenshtein distance by edit_distance_k (k = 40): 38
   Levenshtein distance by edit_distance_k (k = 20): 20

Python
------

Installation
~~~~~~~~~~~~

.. code:: shell

   pip install git+https://github.com/zejunwang1/fastlcs

Alternatively,

.. code:: shell

   git clone https://github.com/zejunwang1/fastlcs
   cd fastlcs/
   pip install .
   # python setup.py install

example
~~~~~~~

.. code:: python

   # coding=utf-8

   import fastlcs

   s1 = "通过以上分析可见,南京财经大学、中央财经大学和上海立信会计金融学院三所高校税收专业的专业必修课课程设计大同小异,而三个学校专业选修课和实践课课程设置差距较大。"
   s2 = "通过对南京财经大学、中央财经大学和上海立信会计金融学院三所高校税收专业的专业必修课、专业选修课和实践教学课的设置进行研究,为本专业课程的调整提供参考。"

   print("LCS length by lcs_len_dp: ", fastlcs.lcs_len_dp(s1, s2))
   print("LCS length by lcs_len_map: ", fastlcs.lcs_len_map(s1, s2))

   print("LCS location information by lcs_dp:")
   pos = fastlcs.lcs_dp(s1, s2)
   for instance in pos:
       print("{}\t{}\t{}".format(instance[0], instance[1], instance[2]))

   print("LCS location information by lcs_hirschberg:")
   pos = fastlcs.lcs_hirschberg(s1, s2)
   for instance in pos:
       print("{}\t{}\t{}".format(instance[0], instance[1], instance[2]))

   print("LCS substring information by lcsubstr_dp:")
   pos = fastlcs.lcsubstr_dp(s1, s2) # pos = fastlcs.lcsubstr_diag(s1, s2)
   print("{}\t{}\t{}".format(pos[0], pos[1], pos[2]))

   print("Levenshtein distance: ", fastlcs.edit_distance(s1, s2))
   print("Levenshtein distance with k-bounded (k = 40): ", 
       fastlcs.edit_distance_k(s1, s2, 40))
   print("Levenshtein distance with k-bounded (k = 20): ", 
       fastlcs.edit_distance_k(s1, s2, 20))

.. code:: context

   LCS length by lcs_len_dp:  52
   LCS length by lcs_len_map:  52
   LCS location information by lcs_dp:
   0   0   2
   9   3   38
   61  42  8
   69  52  1
   70  65  2
   78  74  1
   LCS location information by lcs_hirschberg:
   0   0   2
   9   3   37
   47  40  1
   61  42  8
   70  52  1
   72  54  2
   78  74  1
   LCS substring information by lcsubstr_dp:
   9   3   38
   Levenshtein distance:  38
   Levenshtein distance with k-bounded (k = 40):  38
   Levenshtein distance with k-bounded (k = 20):  20

Speed
~~~~~

We compared the processing speed of fastlcs with
`pylcs <https://github.com/Meteorix/pylcs>`__ on 150,000 similar
sentence pairs.

======= ======================== ===========
tool    func                     time cost/s
======= ======================== ===========
fastlcs lcs_len_dp               2.91
fastlcs lcs_len_map              **2.48**
pylcs   lcs                      9.97
fastlcs lcsubstr_dp              **1.44**
fastlcs lcsubstr_diag            1.82
pylcs   lcs2                     9.80
fastlcs edit_distance            3.41
fastlcs edit_distance_k (k = 40) **0.87**
pylcs   edit_distance            10.48
======= ======================== ===========

fastlcs is significantly faster than
`pylcs <https://github.com/Meteorix/pylcs>`__.

License
-------

This project is released under `MIT
license <https://github.com/zejunwang1/fastlcs/blob/main/LICENSE>`__
