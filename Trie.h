#ifndef TRIE_H
#define TRIE_H

#include <string>
#include <vector>
#include <unordered_map>

class TrieNode {
public:
    std::unordered_map<char, TrieNode*> children;
    bool isEndOfWord;

    TrieNode() : isEndOfWord(false) {}
};

class Trie {
private:
    TrieNode* root;
    void collectWords(TrieNode* node, std::string& currentPrefix, std::vector<std::string>& results) const;
    void clear(TrieNode* node); // Helper for destructor

public:
    Trie();
    ~Trie(); // Destructor to free memory
    void insert(const std::string& word);
    std::vector<std::string> searchPrefix(const std::string& prefix) const;
};

#endif // TRIE_H