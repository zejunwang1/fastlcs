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

