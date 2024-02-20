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

