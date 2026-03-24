#include <stdio.h>
#include <stdlib.h>

// 定义线索二叉树节点
typedef struct ThreadNode {
    int data;
    struct ThreadNode* lchild, * rchild;
    int LTag, RTag; // 0: Link, 1: Thread
} ThreadNode, * ThreadTree;

// 全局变量，始终指向刚刚访问过的节点（用于建立线索）
ThreadTree pre = NULL;

// 创建新节点
ThreadNode* createNode(int data) {
    ThreadNode* node = (ThreadNode*)malloc(sizeof(ThreadNode));
    node->data = data;
    node->lchild = node->rchild = NULL;
    node->LTag = 0;
    node->RTag = 0;
    return node;
}

// 构建一个简单的测试树 (中序序列: 4 2 5 1 6 3 7)
//       1
//      / \
//     2   3
//    / \ / \
//   4  5 6  7
ThreadTree buildSampleTree() {
    ThreadTree root = createNode(1);
    root->lchild = createNode(2);
    root->rchild = createNode(3);
    root->lchild->lchild = createNode(4);
    root->lchild->rchild = createNode(5);
    root->rchild->lchild = createNode(6);
    root->rchild->rchild = createNode(7);
    return root;
}

// 核心：中序线索化递归函数
void InThread(ThreadTree p) {
    if (p != NULL) {
        // 1. 线索化左子树
        InThread(p->lchild);

        // 2. 处理当前节点 (建立线索)

        // 如果左孩子为空，建立前驱线索
        if (p->lchild == NULL) {
            p->lchild = pre;
            p->LTag = 1;
        }

        // 如果前驱节点存在且前驱的右孩子为空，建立后继线索
        if (pre != NULL && pre->rchild == NULL) {
            pre->rchild = p;
            pre->RTag = 1;
        }

        // 更新 pre 为当前节点
        pre = p;

        // 3. 线索化右子树
        InThread(p->rchild);
    }
}

// 对外接口：执行线索化
void CreateInThread(ThreadTree T) {
    pre = NULL;
    if (T != NULL) {
        InThread(T);
        // 处理遍历的最后一个节点 (它的右孩子肯定为空，需指向NULL或哨兵)
        pre->rchild = NULL;
        pre->RTag = 1;
    }
}

// 寻找中序遍历的第一个节点 (最左下节点)
ThreadNode* FirstNode(ThreadNode* p) {
    while (p->LTag == 0) {
        p = p->lchild;
    }
    return p;
}

// 寻找节点 p 在中序遍历下的后继节点
ThreadNode* NextNode(ThreadNode* p) {
    // 如果有右线索，直接返回后继
    if (p->RTag == 1) {
        return p->rchild;
    }
    else {
        // 如果有右孩子，后继是右子树的最左下节点
        return FirstNode(p->rchild);
    }
}

// 不使用栈和递归的中序遍历
void InOrderTraverse(ThreadTree T) {
    if (T == NULL) return;

    ThreadNode* p = FirstNode(T);
    while (p != NULL) {
        printf("%d ", p->data);
        p = NextNode(p);
    }
    printf("\n");
}

int main_xiansuo() {
    ThreadTree root = buildSampleTree();

    printf("原始树构建完成。\n");
    printf("开始中序线索化...\n");

    CreateInThread(root);

    printf("线索化后的中序遍历结果: ");
    InOrderTraverse(root);

    return 0;
}