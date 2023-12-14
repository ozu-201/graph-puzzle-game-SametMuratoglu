#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

struct Node {
    string word;
    vector<Node*> neighbors;

    Node(const string& w) : word(w) {}
};

Node* buildGraph(const vector<string>& words) {
    unordered_map<string, Node*> wordMap;

    for (const string& word : words) {
        Node* newNode = new Node(word);
        wordMap[word] = newNode;
    }

    for (const string& word : words) {
        Node* currentNode = wordMap[word];

        for (const string& otherWord : words) {
            if (word != otherWord && isValidChange(word, otherWord)) {
                currentNode->neighbors.push_back(wordMap[otherWord]);
            }
        }
    }

    return wordMap[words[0]];
}


bool isValidChange(const string& word1, const string& word2) {

    return true;
}

Node* initializeGraph(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        exit(1);
    }

    vector<string> words;
    string word;

    while (file >> word) {
        words.push_back(word);
    }

    file.close();

    return buildGraph(words);
}

int main() {

    return 0;
}