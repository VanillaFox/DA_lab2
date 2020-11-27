#include <iostream>
#include <cstring>
#include <fstream>
#include "tree.h"

void DoLower(TKey* key) {
    int lengh = strlen(key);
    for (int i = 0; i < lengh; i++) {
        key[i] = tolower(key[i]);
    }
}

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    TAvlTree tree;
    TKey key[STR_SIZE + 1];
    TValue value;
    std::ofstream fos;
    std::ifstream fis;

    while (std::cin >> key) {
        if (key[0] == '+') {
            std::cin >> key >> value;
            DoLower(key);
            tree.Root = tree.Insert(tree.Root, key, value);
        }
        else if (key[0] == '-') {
            std::cin >> key;
            DoLower(key);
            tree.Root = tree.DeleteElem(tree.Root, key);
        }
        else if (key[0] == '!' && strlen(key) == 1) {
            std::cin >> key;
            if (key[0] == 'S') {
                std::cin >> key;
                fos.open(key, std::ios::out | std::ios::binary);
                if (fos) {
                    tree.Save(fos, tree.Root);
                    std::cout << "OK\n";
                }
                else {
                    std::cout << "ERROR: can't open file\n";
                }
                fos.close();
            }
            else if (key[0] == 'L') {
                std::cin >> key;
                fis.open(key, std::ios::in | std::ios::binary);
                if (fis) {
                    tree.DeleteTree(tree.Root);
                    tree.Root = tree.Load(fis, nullptr);
                    std::cout << "OK\n";
                    fis.close();
                }
                else {
                    std::cout << "ERROR: can't open file\n";
                }
            }
        }
        else {
            DoLower(key);
            tree.PrintFind(key);
        }
    }
    return 0;
}
