Copyright Szabo Cristina-Andreea 2024-2025

# Trie-based DFA Filter

## **Overview**
This project implements a **Deterministic Finite Automaton** using a **Trie data structure** to process a set of accepted and rejected words. The DFA is constructed based on input words, categorizing them as either accepted or rejected. The output consists of the computed DFA states, transitions, and a structured transition matrix.

## **Features**
- Uses a **Trie** to efficiently store and classify words.
- Flags words as **accepted** or **rejected** at the node level.
- Constructs a **DFA** by mapping Trie nodes to states.
- Outputs a **transition matrix** for easy visualization and further processing.

## **Implementation Details**
1. **Trie Structure**  
   - Each Trie node contains two flags:
     - `accept` → Marks if the prefix leads to an accepted word.
     - `reject` → Marks if the prefix leads to a rejected word.
   - Each node has 26 child pointers (one for each letter in the English alphabet).

2. **Building the DFA**  
   - Reads input words and inserts them into the Trie with their respective flags.
   - Iterates through Trie nodes and assigns them unique DFA states.
   - Adds transitions:
     - If a node has both `accept` and `reject` flags set → a new DFA state is created.
     - If only one flag is set → the transition is made to either the accept or reject state.

3. **Transition Matrix Construction**  
   - The final DFA representation is stored in a matrix format.
   - States transition to accept/reject or other computed states based on input characters.
   - Ensures all accept/reject states remain final states in the matrix.
