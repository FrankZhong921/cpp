/*

	给你一个字符串 s，找到 s 中最长的回文子串。
	输入：s = "babad"
	输出："bab"
	解释："aba" 同样是符合题意的答案。
*/


class Solution {
public:
    string longestPalindrome(string s) {
        //滑动窗口
        
        if(s.size() == 0){
            return "";
        }
        string str = s.substr(0,1);
        int cur = 1;
        
        int size = s.size();
        for(;cur < size;++cur){
            int left = cur -1;
            int right = cur + 1;
            //可能是奇数回文串
            while(left > -1 && right <size){
                if(s[left] == s[right]){
                    if(str.size() < right-left +1){
                       str = s.substr(left,right-left+1);
                    }
                    --left;
                    ++right;
                }else{
                    break;
                }
            }
            //可能是偶数回文串
            left = cur -1;
            right = cur +1;
            if(s[left] == s[cur]){
                if(str.size() < cur-left +1){
                    str = s.substr(left,cur-left+1);
                }
                --left;
            }
            while(left > -1 && right <size){
                if(s[left] == s[right]){
                    if(str.size() < right-left +1){
                        str = s.substr(left,right-left+1);
                    }
                    --left;
                    ++right;
                }else{
                    break;
                }
                
            }
        }
        return str;
    }
};
/*
	执行用时：36 ms, 在所有 C++ 提交中击败了78.55% 的用户
	内存消耗：20.8 MB, 在所有 C++ 提交中击败了59.22% 的用户
*/
/*
	改进：从返回的结果出发，由于使用了多次s.substr()调用，并拷贝给str导致消耗过多内存和时间，
	结果在运行过程中可以用其他变量表示，比如回文字串可以使用首字母索引　+　长度　表示，从而
	在中间计算过程中可以节省时间空间，使用length变量也不用再调用str.size();
	
	*尽量用基本数据类型表示变量．
	
	执行用时：20 ms, 在所有 C++ 提交中击败了90.85% 的用户
	内存消耗：7 MB, 在所有 C++ 提交中击败了94.48% 的用户	
*/
	
