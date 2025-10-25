#include "Trie.h"

Trie::Trie() {
    root = new TrieNode();
}

Trie::~Trie() {
    clear(root);
}

void Trie::clear(TrieNode* node) {
    if (!node) return;
    for (auto const& [key, val] : node->children) {
        clear(val);
    }
    delete node;
}

void Trie::insert(const std::string& word) {
    TrieNode* current = root;
    for (char ch : word) {
        if (current->children.find(ch) == current->children.end()) {
            current->children[ch] = new TrieNode();
        }
        current = current->children[ch];
    }
    current->isEndOfWord = true;
}

std::vector<std::string> Trie::searchPrefix(const std::string& prefix) const {
    std::vector<std::string> results;
    TrieNode* current = root;
    for (char ch : prefix) {
        if (current->children.find(ch) == current->children.end()) {
            return results; // No words with this prefix
        }
        current = current->children[ch];
    }
    
    std::string currentPrefix = prefix;
    collectWords(current, currentPrefix, results);
    return results;
}

void Trie::collectWords(TrieNode* node, std::string& currentPrefix, std::vector<std::string>& results) const{
    if (node->isEndOfWord) {
        results.push_back(currentPrefix);
    }
    for (auto const& [key, val] : node->children) {
        currentPrefix.push_back(key);
        collectWords(val, currentPrefix, results);
        currentPrefix.pop_back(); // Backtrack
    }
}
