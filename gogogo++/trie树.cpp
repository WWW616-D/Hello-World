#include <iostream>
#include <vector>
#include <string>
#include <memory> // 用于智能指针，但这里为了演示经典结构，先用原始指针配合析构函数

using namespace std;

class TrieNode {
public:
    // 使用固定大小的数组，或者可以用 map<char, TrieNode*> 节省空间
    TrieNode* children[26];
    bool isEndOfWord;

    TrieNode() {
        isEndOfWord = false;
        for (int i = 0; i < 26; ++i) {
            children[i] = nullptr;
        }
    }

    // 析构函数：自动释放子节点内存
    ~TrieNode() {
        for (int i = 0; i < 26; ++i) {
            if (children[i] != nullptr) {
                delete children[i];
            }
        }
    }
};

class Trie {
private:
    TrieNode* root;

public:
    Trie() {
        root = new TrieNode();
    }

    // 析构函数：删除根节点会触发连锁反应，释放整棵树
    ~Trie() {
        delete root;
    }

    void insert(string word) {
        TrieNode* current = root;
        for (char c : word) {
            int index = c - 'a';
            if (current->children[index] == nullptr) {
                current->children[index] = new TrieNode();
            }
            current = current->children[index];
        }
        current->isEndOfWord = true;
    }

    bool search(string word) {
        TrieNode* current = root;
        for (char c : word) {
            int index = c - 'a';
            if (current->children[index] == nullptr) {
                return false;
            }
            current = current->children[index];
        }
        return current->isEndOfWord;
    }

    bool startsWith(string prefix) {
        TrieNode* current = root;
        for (char c : prefix) {
            int index = c - 'a';
            if (current->children[index] == nullptr) {
                return false;
            }
            current = current->children[index];
        }
        return true;
    }
};

// --- 测试主函数 ---
int main_trie() {
    // 不需要手动 delete，Trie 对象销毁时会自动调用析构函数清理内存
    Trie trie;

    trie.insert("apple");
    trie.insert("app");
    trie.insert("banana");

    cout << "Search 'apple': " << (trie.search("apple") ? "Found" : "Not Found") << endl;
    cout << "Search 'app': " << (trie.search("app") ? "Found" : "Not Found") << endl;
    cout << "Search 'appl': " << (trie.search("appl") ? "Found" : "Not Found") << endl;

    cout << "StartsWith 'app': " << (trie.startsWith("app") ? "Yes" : "No") << endl;
    cout << "StartsWith 'ban': " << (trie.startsWith("ban") ? "Yes" : "No") << endl;
    cout << "StartsWith 'cat': " << (trie.startsWith("cat") ? "Yes" : "No") << endl;

    return 0;
}