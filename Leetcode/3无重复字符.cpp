/*
	给定一个字符串，请你找出其中不含有重复字符的 最长子串 的长度。
	输入: s = "abcabcbb"
	输出: 3 
	解释: 因为无重复字符的最长子串是 "abc"，所以其长度为 3。

*　时间主要花费在往前查找是否在子串中是否有相同字符，使用ASCII字符表可实现O(1)时间复杂度下查找
*/

class Solution {
public:
    int lengthOfLongestSubstring(string s) {
        int length = s.length();
        if(length == 0){
            return 0;
        }
        int longestSubstring = 0;
       
        for(int i = 0;i < length;++i){
            int sum = 1;
            bool equal = false;
            for(int j = i+1; j< length;++j){
                //该位置的字符与之前遍历过的所有字符比较，若都不相等则计数加一
                //比较
                //计数
                //赋值比较
                
                for(int k = j-1;k >= i ;--k){
                    if( s[j]==s[k]){
                        equal = true;
                        break;
                    }  
                }
                if(equal){
                    break;
                }
                ++sum;
            }
            if(longestSubstring < sum){
                longestSubstring = sum;
            }
            
        }
        return longestSubstring;
    }
};

/*
执行用时：268 ms, 在所有 C++ 提交中击败了7.00% 的用户
内存消耗：6.7 MB, 在所有 C++ 提交中击败了97.92% 的用户
*/

class Solution {
public:
    int lengthOfLongestSubstring(string s) {
        int length = s.length();
        if(length == 0){
            return 0;
        }
        int longestSubstring = 0;
        int span = 1;
        for(int i = 0;i < length;++i){
            int sum = 1;
            bool equal = false;
            for(int j = i+ span; j< length;++j){
                //使用k的for循环每次都会检查相同的字符，故设置span跨度，跳过部分检查
                
                for(int k = j-1;k >= i ;--k){
                    if( s[j]==s[k]){
                        span = j -k;
                        equal = true;
                        break;
                    }  
                }
                if(equal){
                    break;
                }
                if(sum == 1){
                    sum += span;
                }
                else{
                    ++sum;
                };
            }
            if(longestSubstring < sum){
                longestSubstring = sum;
            }
            
        }
        return longestSubstring;
    }
};

/*
执行用时：16 ms, 在所有 C++ 提交中击败了74.49% 的用户
内存消耗：6.8 MB, 在所有 C++ 提交中击败了90.61% 的用户
*/
