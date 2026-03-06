#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef enum { RED, BLACK } Color;

typedef struct Node {
    int data;
    Color color;
    struct Node* left;
    struct Node* right;
    struct Node* parent;
} Node;

typedef struct {
    Node* root;
    Node* nil;  // 哨兵节点
} RedBlackTree;

// 创建新节点
Node* createNode(int data) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->data = data;
    node->color = RED;
    node->left = node->right = node->parent = NULL;
    return node;
}

// 初始化树
void initTree(RedBlackTree* tree) {
    tree->nil = (Node*)malloc(sizeof(Node));
    tree->nil->color = BLACK;
    tree->nil->left = tree->nil->right = tree->nil->parent = NULL;
    tree->root = tree->nil;
}

// 左旋
void rotateLeft(RedBlackTree* tree, Node* x) {
    Node* y = x->right;
    x->right = y->left;
    if (y->left != tree->nil) y->left->parent = x;
    y->parent = x->parent;
    if (x->parent == tree->nil) tree->root = y;
    else if (x == x->parent->left) x->parent->left = y;
    else x->parent->right = y;
    y->left = x;
    x->parent = y;
}

// 右旋
void rotateRight(RedBlackTree* tree, Node* x) {
    Node* y = x->left;
    x->left = y->right;
    if (y->right != tree->nil) y->right->parent = x;
    y->parent = x->parent;
    if (x->parent == tree->nil) tree->root = y;
    else if (x == x->parent->right) x->parent->right = y;
    else x->parent->left = y;
    y->right = x;
    x->parent = y;
}

// 插入修复
void insertFixup(RedBlackTree* tree, Node* z) {
    while (z->parent->color == RED) {
        if (z->parent == z->parent->parent->left) {
            Node* y = z->parent->parent->right;  // 叔叔节点
            if (y->color == RED) {  // 情况1：叔叔是红色
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            }
            else {
                if (z == z->parent->right) {  // 情况2：z是右孩子
                    z = z->parent;
                    rotateLeft(tree, z);
                }
                // 情况3：z是左孩子
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                rotateRight(tree, z->parent->parent);
            }
        }
        else {  // 对称情况
            Node* y = z->parent->parent->left;
            if (y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            }
            else {
                if (z == z->parent->left) {
                    z = z->parent;
                    rotateRight(tree, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                rotateLeft(tree, z->parent->parent);
            }
        }
    }
    tree->root->color = BLACK;
}

// 插入节点
void insert(RedBlackTree* tree, int data) {
    Node* z = createNode(data);
    z->left = z->right = z->parent = tree->nil;

    Node* y = tree->nil;
    Node* x = tree->root;

    while (x != tree->nil) {
        y = x;
        if (z->data < x->data) x = x->left;
        else x = x->right;
    }

    z->parent = y;
    if (y == tree->nil) tree->root = z;
    else if (z->data < y->data) y->left = z;
    else y->right = z;

    insertFixup(tree, z);
}

// 中序遍历
void inorderHelper(RedBlackTree* tree, Node* node) {
    if (node != tree->nil) {
        inorderHelper(tree, node->left);
        printf("%d(%c) ", node->data, node->color == RED ? 'R' : 'B');
        inorderHelper(tree, node->right);
    }
}

void inorder(RedBlackTree* tree) {
    inorderHelper(tree, tree->root);
    printf("\n");
}

// 查找节点
Node* search(RedBlackTree* tree, int data) {
    Node* current = tree->root;
    while (current != tree->nil) {
        if (data == current->data) return current;
        else if (data < current->data) current = current->left;
        else current = current->right;
    }
    return tree->nil;
}

// 查找最小值
Node* minimum(RedBlackTree* tree, Node* node) {
    while (node->left != tree->nil) node = node->left;
    return node;
}

// 替换子树
void transplant(RedBlackTree* tree, Node* u, Node* v) {
    if (u->parent == tree->nil) tree->root = v;
    else if (u == u->parent->left) u->parent->left = v;
    else u->parent->right = v;
    v->parent = u->parent;
}

// 删除修复
void deleteFixup(RedBlackTree* tree, Node* x) {
    while (x != tree->root && x->color == BLACK) {
        if (x == x->parent->left) {
            Node* w = x->parent->right;  // 兄弟节点
            if (w->color == RED) {  // 情况1
                w->color = BLACK;
                x->parent->color = RED;
                rotateLeft(tree, x->parent);
                w = x->parent->right;
            }
            if (w->left->color == BLACK && w->right->color == BLACK) {  // 情况2
                w->color = RED;
                x = x->parent;
            }
            else {
                if (w->right->color == BLACK) {  // 情况3
                    w->left->color = BLACK;
                    w->color = RED;
                    rotateRight(tree, w);
                    w = x->parent->right;
                }
                // 情况4
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->right->color = BLACK;
                rotateLeft(tree, x->parent);
                x = tree->root;
            }
        }
        else {  // 对称情况
            Node* w = x->parent->left;
            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                rotateRight(tree, x->parent);
                w = x->parent->left;
            }
            if (w->right->color == BLACK && w->left->color == BLACK) {
                w->color = RED;
                x = x->parent;
            }
            else {
                if (w->left->color == BLACK) {
                    w->right->color = BLACK;
                    w->color = RED;
                    rotateLeft(tree, w);
                    w = x->parent->left;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->left->color = BLACK;
                rotateRight(tree, x->parent);
                x = tree->root;
            }
        }
    }
    x->color = BLACK;
}

// 删除节点
void deleteNode(RedBlackTree* tree, int data) {
    Node* z = search(tree, data);
    if (z == tree->nil) return;

    Node* y = z;
    Node* x;
    Color y_original_color = y->color;

    if (z->left == tree->nil) {
        x = z->right;
        transplant(tree, z, z->right);
    }
    else if (z->right == tree->nil) {
        x = z->left;
        transplant(tree, z, z->left);
    }
    else {
        y = minimum(tree, z->right);
        y_original_color = y->color;
        x = y->right;

        if (y->parent == z) {
            x->parent = y;
        }
        else {
            transplant(tree, y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }

        transplant(tree, z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }

    free(z);

    if (y_original_color == BLACK) {
        deleteFixup(tree, x);
    }
}

// 打印树结构
void printHelper(RedBlackTree* tree, Node* root, char* indent, int last) {
    if (root != tree->nil) {
        printf("%s", indent);
        if (last) {
            printf("R----");
            sprintf(indent, "%s   ", indent);
        }
        else {
            printf("L----");
            sprintf(indent, "%s|  ", indent);
        }

        printf("%d(%c)\n", root->data, root->color == RED ? 'R' : 'B');
        printHelper(tree, root->left, indent, 0);
        printHelper(tree, root->right, indent, 1);

        // 恢复indent
        indent[strlen(indent) - (last ? 3 : 4)] = '\0';
    }
}

void printTree(RedBlackTree* tree) {
    if (tree->root == tree->nil) {
        printf("树为空\n");
        return;
    }
    char indent[100] = "";
    printHelper(tree, tree->root, indent, 1);
}

// 清理内存
void clearHelper(RedBlackTree* tree, Node* node) {
    if (node != tree->nil) {
        clearHelper(tree, node->left);
        clearHelper(tree, node->right);
        free(node);
    }
}

void clearTree(RedBlackTree* tree) {
    clearHelper(tree, tree->root);
    free(tree->nil);
}

// 检查红黑树性质
bool isValidHelper(RedBlackTree* tree, Node* node, int currentBlackCount, int* expectedBlackCount) {
    if (node == tree->nil) {
        if (*expectedBlackCount == -1) {
            *expectedBlackCount = currentBlackCount;
        }
        else if (*expectedBlackCount != currentBlackCount) {
            return false;
        }
        return true;
    }

    if (node->color == BLACK) currentBlackCount++;

    // 检查连续红色
    if (node->color == RED) {
        if (node->left->color == RED || node->right->color == RED) {
            return false;
        }
    }

    return isValidHelper(tree, node->left, currentBlackCount, expectedBlackCount) &&
        isValidHelper(tree, node->right, currentBlackCount, expectedBlackCount);
}

bool isValid(RedBlackTree* tree) {
    if (tree->root->color != BLACK) return false;

    int expectedBlackCount = -1;
    return isValidHelper(tree, tree->root, 0, &expectedBlackCount);
}

// 测试代码
int main_red() {
    RedBlackTree tree;
    initTree(&tree);

    printf("=== 红黑树插入测试 ===\n");
    int insertValues[] = { 10, 20, 30, 15, 25, 5, 1 };

    for (int i = 0; i < sizeof(insertValues) / sizeof(insertValues[0]); i++) {
        printf("\n插入 %d:\n", insertValues[i]);
        insert(&tree, insertValues[i]);
        printTree(&tree);
        printf("中序遍历: ");
        inorder(&tree);
        printf("是否有效: %s\n", isValid(&tree) ? "是" : "否");
    }

    printf("\n=== 红黑树删除测试 ===\n");
    int deleteValues[] = { 15, 20, 5 };

    for (int i = 0; i < sizeof(deleteValues) / sizeof(deleteValues[0]); i++) {
        printf("\n删除 %d:\n", deleteValues[i]);
        deleteNode(&tree, deleteValues[i]);
        printTree(&tree);
        printf("中序遍历: ");
        inorder(&tree);
        printf("是否有效: %s\n", isValid(&tree) ? "是" : "否");
    }

    printf("\n=== 查找测试 ===\n");
    printf("查找 10: %s\n", search(&tree, 10) != tree.nil ? "找到" : "未找到");
    printf("查找 15: %s\n", search(&tree, 15) != tree.nil ? "找到" : "未找到");

    clearTree(&tree);
    return 0;
}