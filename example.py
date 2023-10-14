# coding=utf-8

import fastlcs

s1 = "通过以上分析可见,南京财经大学、中央财经大学和上海立信会计金融学院三所高校税收专业的专业必修课课程设计大同小异,而三个学校专业选修课和实践课课程设置差距较大。"
s2 = "通过对南京财经大学、中央财经大学和上海立信会计金融学院三所高校税收专业的专业必修课、专业选修课和实践教学课的设置进行研究,为本专业课程的调整提供参考。"

print("LCS length by lcs_len_dp: ", fastlcs.lcs_len_dp(s1, s2))
print("LCS length by lcs_len_map: ", fastlcs.lcs_len_map(s1, s2))

print("LCS location information by lcs_dp:")
pos = fastlcs.lcs_dp(s1, s2)
for instance in pos:
    # the first element of instance corresponds to the location of longer string
    print("{}\t{}\t{}".format(instance[0], instance[1], instance[2]))

print("LCS location information by lcs_hirschberg:")
pos = fastlcs.lcs_hirschberg(s1, s2)
for instance in pos:
    # the first element of instance corresponds to the location of longer string
    print("{}\t{}\t{}".format(instance[0], instance[1], instance[2]))

print("LCS substring information by lcsubstr_dp:")
pos = fastlcs.lcsubstr_dp(s1, s2) # pos = fastlcs.lcsubstr_diag(s1, s2)
# the first element corresponds to the location of longer string
print("{}\t{}\t{}".format(pos[0], pos[1], pos[2]))

print("Levenshtein distance: ", fastlcs.edit_distance(s1, s2))
print("Levenshtein distance with k-bounded (k = 40): ", 
    fastlcs.edit_distance_k(s1, s2, 40))
print("Levenshtein distance with k-bounded (k = 20): ", 
    fastlcs.edit_distance_k(s1, s2, 20))

