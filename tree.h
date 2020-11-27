#pragma once
#include<iostream>
#include <fstream>
#include<cstring>
const int STR_SIZE = 256;
using TKey = char;
using TValue = unsigned long long;

class TAvlTree {
public:
    struct TNode {
        TNode() :Key(), Value(), Left(nullptr), Right(nullptr), Height(1) {}
        TNode(TKey* key, TValue val) : Value(val), Left(nullptr), Right(nullptr), Height(1) {
            memcpy(Key, key, strlen(key) + 1);
        }
        ~TNode() {}
        TNode* Left, * Right;
        TKey Key[STR_SIZE + 1];
        TValue Value;
        long long Height;
    };


    int CompareKey(TKey* root, TKey* str) {
        return strcmp(root, str);
    }

    long long GetHeight(TNode* node) {
        return node != nullptr ? node->Height : 0;
    }

    long long BalanceFactor(TNode* node) {
        if (node == nullptr) {
            return 0;
        }
        return GetHeight(node->Right) - GetHeight(node->Left);
    }

    void CorrectHeight(TNode* node) {
        long long h_left = GetHeight(node->Left);
        long long h_right = GetHeight(node->Right);
        node->Height = (h_left > h_right ? h_left : h_right) + 1;
    }

    TNode* RotateRight(TNode* node) {
        TNode* q = node->Left;
        node->Left = q->Right;
        q->Right = node;
        CorrectHeight(q);
        CorrectHeight(node);
        return q;
    }

    TNode* RotateLeft(TNode* node) {
        TNode* q = node->Right;
        node->Right = q->Left;
        q->Left = node;
        CorrectHeight(q);
        CorrectHeight(node);
        return q;
    }

    TNode* Balance(TNode* node) {
        CorrectHeight(node);
        if (BalanceFactor(node) == 2) {
            if (BalanceFactor(node->Right) < 0) {
                node->Right = RotateRight(node->Right);
            }
            return RotateLeft(node);
        }
        else if (BalanceFactor(node) == -2) {
            if (BalanceFactor(node->Left) > 0) {
                node->Left = RotateLeft(node->Left);
            }
            return RotateRight(node);
        }
        return node;
    }

    TNode* Insert(TNode* node, TKey* key, TValue val) {
        if (node == nullptr) {
            node = new TNode();
            node->Value = val;
            memcpy(node->Key, key, strlen(key) + 1);
            std::cout << "OK\n";
            return node;
        }
        if (CompareKey(node->Key, key) > 0) {
            node->Left = Insert(node->Left, key, val);
        }
        else if (CompareKey(node->Key, key) < 0) {
            node->Right = Insert(node->Right, key, val);
        }
        else {
            std::cout << "Exist\n";
        }
        return Balance(node);
    }


    TNode* MinNode(TNode* node) {
        TNode* tmp = node;
        while (tmp->Left != nullptr) {
            tmp = tmp->Left;
        }
        return tmp;
    }

    TNode* DeleteElem(TNode* node, TKey* key) {
        if (node == nullptr) {
            std::cout << "NoSuchWord\n";
            return node;
        }
        if (CompareKey(node->Key, key) < 0) {
            node->Right = DeleteElem(node->Right, key);
        }
        else if (CompareKey(node->Key, key) > 0) {
            node->Left = DeleteElem(node->Left, key);
        }
        else {
            if (node->Left == nullptr || node->Right == nullptr) {
                TNode* tmp = node->Left ? node->Left : node->Right;
                if (tmp == nullptr) {
                    tmp = node;
                    node = nullptr;
                }
                else {
                    *node = *tmp;
                }
                std::cout << "OK\n";
                delete tmp;
            }
            else {
                TNode* tmp = MinNode(node->Right);
                memcpy(node->Key, tmp->Key, strlen(tmp->Key) + 1);
                node->Value = tmp->Value;
                node->Right = DeleteElem(node->Right, tmp->Key);
            }
        }
        if (node == nullptr) {
            return node;
        }
        return Balance(node);
    }

    TNode* Root;

    TNode* Find(TNode* node, TKey* key) {
        if (node == nullptr) {
            return nullptr;
        }
        TNode* tmp = node;
        while (tmp != nullptr) {
            if (CompareKey(tmp->Key, key) > 0) {
                tmp = tmp->Left;
            }
            else if (CompareKey(tmp->Key, key) < 0) {
                tmp = tmp->Right;
            }
            else {
                return tmp;
            }
        }
        delete tmp;
        return nullptr;
    }

    TAvlTree() : Root(nullptr) {}

    void DeleteTree(TNode* root) {
        if (root != nullptr) {
            DeleteTree(root->Left);
            DeleteTree(root->Right);
            delete root;
        }
    }

    ~TAvlTree() {
        DeleteTree(Root);
    }

    void PrintFind(TKey* key) {
        TNode* res = Find(Root, key);
        if (res != nullptr) {
            std::cout << "OK: " << res->Value << std::endl;
        }
        else {
            std::cout << "NoSuchWord\n";
        }
    }

    void Save(std::ostream& file, TNode* node) {
        if (node == nullptr) {
            return;
        }
        int keySize = strlen(node->Key);
        file.write((char*)&keySize, sizeof(int));
        file.write(node->Key, keySize);
        file.write((char*)&(node->Value), sizeof(TValue));

        bool hasLeft = (node->Left != nullptr);
        bool hasRight = (node->Right != nullptr);

        file.write((char*)&hasLeft, sizeof(bool));
        file.write((char*)&hasRight, sizeof(bool));

        if (hasLeft) {
            Save(file, node->Left);
        }
        if (hasRight) {
            Save(file, node->Right);
        }
    }

    TNode* Load(std::istream& file, TNode* node) {
        TNode* root = nullptr;

        int keysize;
        file.read((char*)&keysize, sizeof(int));

        if (file.gcount() == 0) {
            return root;
        }

        TKey* key = new char[keysize + 1];
        key[keysize] = '\0';
        file.read(key, keysize);

        TValue value;
        file.read((char*)&value, sizeof(TValue));

        bool hasLeft = false;
        bool hasRight = false;
        file.read((char*)&hasLeft, sizeof(bool));
        file.read((char*)&hasRight, sizeof(bool));

        root = new TNode();
        root->Value = value;
        memcpy(root->Key, key, strlen(key));
        if (hasLeft) {
            root->Left = Load(file, root);
        }
        else {
            root->Left = nullptr;
        }

        if (hasRight) {
            root->Right = Load(file, root);
        }
        else {
            root->Right = nullptr;
        }
        delete[] key;
        return root;
    }
};
