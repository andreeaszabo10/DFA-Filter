Copyright Szabo Cristina-Andreea 2024-2025

# DFA Filter

Am folosit structura de date trie in care am adaugat toate cuvintele din fisierul de input. Pentru fiecare nod am luat doua flag-uri, unul de accept si unul de reject. Ele imi indica daca prefixul pana in nodul curent duce la un cuvant acceptat sau respins sau ambele. Dupa ce am adaugat toate cuvintele in trie cu flag-urile corespunzatoare active, am iterat printre nodurile trie-ului si pentru fiecare nod am verificat toti copiii si daca respectivul copil avea ambele flag-uri pe 1 am adaugat o noua stare in afd, o tranzitie de la nod curent la copil si copilul in coada de verificat, altfel am verificat care flag e pe 1 si am adaugat o tranzitie de la nodul curent la starea de accept/reject in functie de flag. La final am printat numarul de stari calculat, numarul de stari finale adica mereu 1 si starea initiala, apoi matricea de tranzitii obtinuta.
