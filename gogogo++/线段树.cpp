#include <iostream>
#include <vector>

using namespace std;

class SegmentTreeLazy {
private:
    struct Node {
        long long sum;
        long long lazy;
        Node() : sum(0), lazy(0) {}
    };

    vector<Node> tree;
    int n;

    // 向上更新
    void push_up(int node) {
        tree[node].sum = tree[node * 2].sum + tree[node * 2 + 1].sum;
    }

    // 向下传递懒标记
    void push_down(int node, int start, int end) {
        if (tree[node].lazy != 0) {
            int mid = (start + end) / 2;
            int left = node * 2;
            int right = node * 2 + 1;

            // 更新左孩子
            tree[left].sum += tree[node].lazy * (mid - start + 1);
            tree[left].lazy += tree[node].lazy;

            // 更新右孩子
            tree[right].sum += tree[node].lazy * (end - mid);
            tree[right].lazy += tree[node].lazy;

            // 清空当前
            tree[node].lazy = 0;
        }
    }

    void build(const vector<int>& arr, int node, int start, int end) {
        tree[node].lazy = 0;
        if (start == end) {
            tree[node].sum = arr[start];
            return;
        }
        int mid = (start + end) / 2;
        build(arr, node * 2, start, mid);
        build(arr, node * 2 + 1, mid + 1, end);
        push_up(node);
    }

    void update_range(int node, int start, int end, int L, int R, int val) {
        if (L <= start && end <= R) {
            tree[node].sum += (long long)val * (end - start + 1);
            tree[node].lazy += val;
            return;
        }

        push_down(node, start, end); // 关键：访问孩子前先下传

        int mid = (start + end) / 2;
        if (L <= mid) update_range(node * 2, start, mid, L, R, val);
        if (R > mid) update_range(node * 2 + 1, mid + 1, end, L, R, val);

        push_up(node);
    }

    long long query(int node, int start, int end, int L, int R) {
        if (L <= start && end <= R) {
            return tree[node].sum;
        }

        push_down(node, start, end); // 关键：访问孩子前先下传

        int mid = (start + end) / 2;
        long long res = 0;
        if (L <= mid) res += query(node * 2, start, mid, L, R);
        if (R > mid) res += query(node * 2 + 1, mid + 1, end, L, R);
        return res;
    }

public:
    SegmentTreeLazy(const vector<int>& arr) {
        n = arr.size();
        tree.resize(4 * n);
        build(arr, 1, 0, n - 1);
    }

    void update(int L, int R, int val) {
        if (L < 0 || R >= n || L > R) return;
        update_range(1, 0, n - 1, L, R, val);
    }

    long long query(int L, int R) {
        if (L < 0 || R >= n || L > R) return 0;
        return query(1, 0, n - 1, L, R);
    }
};

int main_long() {
    vector<int> arr = { 1, 2, 3, 4, 5 };
    int n = arr.size();

    cout << "Original Array: ";
    for (int x : arr) cout << x << " ";
    cout << endl;

    SegmentTreeLazy st(arr);

    cout << "Sum [0, 4]: " << st.query(0, 4) << endl; // 15

    cout << "Updating range [1, 3] += 10..." << endl;
    st.update(1, 3, 10);

    cout << "Sum [0, 4] after update: " << st.query(0, 4) << endl; // 45
    cout << "Sum [1, 3] after update: " << st.query(1, 3) << endl; // 39
    cout << "Sum [0, 0] after update: " << st.query(0, 0) << endl; // 1
    cout << "Sum [4, 4] after update: " << st.query(4, 4) << endl; // 5

    return 0;
}