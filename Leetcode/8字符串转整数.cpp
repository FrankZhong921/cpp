/*
	请你来实现一个 myAtoi(string s) 函数，使其能将字符串转换成一个 32 位有符号整数（类似 C/C++ 中的 atoi 函数）。
函数 myAtoi(string s) 的算法如下：

    读入字符串并丢弃无用的前导空格
    检查下一个字符（假设还未到字符末尾）为正还是负号，读取该字符（如果有）。 确定最终结果是负数还是正数。 如果两者都不存在，则假定结果为正。
    读入下一个字符，直到到达下一个非数字字符或到达输入的结尾。字符串的其余部分将被忽略。
    将前面步骤读入的这些数字转换为整数（即，"123" -> 123， "0032" -> 32）。如果没有读入数字，则整数为 0 。必要时更改符号（从步骤 2 开始）。
    如果整数数超过 32 位有符号整数范围 [−231,  231 − 1] ，需要截断这个整数，使其保持在这个范围内。具体来说，小于 −231 的整数应该被固定为 −231 ，大于 231 − 1 的整数应该被固定为 231 − 1 。
    返回整数作为最终结果。



*/

class Solution {
public:
    int myAtoi(string s) {
        int result = 0;
        stringstream ss;
        ss << s;
        ss >> result;
        return result;
    }
};

/*
执行用时：4 ms, 在所有 C++ 提交中击败了73.82% 的用户
内存消耗：7.5 MB, 在所有 C++ 提交中击败了16.03% 的用户

*/


class Solution {
public:
    int myAtoi(string s) {
        int i = 0, n = s.size(), res = 0;
        while (s[i] == ' ') i ++;
        bool isNegative = false;
        if (i < s.size()){
            if (s[i] == '-') isNegative = true, i ++;
            else if (s[i] == '+') i ++;
        } 
        while (i < s.size() && isdigit(s[i])){
            int x = s[i] - '0';
            if (isNegative) x = -x;
            //答案溢出范围时正、负的两种情况，直接返回答案
            if (res > INT_MAX / 10 || (res == INT_MAX / 10 && x > 7)) return INT_MAX;
            if (res < INT_MIN / 10 || (res == INT_MIN / 10 && x < -8)) return INT_MIN;
            res = res * 10 + x, i ++;
        }
        return res;
    }
};

/*
执行用时：0 ms, 在所有 C++ 提交中击败了 100% 的用户
内存消耗：6.9 MB, 在所有 C++ 提交中击败了 84.60% 的用户

*/
