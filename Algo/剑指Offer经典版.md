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
