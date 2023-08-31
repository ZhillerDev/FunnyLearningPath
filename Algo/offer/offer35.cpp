#include <bits/stdc++.h>

using namespace std;

class RandomNode
{
public:
    int val;
    RandomNode *next;
    RandomNode *random;
    RandomNode(int _val)
    {
        val = _val;
        next = NULL;
        random = NULL;
    }
};

void main()
{
    return;
}

class Solution
{
public:
    RandomNode *copyNodeList(RandomNode *head)
    {
        // 如果原始链表为空，则返回空指针。
        if (head == nullptr)
            return nullptr;

        // 创建指向原始链表中当前节点的指针。
        RandomNode *cur = head;

        // 创建一个无序映射，用于存储原始链表中节点和其对应的复制链表中节点之间的映射关系。
        unordered_map<RandomNode *, RandomNode *> map;

        // 遍历原始链表，为链表中的每个节点创建一个新节点，并在映射中添加原始节点和复制节点之间的映射关系。
        while (cur != nullptr)
        {
            map[cur] = new RandomNode(cur->val);
            cur = cur->next;
        }

        // 重置当前指针为原始链表的头部。
        cur = head;

        // 再次遍历原始链表，并根据映射中的关系为每个复制节点设置next和random指针。
        while (cur != nullptr)
        {
            map[cur]->next = map[cur->next];
            map[cur]->random = map[cur->random];
            cur = cur->next;
        }

        // 返回复制链表的头部。
        return map[head];
    }
};