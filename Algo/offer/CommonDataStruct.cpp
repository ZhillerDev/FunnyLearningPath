#include <bits/stdc++.h>

using namespace std;

int main()
{
    return 0;
}

class NormalNode
{
public:
    int val;
    NormalNode *next;
    NormalNode(int _val)
    {
        val = _val;
        next = NULL;
    }
};

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
