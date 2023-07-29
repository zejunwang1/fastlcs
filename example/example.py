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

