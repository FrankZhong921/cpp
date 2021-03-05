/*
	给你一个整数 x ，如果 x 是一个回文整数，返回 true ；否则，返回 false 。
	回文数是指正序（从左向右）和倒序（从右向左）读都是一样的整数。例如，121 是回文，而 123 不是。


*/


class Solution {
public:
    bool isPalindrome(int x) {
        stringstream ss;
        ss << x;
        string s;
        ss >> s;
        if(s.size()==1) return true;
        int i= 0,j=s.size()-1;			//不能使用size_t无符号数，否则如果输入为0，string也为"0",++i,--j后会使原本预想的j=-1,i=i变成j=118446744073709551615;
        for(; i< s.size() && i<=j; ++i,--j){	//比如在32位机上，定义为unsigned int，也就是说32位系统上是32位无符号整形数。在64位机上是64位的无符号整形。
            if(s[i]!=s[j]){

            
            break;
            }
        }
        if(i>=j){
            return true;
        }
        return false;
    }
};


