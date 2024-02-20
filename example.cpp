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

