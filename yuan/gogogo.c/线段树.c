#include <stdio.h>
#include <stdlib.h>

#define MAX_N 100005
#define MAX_TREE (4 * MAX_N)

// 定义节点结构
typedef struct {
    long long sum;  // 区间和
    long long lazy; // 懒标记：表示该区间每个元素待加的值
} Node;

Node tree[MAX_TREE];
int arr[MAX_N];
int n;

// 向上更新：用孩子更新父亲
void push_up(int node) {
    tree[node].sum = tree[node * 2].sum + tree[node * 2 + 1].sum;
}

// 向下传递懒标记
// node: 当前节点
// start, end: 当前节点管理的区间范围
void push_down(int node, int start, int end) {
    if (tree[node].lazy != 0) {
        int mid = (start + end) / 2;
        int left = node * 2;
        int right = node * 2 + 1;

        // 更新左孩子
        // 和增加量 = 懒标记值 * 左孩子区间长度
        tree[left].sum += tree[node].lazy * (mid - start + 1);
        tree[left].lazy += tree[node].lazy; // 标记累加

        // 更新右孩子
        tree[right].sum += tree[node].lazy * (end - mid);
        tree[right].lazy += tree[node].lazy;

        // 清空当前节点的标记
        tree[node].lazy = 0;
    }
}

// 建树
void build(int node, int start, int end) {
    tree[node].lazy = 0; // 初始化标记
    if (start == end) {
        tree[node].sum = arr[start];
        return;
    }
    int mid = (start + end) / 2;
    build(node * 2, start, mid);
    build(node * 2 + 1, mid + 1, end);
    push_up(node);
}

// 区间更新：[L, R] 范围内的数都加上 val
void update_range(int node, int start, int end, int L, int R, int val) {
    // 情况1: 完全包含 -> 打标记，更新和，直接返回
    if (L <= start && end <= R) {
        tree[node].sum += (long long)val * (end - start + 1);
        tree[node].lazy += val;
        return;
    }

    // 情况2: 部分重叠 -> 先下传标记，再递归
    push_down(node, start, end);

    int mid = (start + end) / 2;
    if (L <= mid) {
        update_range(node * 2, start, mid, L, R, val);
    }
    if (R > mid) {
        update_range(node * 2 + 1, mid + 1, end, L, R, val);
    }

    // 回溯更新
    push_up(node);
}

// 区间查询
long long query(int node, int start, int end, int L, int R) {
    // 情况1: 完全包含 -> 直接返回
    if (L <= start && end <= R) {
        return tree[node].sum;
    }

    // 情况2: 部分重叠 -> 先下传标记
    push_down(node, start, end);

    int mid = (start + end) / 2;
    long long res = 0;
    if (L <= mid) {
        res += query(node * 2, start, mid, L, R);
    }
    if (R > mid) {
        res += query(node * 2 + 1, mid + 1, end, L, R);
    }
    return res;
}

int main() {
    n = 5;
    // 初始化数组: 1, 2, 3, 4, 5
    for (int i = 0; i < n; i++) {
        arr[i] = i + 1;
    }

    printf("Original Array: ");
    for (int i = 0; i < n; i++) printf("%d ", arr[i]);
    printf("\n");

    // 1. 建树
    build(1, 0, n - 1);

    // 2. 初始查询
    printf("Sum [0, 4]: %lld\n", query(1, 0, n - 1, 0, 4)); // 1+2+3+4+5 = 15

    // 3. 区间更新: 把 [1, 3] (即 2,3,4) 全部加 10
    // 预期数组变为: 1, 12, 13, 14, 5
    printf("Updating range [1, 3] += 10...\n");
    update_range(1, 0, n - 1, 1, 3, 10);

    // 4. 再次查询
    printf("Sum [0, 4] after update: %lld\n", query(1, 0, n - 1, 0, 4)); // 1+12+13+14+5 = 45
    printf("Sum [1, 3] after update: %lld\n", query(1, 0, n - 1, 1, 3)); // 12+13+14 = 39
    printf("Sum [0, 0] after update: %lld\n", query(1, 0, n - 1, 0, 0)); // 1 (未变)
    printf("Sum [4, 4] after update: %lld\n", query(1, 0, n - 1, 4, 4)); // 5 (未变)

    return 0;
}