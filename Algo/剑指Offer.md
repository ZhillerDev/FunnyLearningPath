### 字符串

#### 替换空格

> https://leetcode.cn/problems/ti-huan-kong-ge-lcof/solutions/94608/mian-shi-ti-05-ti-huan-kong-ge-ji-jian-qing-xi-tu-/

foreach 遍历原字符串，遇到空格就将其替换为 `%20`并插入结果字符串，如果不是空格就插入原字符

```cpp
class Solution {
public:
    string replaceSpace(string s) {
        string result;
        for(char c:s){
            if(c==' '){
                result+="%20";
            }else{
                result+=c;
            }
        }
        return result;
    }
};
```

<br>

在原字符串上操作，使用双指针法，不申请额外的内存空间

全字符串遍历后获得对应空格数目，然后双指针从后往前依次插入，遇到空格就插入 `%20`

```cpp
void replaceSpaces(string& s) {
    int len = s.length();   // 获取字符串s的长度
    int count = 0;          // 记录空格的个数
    for (int i = 0; i < len; i++) {
        if (s[i] == ' ') {  // 如果遇到空格，count加1
            count++;
        }
    }
    s.resize(len + 2 * count);   // 扩展字符串s的长度
    int p1 = len - 1;   // p1指向原字符串s的末尾
    int p2 = s.length() - 1;    // p2指向新字符串的末尾
    while (p1 >= 0 && p1 < p2) {
        if (s[p1] == ' ') {     // 如果遇到空格，将"%20"插入到p2处
            s[p2--] = '0';
            s[p2--] = '2';
            s[p2--] = '%';
        } else {    // 如果不是空格，直接复制字符到p2处
            s[p2--] = s[p1];
        }
        p1--;   // p1向前移动
    }
}
```

<br>

#### 左旋字符串

> https://leetcode.cn/problems/zuo-xuan-zhuan-zi-fu-chuan-lcof/?envType=study-plan-v2&id=coding-interviews

基于库函数，使用 substr 实现

```cpp
string leftRotateString(string s, int n) {
    if (n >= s.length()) {  // 如果n大于等于字符串s的长度，则不需要旋转
        return s;
    }
    string left = s.substr(0, n);   // 取出前n个字符
    string right = s.substr(n);     // 取出剩余的字符
    return right + left;    // 返回右半部分和左半部分拼接而成的字符串
}
```

<br>

非库函数，手写翻转函数（swap 可以不用自己实现浪费时间）

```cpp
void reverse(string& s, int start, int end) {
    // 将字符串s中从start到end的字符翻转
    while (start < end) {
        swap(s[start], s[end]);
        start++;
        end--;
    }
}

string leftRotateString(string s, int n) {
    int len = s.length();
    if (n >= len) {  // 如果n大于等于字符串s的长度，则不需要旋转
        return s;
    }
    reverse(s, 0, n - 1);       // 翻转前n个字符
    reverse(s, n, len - 1);     // 翻转剩余的字符
    reverse(s, 0, len - 1);     // 翻转整个字符串
    return s;
}
```

<br>

#### 表示数值的字符串

> https://leetcode.cn/problems/biao-shi-shu-zhi-de-zi-fu-chuan-lcof/description/?envType=study-plan-v2&id=coding-interviews

GPT 给出的，使用 Cpp 自带正则表达式，仅需两行即可完成算法

此算法在 LeetCode 判题错误，可能是某些问题导致的，限于作者技术原因，无法给出对应的修改手段

```cpp
#include <regex>

bool isNumber(std::string s) {
    std::regex pattern(" *[-+]?(([0-9]+(\\.[0-9]*)?)|(\\.[0-9]+))(e[-+]?[0-9]+)? *");
    return std::regex_match(s, pattern);
}
```

<br>

常规，遍历后取标志位仅需替换

```cpp
bool isNumber(string s) {
    // 先去掉左右两边的空格
    int n = s.length();
    int left = 0, right = n - 1;
    while (left <= right && isspace(s[left])) {
        left++;
    }
    while (left <= right && isspace(s[right])) {
        right--;
    }
    if (left > right) {  // 去掉空格后，如果字符串为空，则不是数值
        return false;
    }
    // 判断符号位
    if (s[left] == '+' || s[left] == '-') {
        left++;
    }
    bool is_number = false, is_exp = false, is_point = false;
    while (left <= right) {
        char ch = s[left];
        if (isdigit(ch)) {  // 如果当前字符是数字
            is_number = true;
        } else if (ch == '.') {  // 如果当前字符是小数点
            if (is_point || is_exp) {  // 如果之前已经出现过小数点或者指数，则不是数值
                return false;
            }
            is_point = true;
        } else if (ch == 'e' || ch == 'E') {  // 如果当前字符是指数符号
            if (is_exp || !is_number) {  // 如果之前已经出现过指数或者之前没有出现过数字，则不是数值
                return false;
            }
            is_exp = true;
            is_number = false;  // 重置is_number，用于判断指数的部分是否为整数
        } else if (ch == '+' || ch == '-') {  // 如果当前字符是符号位
            if (s[left - 1] != 'e' && s[left - 1] != 'E') {  // 如果符号位不在指数的前面，则不是数值
                return false;
            }
        } else {  // 如果当前字符不是数字、小数点、指数符号、或者符号位，则不是数值
            return false;
        }
        left++;
    }
    return is_number;
}
```

<br>

#### 把字符串转换成整数

> https://leetcode.cn/problems/ba-zi-fu-chuan-zhuan-huan-cheng-zheng-shu-lcof/description/?envType=study-plan-v2&id=coding-interviews

此题需要注意三个关键点：符号位正负判断、数值累加是否越界、无关字符忽略

```cpp
int StrToInt(string str) {
    int res = 0;  // 保存转换结果
    int sign = 1;  // 符号标志，默认为正号
    int i = 0;  // 遍历字符串
    int n = str.size();

    // 处理空白字符
    while (i < n && str[i] == ' ') i++;

    // 处理符号
    if (i < n && (str[i] == '+' || str[i] == '-')) {
        sign = (str[i] == '-') ? -1 : 1;
        i++;
    }

    // 处理数字部分
    while (i < n && isdigit(str[i])) {
        int digit = str[i] - '0';

        // 判断是否溢出
        if (res > INT_MAX / 10 || (res == INT_MAX / 10 && digit > INT_MAX % 10)) {
            return (sign == 1) ? INT_MAX : INT_MIN;
        }

        // 逐位累加数值
        res = res * 10 + digit;
        i++;
    }

    // 最后输出时才乘以符号位变号
    return sign * res;
}

```

<br>

### 链表

#### 从头到尾打印链表

> https://leetcode.cn/problems/cong-wei-dao-tou-da-yin-lian-biao-lcof/description/?envType=study-plan-v2&id=coding-interviews

得益于栈后进先出的特性，我们可以使用栈来实现这一操作

```cpp
#include <vector>
#include <stack>
using namespace std;

// Definition for singly-linked list.
struct ListNode {
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(NULL) {}
};

class Solution {
public:
    vector<int> reversePrint(ListNode* head) {
        // 创建一个栈用于存放链表节点值
        stack<int> s;
        // 遍历链表，将节点值依次入栈
        while (head) {
            s.push(head->val);
            head = head->next;
        }
        // 创建一个vector，将栈中元素依次弹出并添加到vector中
        vector<int> res;
        while (!s.empty()) {
            res.push_back(s.top());
            s.pop();
        }
        return res;
    }
};

```

<br>

#### 翻转链表

```cpp
/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode(int x) : val(x), next(NULL) {}
 * };
 */
class Solution {
public:
    ListNode* reverseList(ListNode* head) {
        // 如果头结点为空或者只有一个节点，则直接返回头结点
        if (head == nullptr || head->next == nullptr) {
            return head;
        }
        // 定义两个指针，分别指向前一个节点和当前节点
        ListNode *pre = nullptr;
        ListNode *cur = head;
        // 定义一个临时指针，用于记录当前节点的下一个节点
        ListNode *temp = nullptr;
        // 遍历链表，将当前节点指向前一个节点
        while (cur != nullptr) {
            temp = cur->next;  // 记录当前节点的下一个节点
            cur->next = pre;   // 将当前节点指向前一个节点
            pre = cur;         // 将前一个节点指针指向当前节点
            cur = temp;        // 将当前节点指针指向下一个节点
        }
        return pre;            // 返回新的头结点
    }
};

```

<br>

#### 复杂链表的绘制

> https://leetcode.cn/problems/fu-za-lian-biao-de-fu-zhi-lcof/description/?envType=study-plan-v2&id=coding-interviews

```cpp
#include <unordered_map>

using namespace std;

class Node {
public:
    int val;
    Node* next;
    Node* random;

    Node(int _val) {
        val = _val;
        next = NULL;
        random = NULL;
    }
};

class Solution {
public:
    Node* copyRandomList(Node* head) {
        // 头结点为空代表不存在，直接返回空
        if (head == nullptr) {
            return nullptr;
        }

        // 使用unordered_map存储新旧节点的对应关系
        unordered_map<Node*, Node*> nodeMap;
        Node* cur = head;

        // 第一遍循环，将旧链表的节点作为key，新链表的节点作为value存入nodeMap中
        while (cur != nullptr) {
            nodeMap[cur] = new Node(cur->val);
            cur = cur->next;
        }
        cur = head;

        // 第二遍循环，根据nodeMap中存储的对应关系，连接新链表的next和random指针
        while (cur != nullptr) {
            nodeMap[cur]->next = nodeMap[cur->next];
            nodeMap[cur]->random = nodeMap[cur->random];
            cur = cur->next;
        }
        return nodeMap[head];
    }
};

```

<br>

### 双指针

#### 删除链表的节点

> https://leetcode.cn/problems/shan-chu-lian-biao-de-jie-dian-lcof/?envType=study-plan-v2&id=coding-interviews

双指针，可以保证取到的节点都不越界

```cpp
ListNode* deleteNode(ListNode* head, int val) {
    if (head == nullptr) return nullptr;  // 特判，链表为空

    if (head->val == val) return head->next;  // 特判，头节点就是要删除的节点

    ListNode* pre = head;
    ListNode* cur = head->next;
    while (cur != nullptr && cur->val != val) {  // 找到要删除的节点
        pre = cur;
        cur = cur->next;
    }

    if (cur != nullptr) pre->next = cur->next;  // 找到了要删除的节点，将前一个节点指向后一个节点

    return head;  // 返回链表的头节点
}
```

<br>

#### 链表中倒数第 K 个节点

> https://leetcode.cn/problems/lian-biao-zhong-dao-shu-di-kge-jie-dian-lcof/?envType=study-plan-v2&id=coding-interviews

首先将两个指针都指向头节点，然后将其中一个指针先向前移动 k-1 步，接着两个指针同时向前移动，直到第二个指针到达链表末尾，此时第一个指针所指的节点就是倒数第 k 个节点

```cpp
/*
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode(int x) : val(x), next(NULL) {}
 * };
 */
class Solution {
public:
    ListNode* getKthFromEnd(ListNode* head, int k) {
        ListNode *slow = head, *fast = head;
        for(int i = 0; i < k; i++) {
            fast = fast->next;
        }
        while(fast != nullptr) {
            slow = slow->next;
            fast = fast->next;
        }
        return slow;
    }
};

```

<br>

#### 合并两个排序的链表

> https://leetcode.cn/problems/he-bing-liang-ge-pai-xu-de-lian-biao-lcof/description/?envType=study-plan-v2&id=coding-interviews

一个比较复杂但是行数很少的递归形式题解

每次比较两个链表的头结点，将较小的那个接到结果链表的末尾。再将剩余的链表继续递归调用函数进行合并。当其中一个链表为空时，将另一个链表剩余的部分接到结果链表末尾。最后返回结果链表头结点即可

```cpp
ListNode* mergeTwoLists(ListNode* l1, ListNode* l2) {

    // 因为两个链表不一定等长，如果一个遍历完毕，则另一个直接一一接上去即可
    if (!l1) return l2;
    if (!l2) return l1;

    if (l1->val <= l2->val) {
        l1->next = mergeTwoLists(l1->next, l2);
        return l1;
    } else {
        l2->next = mergeTwoLists(l1, l2->next);
        return l2;
    }
}
```

<br>

#### 两个链表的第一个公共节点

> https://leetcode.cn/problems/liang-ge-lian-biao-de-di-yi-ge-gong-gong-jie-dian-lcof/?envType=study-plan-v2&id=coding-interviews

假设两个链表分别为 A 和 B，且 A 的长度为 a，B 的长度为 b。当两个链表的长度相等时，两个指针同时遍历两个链表，直到找到第一个相同的节点。当两个链表长度不等时，长链表的指针先遍历 $|a-b|$ 个节点，然后两个指针同时遍历链表，直到找到第一个相同的节点

```cpp
/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode(int x) : val(x), next(NULL) {}
 * };
 */
class Solution {
public:
    ListNode *getIntersectionNode(ListNode *headA, ListNode *headB) {
        ListNode *p1 = headA, *p2 = headB;
        while (p1 != p2) {
            p1 = p1 == nullptr ? headB : p1->next;
            p2 = p2 == nullptr ? headA : p2->next;
        }
        return p1;
    }
};
```

<br>

#### 调整数组顺序使奇数位于偶数前面

> https://leetcode.cn/problems/diao-zheng-shu-zu-shun-xu-shi-qi-shu-wei-yu-ou-shu-qian-mian-lcof/?envType=study-plan-v2&id=coding-interviews

基本过程如下

1. 定义两个指针 i 和 j，分别指向数组的首尾两端。
2. 当 `i<j` 时，执行以下操作：
   如果 `a[i]`是偶数，`a[j]`是奇数，则交换 `a[i]`和 `a[j]`的值；
   如果 `a[i]`是奇数，则 i 自增 1；
   如果 `a[j]`是偶数，则 j 自减 1。
3. 当 i>=j 时，排序完成。

```cpp
#include <iostream>
#include <vector>

using namespace std;

void sortArrayByParity(vector<int>& nums) {
    int i = 0, j = nums.size() - 1;
    while (i < j) {
        if (nums[i] % 2 == 0 && nums[j] % 2 == 1) {
            swap(nums[i], nums[j]);
        }
        if (nums[i] % 2 == 1) {
            i++;
        }
        if (nums[j] % 2 == 0) {
            j--;
        }
    }

    return nums;
}
```

<br>

#### 和为 s 的两个数字

> https://leetcode.cn/problems/he-wei-sde-liang-ge-shu-zi-lcof/description/?envType=study-plan-v2&id=coding-interviews

根据此思路组织代码

1. 定义两个指针 i 和 j，分别指向数组的首尾两端。
2. 当 `i<j` 时，执行以下操作：
   如果 `a[i]+a[j]`等于 s，则返回 `a[i]`和 `a[j]`的值；
   如果 `a[i]+a[j]`小于 s，则 i 自增 1；
   如果 `a[i]+a[j]`大于 s，则 j 自减 1。
3. 当 `i>=j` 时，数组中不存在两个数的和等于 s。

```cpp
#include <iostream>
#include <vector>

using namespace std;

vector<int> findNumbersWithSum(vector<int>& nums, int s) {
    int i = 0, j = nums.size() - 1;
    while (i < j) {
        int sum = nums[i] + nums[j];
        if (sum == s) {
            return {nums[i], nums[j]};
        }
        if (sum < s) {
            i++;
        }
        if (sum > s) {
            j--;
        }
    }
    return {};
}
```

<br>

#### 翻转单词顺序

> https://leetcode.cn/problems/fan-zhuan-dan-ci-shun-xu-lcof/?envType=study-plan-v2&id=coding-interviews

双指针解决问题

```cpp
class Solution {
public:
    string reverseWords(string s) {
        string res;
        int n = s.size();
        if(n == 0) return res;
        int right = n - 1;
        while(right >= 0){
            //从后往前寻找第一字符
            while(right >= 0 && s[right] == ' ') right--;
            if(right < 0) break;

            //从后往前寻找第一个空格
            int left = right;
            while( left >= 0 && s[left] != ' ' ) left--;

            //添加单词到结果
            res += s.substr(left + 1, right - left);
            res += ' ';

            //继续往前分割单词
            right = left;
        }
        //去除最后一个字符空格
        if (!res.empty()) res.pop_back();
        return res;
    }
};
```

<br>

### 栈与队列
