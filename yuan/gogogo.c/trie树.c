#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// 定义 Trie 节点结构
typedef struct TrieNode {
    struct TrieNode* children[26]; // 26个小写字母
    bool isEndOfWord;              // 标记是否是单词结尾
} TrieNode;

// 创建新节点
TrieNode* createNode() {
    TrieNode* node = (TrieNode*)malloc(sizeof(TrieNode));
    node->isEndOfWord = false;
    for (int i = 0; i < 26; i++) {
        node->children[i] = NULL;
    }
    return node;
}

// 初始化 Trie (其实就是创建根节点)
TrieNode* trieInit() {
    return createNode();
}

// 插入单词
void trieInsert(TrieNode* root, const char* word) {
    TrieNode* current = root;
    int length = strlen(word);

    for (int i = 0; i < length; i++) {
        int index = word[i] - 'a'; // 将字符映射到 0-25

        // 如果路径不存在，创建新节点
        if (!current->children[index]) {
            current->children[index] = createNode();
        }

        // 移动到下一个节点
        current = current->children[index];
    }

    // 标记单词结束
    current->isEndOfWord = true;
}

// 搜索单词 (必须完整匹配)
bool trieSearch(TrieNode* root, const char* word) {
    TrieNode* current = root;
    int length = strlen(word);

    for (int i = 0; i < length; i++) {
        int index = word[i] - 'a';

        // 如果路径断了，说明不存在
        if (!current->children[index]) {
            return false;
        }

        current = current->children[index];
    }

    // 不仅路径要通，最后还必须标记为单词结尾
    return (current != NULL && current->isEndOfWord);
}

// 检查前缀
bool trieStartsWith(TrieNode* root, const char* prefix) {
    TrieNode* current = root;
    int length = strlen(prefix);

    for (int i = 0; i < length; i++) {
        int index = prefix[i] - 'a';

        if (!current->children[index]) {
            return false;
        }

        current = current->children[index];
    }

    // 只要路径能走完，说明有这个前缀
    return true;
}

// 递归释放内存 (后序遍历)
void trieFreeNode(TrieNode* node) {
    if (node == NULL) return;

    for (int i = 0; i < 26; i++) {
        if (node->children[i]) {
            trieFreeNode(node->children[i]);
        }
    }
    free(node);
}

void trieDestroy(TrieNode* root) {
    trieFreeNode(root);
}

// --- 测试主函数 ---
int main_trie() {
    TrieNode* root = trieInit();

    trieInsert(root, "apple");
    trieInsert(root, "app");
    trieInsert(root, "banana");

    printf("Search 'apple': %s\n", trieSearch(root, "apple") ? "Found" : "Not Found");
    printf("Search 'app': %s\n", trieSearch(root, "app") ? "Found" : "Not Found");
    printf("Search 'appl': %s\n", trieSearch(root, "appl") ? "Found" : "Not Found"); // 应该找不到

    printf("StartsWith 'app': %s\n", trieStartsWith(root, "app") ? "Yes" : "No");
    printf("StartsWith 'ban': %s\n", trieStartsWith(root, "ban") ? "Yes" : "No");
    printf("StartsWith 'cat': %s\n", trieStartsWith(root, "cat") ? "Yes" : "No");

    // 清理内存
    trieDestroy(root);

    return 0;
}