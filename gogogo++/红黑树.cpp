#include <iostream>
#include <queue>
#include <memory>
#include <iomanip>

enum class Color { RED, BLACK };

template<typename T>
class RedBlackTree {
private:
    struct Node {
        T data;
        Color color;
        Node* left;
        Node* right;
        Node* parent;

        Node(const T& val) : data(val), color(Color::RED),
            left(nullptr), right(nullptr), parent(nullptr) {
        }
    };

    Node* root;
    Node* nil;  // 哨兵节点，代表空节点

    // 左旋
    void rotateLeft(Node* x) {
        Node* y = x->right;
        x->right = y->left;
        if (y->left != nil) y->left->parent = x;
        y->parent = x->parent;
        if (x->parent == nil) root = y;
        else if (x == x->parent->left) x->parent->left = y;
        else x->parent->right = y;
        y->left = x;
        x->parent = y;
    }

    // 右旋
    void rotateRight(Node* x) {
        Node* y = x->left;
        x->left = y->right;
        if (y->right != nil) y->right->parent = x;
        y->parent = x->parent;
        if (x->parent == nil) root = y;
        else if (x == x->parent->right) x->parent->right = y;
        else x->parent->left = y;
        y->right = x;
        x->parent = y;
    }

    // 插入修复
    void insertFixup(Node* z) {
        while (z->parent->color == Color::RED) {
            if (z->parent == z->parent->parent->left) {
                Node* y = z->parent->parent->right;  // 叔叔节点
                if (y->color == Color::RED) {  // 情况1：叔叔是红色
                    z->parent->color = Color::BLACK;
                    y->color = Color::BLACK;
                    z->parent->parent->color = Color::RED;
                    z = z->parent->parent;
                }
                else {
                    if (z == z->parent->right) {  // 情况2：z是右孩子，需要左旋
                        z = z->parent;
                        rotateLeft(z);
                    }
                    // 情况3：z是左孩子
                    z->parent->color = Color::BLACK;
                    z->parent->parent->color = Color::RED;
                    rotateRight(z->parent->parent);
                }
            }
            else {  // 对称情况
                Node* y = z->parent->parent->left;
                if (y->color == Color::RED) {
                    z->parent->color = Color::BLACK;
                    y->color = Color::BLACK;
                    z->parent->parent->color = Color::RED;
                    z = z->parent->parent;
                }
                else {
                    if (z == z->parent->left) {
                        z = z->parent;
                        rotateRight(z);
                    }
                    z->parent->color = Color::BLACK;
                    z->parent->parent->color = Color::RED;
                    rotateLeft(z->parent->parent);
                }
            }
        }
        root->color = Color::BLACK;
    }

    // 插入节点
    void insertNode(Node* z) {
        Node* y = nil;
        Node* x = root;

        while (x != nil) {
            y = x;
            if (z->data < x->data) x = x->left;
            else x = x->right;
        }

        z->parent = y;
        if (y == nil) root = z;
        else if (z->data < y->data) y->left = z;
        else y->right = z;

        z->left = nil;
        z->right = nil;
        z->color = Color::RED;

        insertFixup(z);
    }

    // 中序遍历
    void inorderHelper(Node* node) const {
        if (node != nil) {
            inorderHelper(node->left);
            std::cout << node->data << "("
                << (node->color == Color::RED ? "R" : "B") << ") ";
            inorderHelper(node->right);
        }
    }

    // 查找节点
    Node* searchNode(const T& val) const {
        Node* current = root;
        while (current != nil) {
            if (val == current->data) return current;
            else if (val < current->data) current = current->left;
            else current = current->right;
        }
        return nil;
    }

    // 获取最小值节点
    Node* minimum(Node* node) const {
        while (node->left != nil) node = node->left;
        return node;
    }

    // 替换子树
    void transplant(Node* u, Node* v) {
        if (u->parent == nil) root = v;
        else if (u == u->parent->left) u->parent->left = v;
        else u->parent->right = v;
        v->parent = u->parent;
    }

    // 删除修复
    void deleteFixup(Node* x) {
        while (x != root && x->color == Color::BLACK) {
            if (x == x->parent->left) {
                Node* w = x->parent->right;  // 兄弟节点
                if (w->color == Color::RED) {  // 情况1：兄弟是红色
                    w->color = Color::BLACK;
                    x->parent->color = Color::RED;
                    rotateLeft(x->parent);
                    w = x->parent->right;
                }
                if (w->left->color == Color::BLACK && w->right->color == Color::BLACK) {  // 情况2
                    w->color = Color::RED;
                    x = x->parent;
                }
                else {
                    if (w->right->color == Color::BLACK) {  // 情况3
                        w->left->color = Color::BLACK;
                        w->color = Color::RED;
                        rotateRight(w);
                        w = x->parent->right;
                    }
                    // 情况4
                    w->color = x->parent->color;
                    x->parent->color = Color::BLACK;
                    w->right->color = Color::BLACK;
                    rotateLeft(x->parent);
                    x = root;
                }
            }
            else {  // 对称情况
                Node* w = x->parent->left;
                if (w->color == Color::RED) {
                    w->color = Color::BLACK;
                    x->parent->color = Color::RED;
                    rotateRight(x->parent);
                    w = x->parent->left;
                }
                if (w->right->color == Color::BLACK && w->left->color == Color::BLACK) {
                    w->color = Color::RED;
                    x = x->parent;
                }
                else {
                    if (w->left->color == Color::BLACK) {
                        w->right->color = Color::BLACK;
                        w->color = Color::RED;
                        rotateLeft(w);
                        w = x->parent->left;
                    }
                    w->color = x->parent->color;
                    x->parent->color = Color::BLACK;
                    w->left->color = Color::BLACK;
                    rotateRight(x->parent);
                    x = root;
                }
            }
        }
        x->color = Color::BLACK;
    }

    // 删除节点
    void deleteNode(Node* z) {
        if (z == nil) return;

        Node* y = z;
        Node* x;
        Color y_original_color = y->color;

        if (z->left == nil) {  // 只有右孩子或无孩子
            x = z->right;
            transplant(z, z->right);
        }
        else if (z->right == nil) {  // 只有左孩子
            x = z->left;
            transplant(z, z->left);
        }
        else {  // 有两个孩子
            y = minimum(z->right);  // 找后继
            y_original_color = y->color;
            x = y->right;

            if (y->parent == z) {
                x->parent = y;
            }
            else {
                transplant(y, y->right);
                y->right = z->right;
                y->right->parent = y;
            }

            transplant(z, y);
            y->left = z->left;
            y->left->parent = y;
            y->color = z->color;
        }

        delete z;

        if (y_original_color == Color::BLACK) {
            deleteFixup(x);
        }
    }

    // 清理内存
    void clearHelper(Node* node) {
        if (node != nil) {
            clearHelper(node->left);
            clearHelper(node->right);
            delete node;
        }
    }

    // 打印树结构
    void printHelper(Node* root, std::string indent, bool last) const {
        if (root != nil) {
            std::cout << indent;
            if (last) {
                std::cout << "R----";
                indent += "   ";
            }
            else {
                std::cout << "L----";
                indent += "|  ";
            }

            std::cout << root->data << "("
                << (root->color == Color::RED ? "R" : "B") << ")\n";
            printHelper(root->left, indent, false);
            printHelper(root->right, indent, true);
        }
    }

public:
    RedBlackTree() {
        nil = new Node(T());
        nil->color = Color::BLACK;
        nil->left = nil->right = nil->parent = nil;
        root = nil;
    }

    ~RedBlackTree() {
        clearHelper(root);
        delete nil;
    }

    // 插入接口
    void insert(const T& val) {
        Node* z = new Node(val);
        z->left = z->right = z->parent = nil;
        insertNode(z);
    }

    // 删除接口
    void remove(const T& val) {
        Node* z = searchNode(val);
        if (z != nil) deleteNode(z);
    }

    // 查找接口
    bool search(const T& val) const {
        return searchNode(val) != nil;
    }

    // 中序遍历
    void inorder() const {
        inorderHelper(root);
        std::cout << std::endl;
    }

    // 打印树结构
    void print() const {
        if (root == nil) {
            std::cout << "树为空\n";
            return;
        }
        printHelper(root, "", true);
    }

    // 检查红黑树性质
    bool isValid() const {
        if (root->color != Color::BLACK) return false;  // 性质2

        // 检查连续红色节点和黑高
        int blackHeight = -1;
        return validateProperties(root, 0, blackHeight);
    }

private:
    bool validateProperties(Node* node, int currentBlackCount, int& expectedBlackCount) const {
        if (node == nil) {
            if (expectedBlackCount == -1) {
                expectedBlackCount = currentBlackCount;
            }
            else if (expectedBlackCount != currentBlackCount) {
                return false;  // 黑高不一致
            }
            return true;
        }

        if (node->color == Color::BLACK) currentBlackCount++;

        // 检查连续红色
        if (node->color == Color::RED) {
            if (node->left->color == Color::RED || node->right->color == Color::RED) {
                return false;
            }
        }

        return validateProperties(node->left, currentBlackCount, expectedBlackCount) &&
            validateProperties(node->right, currentBlackCount, expectedBlackCount);
    }
};

// 测试代码
int main() {
    RedBlackTree<int> rbt;

    std::cout << "=== 红黑树插入测试 ===\n";
    int insertValues[] = { 10, 20, 30, 15, 25, 5, 1 };

    for (int val : insertValues) {
        std::cout << "\n插入 " << val << ":\n";
        rbt.insert(val);
        rbt.print();
        std::cout << "中序遍历: ";
        rbt.inorder();
        std::cout << "是否有效: " << (rbt.isValid() ? "是" : "否") << "\n";
    }

    std::cout << "\n=== 红黑树删除测试 ===\n";
    int deleteValues[] = { 15, 20, 5 };

    for (int val : deleteValues) {
        std::cout << "\n删除 " << val << ":\n";
        rbt.remove(val);
        rbt.print();
        std::cout << "中序遍历: ";
        rbt.inorder();
        std::cout << "是否有效: " << (rbt.isValid() ? "是" : "否") << "\n";
    }

    std::cout << "\n=== 查找测试 ===\n";
    std::cout << "查找 10: " << (rbt.search(10) ? "找到" : "未找到") << "\n";
    std::cout << "查找 15: " << (rbt.search(15) ? "找到" : "未找到") << "\n";

    return 0;
}