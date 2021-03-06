/*
	给你 n 个非负整数 a1，a2，...，an，每个数代表坐标中的一个点 (i, ai) 。在坐标内画 n 条垂直线，垂直线 i 的两个端点分别为 (i, ai) 和 (i, 0) 。找出其中的两条线，使得它们与 x 轴共同构成的容器可以容纳最多的水。
	
	输入：[1,8,6,2,5,4,8,3,7]
	输出：49 
	解释：图中垂直线代表输入数组 [1,8,6,2,5,4,8,3,7]。在此情况下，容器能够容纳水（表示为蓝色部分）的最大值为 49



*/
class Solution {
public:
    
    int maxArea(vector<int>& height) {
        
        int i = 0,j = height.size()-1;
        int constrains = (height[i]< height[j])? i:j;
        int contain = (j-i)*height[constrains];
        
        while(i < j){
            
            if(height[i]<=height[j]){
                ++i;
            }
            else{
                --j;
            }
            constrains = (height[i]< height[j])? i:j;
            if(contain < (j-i) * height[constrains]){
                contain = (j-i) * height[constrains];
            }
            
        }
        return contain;
    }
};

/*
	执行用时：104 ms, 在所有 C++ 提交中击败了5.02% 的用户
	内存消耗：57.6 MB, 在所有 C++ 提交中击败了5.08% 的用户

*/
