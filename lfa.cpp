// https://www.geeksforgeeks.org/trie-insert-and-search/

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <queue>

using namespace std;

// TrieNode structure taken from geeksforgeeks
struct TrieNode {
    // flags indicating whether the node forms accepted/rejected words
    bool accept = false;
    bool reject = false;
    TrieNode* child[26];

    TrieNode() {
        // initially all are set to false
        accept = false;
        reject = false;
        // no children for the node yet
        for (int i = 0; i < 26; i++) {
            child[i] = nullptr;
        }
    }
};

// insert function for adding keys into the trie, taken from geeksforgeeks
void insert_key(TrieNode* root, const string& key, bool accept, bool reject) {
    TrieNode* curr = root;
    
    // for each character in the word
    for (char c : key) {
        // if the child is nullptr, create a new TrieNode
        if (curr->child[c - 'a'] == nullptr) {
            curr->child[c - 'a'] = new TrieNode();
        }
        curr = curr->child[c - 'a'];

        // check the word type and set the corresponding flag
        // set reject flag if it's a rejected word and accept flag if it's an accepted one
        if (accept) curr->accept = true;
        if (reject) curr->reject = true;
    }
}

// transition matrix initialized with 1000 (max states)
vector<vector<int>> transitions(1000, vector<int>(26, 1));
// variables for accept, reject, and initial states
int accept_state = 0;
int reject_state = 1;
int initial_state = 2;
// already have the accept and reject states numbered
int num_states = 2;
unordered_map<TrieNode*, int> states;

// function to traverse the trie
void check_nodes(TrieNode* root) {
    // queue to store nodes that need to be checked for children
    queue<TrieNode*> q;
    // add the root to the queue and assign it a state
    q.push(root);
    states[root] = num_states++;

    // to traverse all nodes
    while (!q.empty()) {
        TrieNode* curr = q.front();
        q.pop();

        // get the current node's state
        int curr_state = states[curr];

        // check the current node's children by iterating through the alphabet
        for (int i = 0; i < 26; i++) {
            if (curr->child[i] != nullptr) {
                TrieNode* next = curr->child[i];
                // if the child node has both accept and reject flags set, add a new state in AFD,
                // create a transition from the current node to the child and add the child to the queue
                if (next->accept && next->reject) {
                    states[next] = num_states++;
                    q.push(next);
                    transitions[curr_state][i] = states[next];
                    continue;
                }
                // the child has only the accept flag, meaning the word is accepted
                if (next->accept) {
                    // create a transition to the accept state
                    transitions[curr_state][i] = accept_state;
                    continue;
                }
                // the child has only the reject flag, meaning the word is rejected
                if (next->reject) {
                    // create a transition to the reject state
                    transitions[curr_state][i] = reject_state;
                    continue;
                }
            }
        }
    }
}

int main() {
    ifstream fin("input.txt");
    ofstream fout("output.txt");

    // read input data
    int cnt_accept, cnt_fail, len_string;
    fin >> cnt_accept >> cnt_fail >> len_string;
    
    // create the root of the trie
    TrieNode* root = new TrieNode();

    // read the accepted words and insert them into the trie, setting the accept flag for each node
    for (int i = 0; i < cnt_accept; i++) {
        string word;
        fin >> word;
        insert_key(root, word, true, false);
    }

    // read the rejected words and insert them into the trie, setting the reject flag for each node
    for (int i = 0; i < cnt_fail; i++) {
        string word;
        fin >> word;
        insert_key(root, word, false, true);
    }

    fin.close();

    // ensure any transition from the accept state goes to itself, similarly for the reject state
    for (int i = 0; i < 26; i++) {
        transitions[accept_state][i] = accept_state;
        transitions[reject_state][i] = reject_state;
    }

    // generate the transition matrix based on the flags of the nodes
    check_nodes(root);

    // write the DFA to the output file
    fout << "dfa\n";
    fout << num_states << " 1 " << initial_state << "\n";
    fout << accept_state << "\n";

    // write the transition matrix
    for (int i = 0; i < num_states; i++) {
        for (int j = 0; j < 26; j++) {
            fout << transitions[i][j] << " ";
        }
        fout << "\n";
    }

    fout.close();
    return 0;
}
