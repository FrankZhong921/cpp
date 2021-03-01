/*　２ 两数相加
给你两个 非空 的链表，表示两个非负的整数。它们每位数字都是按照 逆序 的方式存储的，并且每个节点只能存储 一位 数字。

请你将两个数相加，并以相同形式返回一个表示和的链表。

你可以假设除了数字 0 之外，这两个数都不会以 0 开头。

输入：l1 = [9,9,9,9,9,9,9], l2 = [9,9,9,9]
输出：[8,9,9,9,0,0,0,1]

输入：l1 = [2,4,3], l2 = [5,6,4]
输出：[7,0,8]
解释：342 + 465 = 807.

*/
/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode() : val(0), next(nullptr) {}
 *     ListNode(int x) : val(x), next(nullptr) {}
 *     ListNode(int x, ListNode *next) : val(x), next(next) {}
 * };
 */
　	
/*
执行用时：48 ms, 在所有 C++ 提交中击败了53.23% 的用户
内存消耗：69.3 MB, 在所有 C++ 提交中击败了95.56% 的用户
*/
//改进：　设置标志位记录是否进位
class Solution {
public:
    
    
    ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
        ListNode* result = new ListNode(0);
        ListNode* ptr = result;
        int sum = 0;
        bool yn = false;
        while(l1|| l2){
            sum = 0;
            if(l1){
                sum += l1->val;
                l1 = l1->next;
            }
            if(l2){
                sum += l2->val;
                l2 = l2->next;
            }
            if(yn){
                ++sum ;
            }
            yn = sum >= 10? true:false;
            ptr->val = sum % 10;
            if(l1 != nullptr || l2 != nullptr || yn){
                ptr->next = new ListNode(1);
                ptr = ptr->next;
            }
        }
        return result;
    }
};
/*
执行用时：36 ms, 在所有 C++ 提交中击败了86.46% 的用户
内存消耗：69.4 MB, 在所有 C++ 提交中击败了89.22% 的用户
*/
