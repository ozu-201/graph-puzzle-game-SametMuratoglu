#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <unordered_set>
#include <unordered_map>
#include <limits>
#include <algorithm>

using namespace std;

struct Node {
    string word;
    vector<Node*> neighbors;

    Node(string w) : word(w) {}
};

class Graph {
public:
    unordered_set<string> dictionary;
    vector<Node*> nodes;

    void readDictionary(const string& filename) {
        ifstream file(filename);
        string word;

        while (file >> word) {
            dictionary.insert(word);
        }
    }

    void buildGraph(int length) {
        for (const auto& word : dictionary) {
            if (word.length() == length) {
                nodes.push_back(new Node(word));
            }
        }

        for (size_t i = 0; i < nodes.size(); ++i) {
            for (size_t j = i + 1; j < nodes.size(); ++j) {
                if (isOneLetterApart(nodes[i]->word, nodes[j]->word)) {
                    nodes[i]->neighbors.push_back(nodes[j]);
                    nodes[j]->neighbors.push_back(nodes[i]);
                }
            }
        }
    }

    vector<string> findShortestPathBFS(const string& start, const string& target) {
        return findShortestPath(start, target, false);
    }

    vector<string> findShortestPathDijkstra(const string& start, const string& target) {
        return findShortestPath(start, target, true);
    }

private:
    Node* findNode(const string& word) {
        for (const auto& node : nodes) {
            if (node->word == word) {
                return node;
            }
        }
        return nullptr;
    }

    vector<string> findShortestPath(const string& start, const string& target, bool useDijkstra) {
        vector<string> path;
        Node* startNode = findNode(start);

        if (startNode == nullptr) {
            cerr << "Start word not found in the dictionary." << endl;
            return path;
        }

        Node* targetNode = findNode(target);

        if (targetNode == nullptr) {
            cerr << "Target word not found in the dictionary." << endl;
            return path;
        }

        unordered_map<Node*, int> distance;
        unordered_map<Node*, Node*> previous;
        priority_queue<pair<int, Node*>, vector<pair<int, Node*>>, greater<pair<int, Node*>>> pq;

        for (const auto& node : nodes) {
            distance[node] = numeric_limits<int>::max();
        }

        distance[startNode] = 0;
        pq.push({0, startNode});

        while (!pq.empty()) {
            Node* current = pq.top().second;
            pq.pop();

            for (const auto& neighbor : current->neighbors) {
                int alt = distance[current] + 1;

                if (alt < distance[neighbor]) {
                    distance[neighbor] = alt;
                    previous[neighbor] = current;
                    pq.push({alt, neighbor});
                }
            }
        }

        Node* current = targetNode;
        while (current != nullptr) {
            path.push_back(current->word);
            current = previous[current];
        }

        reverse(path.begin(), path.end());

        return path;
    }

    bool isOneLetterApart(const string& word1, const string& word2) {
        int diffCount = 0;

        for (size_t i = 0; i < word1.length(); ++i) {
            if (word1[i] != word2[i]) {
                ++diffCount;
                if (diffCount > 1) {
                    return false;
                }
            }
        }

        return diffCount == 1;
    }
};

int main() {
    Graph graph;
    graph.readDictionary("dictionary.txt");

    string startWord, targetWord;
    cout << "Select the word you start to search: ";
    cin >> startWord;
    cout << "Enter the target word: ";
    cin >> targetWord;

    int wordLength = startWord.length();
    graph.buildGraph(wordLength);

    vector<string> bfsPath = graph.findShortestPathBFS(startWord, targetWord);
    cout << "Shortest Path from '" << startWord << "' to '" << targetWord << "' (BFS):" << endl;

    for (size_t i = 0; i < bfsPath.size(); ++i) {
        cout << bfsPath[i];
        if (i < bfsPath.size() - 1) {
            cout << " -> ";
        }
    }
    cout << endl;


    vector<string> dijkstraPath = graph.findShortestPathDijkstra(startWord, targetWord);
    cout << "Shortest Path from '" << startWord << "' to '" << targetWord << "' (Dijkstra):" << endl;

    for (size_t i = 0; i < dijkstraPath.size(); ++i) {
        cout << dijkstraPath[i];
        if (i < dijkstraPath.size() - 1) {
            cout << " -> ";
        }
    }
    cout << endl;

    return 0;
}