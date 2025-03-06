// implementare trie de aici https://www.geeksforgeeks.org/trie-insert-and-search/

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <queue>

using namespace std;

// structura de trie node luata de pe geeksforgeeks
struct TrieNode {
    // flag-uri care imi arata daca nodul formeaza cuvinte acceptate/respinse
    bool accept = false;
    bool reject = false;
    TrieNode* child[26];

    TrieNode() {
        // initial toate sunt pe false
        accept = false;
        reject = false;
        // nu exista inca vreun copil al nodului
        for (int i = 0; i < 26; i++) {
            child[i] = nullptr;
        }
    }
};

// functie de inserare in trie luata de pe geeksforgeeks
void insert_key(TrieNode* root, const string& key, bool accept, bool reject) {
    TrieNode* curr = root;
    
    // pentru fiecare caracter din cuvant
    for (char c : key) {
        // verific daca 
        if (curr->child[c - 'a'] == nullptr) {
            curr->child[c - 'a'] = new TrieNode();
        }
        curr = curr->child[c - 'a'];

        // verific ce tip de cuvant e si setez flag-ul corespunzator
        // reject pe 1 daca e respins si accept pe 1 daca e 
        if (accept) curr->accept = true;
        if (reject) curr->reject = true;
    }
}

// matricea de tranzitii initializata cu 1000(nr max de stari)
vector<vector<int>> transitions(1000, vector<int>(26, 1));
// numar pentru starea de accept, reject si initiala
int accept_state = 0;
int reject_state = 1;
int initial_state = 2;
// deja am starea de accept si reject numarate
int num_states = 2;
unordered_map<TrieNode*, int> states;

// functie de parcurgere a trie-ului
void check_nodes(TrieNode* root) {
    // coada in care retin nodurile pentru care trebuie sa verific copiii
    queue<TrieNode*> q;
    // adaug root-ul in coada si ii asignez o stare
    q.push(root);
    states[root] = num_states++;

    // ca sa parcurg toate nodurile
    while (!q.empty()) {
        TrieNode* curr = q.front();
        q.pop();

        // iau starea nodului curent
        int curr_state = states[curr];

        // verific copiii nodului curent iterand prin toate literele alfabetului
        for (int i = 0; i < 26; i++) {
            if (curr->child[i] != nullptr) {
                TrieNode* next = curr->child[i];
                // daca nodul copil are ambele flag-uri pe 1, adaug o noua stare la afd,
                // tranzitie de la nodul curent la copil si adaug copilul in coada
                if (next->accept && next->reject) {
                    states[next] = num_states++;
                    q.push(next);
                    transitions[curr_state][i] = states[next];
                    continue;
                }
                // nodul are doar flag-ul de accept deci e clar ca e acceptat cuvantul
                if (next->accept) {
                    // adaug tranzitie spre starea de accept
                    transitions[curr_state][i] = accept_state;
                    continue;
                }
                // nodul are doar flag-ul de reject deci cuvantul e respins
                if (next->reject) {
                    // adaug tranzitie spre starea de reject
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

    // citesc datele din fisierul de input
    int cnt_accept, cnt_fail, len_string;
    fin >> cnt_accept >> cnt_fail >> len_string;
    
    // creez root-ul
    TrieNode* root = new TrieNode();

    // citesc cuvintele din accept si le pun in trie, fiecare nod avand pe 1 flag-ul de accept
    for (int i = 0; i < cnt_accept; i++) {
        string word;
        fin >> word;
        insert_key(root, word, true, false);
    }

    // citesc cuvintele din reject si le pun in trie, fiecare nod avand pe 1 flag-ul de reject
    for (int i = 0; i < cnt_fail; i++) {
        string word;
        fin >> word;
        insert_key(root, word, false, true);
    }

    fin.close();

    // fac ca orice tranzitie din starea de accept sa mearga tot in ea, la fel si cu reject
    for (int i = 0; i < 26; i++) {
        transitions[accept_state][i] = accept_state;
        transitions[reject_state][i] = reject_state;
    }

    // creez matricea de tranzitii in functie de flag-urile nodurilor
    check_nodes(root);

    // scriu dfa pe prima linie, apoi numarul de stari, apoi 1 pt ca am o singura
    // stare finala, starea initiala care e 2, apoi pe urmatoarea linie starea de accept adica 0
    fout << "dfa\n";
    fout << num_states << " 1 " << initial_state << "\n";
    fout << accept_state << "\n";

    // scriu matricea de tranzitii
    for (int i = 0; i < num_states; i++) {
        for (int j = 0; j < 26; j++) {
            fout << transitions[i][j] << " ";
        }
        fout << "\n";
    }

    fout.close();
    return 0;
}