#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <vector>

using namespace std;

struct Node {
    string word;
    vector<Node*> neighbors;

    Node(const string& w) : word(w) {}
};

bool isValidChange(const string& word1, const string& word2) {

    return true;
}

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

vector<string> breadthFirstSearch(Node* start, const string& target) {
    queue<pair<Node*, vector<string>>> q;
    unordered_set<Node*> visited;

    q.push({start, {start->word}});
    visited.insert(start);

    while (!q.empty()) {
        auto current = q.front();
        q.pop();

        for (Node* neighbor : current.first->neighbors) {
            if (visited.find(neighbor) == visited.end()) {
                vector<string> path = current.second;
                path.push_back(neighbor->word);

                if (neighbor->word == target) {
                    return path;
                }

                q.push({neighbor, path});
                visited.insert(neighbor);
            }
        }
    }

    return {};
}

int main() {
    Node* startingNode = initializeGraph("dictionary.txt");

    string targetWord = "target_word";

    vector<string> path = breadthFirstSearch(startingNode, targetWord);

    if (path.empty()) {
        cout << "No path found to the target word." << endl;
    } else {
        cout << "Path to the target word:" << endl;
        for (const string& word : path) {
            cout << word << " ";
        }
        cout << endl;
    }

    return 0;
}