#include <iostream>
#include <queue>
#include <utility>
using namespace std;
struct Node{
    int key;
    Node* left;
    Node* right;
    int height;
};
typedef Node* Tree;
Tree newNode(int data){
    return new Node{data, nullptr, nullptr, 1};
}
int getHeight(Tree root){
    if(!root)
        return 0;
    return root->height;
}
void LL(Tree &root){
    Tree p = root->left;
    root->left = p->right;
    p->right = root;
    root->height = max(getHeight(root->left), getHeight(root->right)) + 1;
    root = p;
    root->height = max(getHeight(root->left), getHeight(root->right)) + 1;
}
void RR(Tree &root){
    Tree p = root->right;
    root->right = p->left;
    p->left = root;
    root->height = max(getHeight(root->left), getHeight(root->right)) + 1;
    root = p;
    root->height = max(getHeight(root->left), getHeight(root->right)) + 1;
}
void LR(Tree &root){
    RR(root->left);
    LL(root);
}
void RL(Tree &root){
    LL(root->right);
    RR(root);
}
void balanceLeft(Tree &root){
    if(!root->left->left)
        LR(root);
    else if(!root->left->right)
        LL(root);
    else{
        if(root->left->right->height > root->left->left->height)
            LR(root);
        else
            LL(root);
    }
}
void balanceRight(Tree &root){
    if(!root->right->right)
        RL(root);
    else if(!root->right->left)
        RR(root);
    else{
        if(root->right->left->height > root->right->right->height)
            RL(root);
        else
            RR(root);
    }
}
Tree insertNode(Tree root, int data){
    if(!root)
        return newNode(data);
    if(data < root->key)
        root->left = insertNode(root->left, data);
    else if(data > root->key)
        root->right = insertNode(root->right, data);
    root->height = max(getHeight(root->left), getHeight(root->right)) + 1;
    if(getHeight(root->left) - getHeight(root->right) == 2)
        balanceLeft(root);
    else if(getHeight(root->right) - getHeight(root->left) == 2)
        balanceRight(root);
    return root;
}
Tree deleteNode(Tree root, int data) {
    if (!root) {
        return nullptr;
    }
    if (data < root->key) {
        root->left = deleteNode(root->left, data);
    } else if (data > root->key) {
        root->right = deleteNode(root->right, data);
    } else {
        if (!root->left || !root->right) {
            Tree temp = root->left ? root->left : root->right;
            delete root;
            return temp;
        } else {
            Tree temp = root->right;
            while (temp->left) {
                temp = temp->left;
            }
            root->key = temp->key;
            root->right = deleteNode(root->right, temp->key);
        }
    }
    root->height = max(getHeight(root->left), getHeight(root->right)) + 1;
    if (getHeight(root->left) - getHeight(root->right) == 2) {
        balanceLeft(root);
    } else if (getHeight(root->right) - getHeight(root->left) == 2) {
        balanceRight(root);
    }
    return root;
}
bool isFull(Tree root){
    if(!root)
        return true;
    if(!root->left && !root->right)
        return true;
    if(root->left && root->right)
        return isFull(root->left) && isFull(root->right);
    return false;
}
bool isComplete(Tree root){
    if(!root)
        return true;
    queue<Tree> q;
    q.push(root);
    bool flag = false;
    while(!q.empty()){
        Tree node = q.front();
        q.pop();
        if(node->left){
            if(flag)
                return false;
            q.push(node->left);
        } else {
            flag = true;
        }
        if(node->right){
            if(flag)
                return false;
            q.push(node->right);
        } else {
            flag = true;
        }
    }
    return true;
}
bool isPerfect(Tree root){
    if(!root)
        return true;
    int depth = 0;
    Tree temp = root;
    while(temp){
        depth++;
        temp = temp->left;
    }
    queue<pair<Tree, int>> q;
    q.push({root, 1});
    while(!q.empty()){
        pair<Tree,int> p = q.front();
        Tree node = p.first;
        int level = p.second;
        q.pop();
        if(!node->left && !node->right){
            if(level != depth)
                return false;
        } else if(node->left && node->right){
            q.push({node->left, level + 1});
            q.push({node->right, level + 1});
        } else {
            return false;
        }
    }
    return true;
}
void NLR(Tree root){
    if(root){
        cout << root->key << " ";
        NLR(root->left);
        NLR(root->right);
    }
}
void LNR(Tree root){
    if(root){
        LNR(root->left);
        cout << root->key << " ";
        LNR(root->right);
    }
}
void LRN(Tree root){
    if(root){
        LRN(root->left);
        LRN(root->right);
        cout << root->key << " ";
    }
}
void levelOrder(Tree root){
    if(!root)
        return;
    queue<Tree> q;
    q.push(root);
    while(!q.empty()){
        Tree node = q.front();
        cout << node->key << " ";
        q.pop();
        if(node->left)
            q.push(node->left);
        if(node->right)
            q.push(node->right);
    }
}
bool findNode(Tree root, int val) {
    if (!root) return false;
    if (root->key == val) return true;
    if (val < root->key) return findNode(root->left, val);
    return findNode(root->right, val);
}
int findCommonAncestor(Tree root, int x, int y){
    if (!root) return -1;
    if (!findNode(root, x) || !findNode(root, y)) return -1;
    if (x < root->key && y < root->key)
        return findCommonAncestor(root->left, x, y);
    if (x > root->key && y > root->key)
        return findCommonAncestor(root->right, x, y);
    return root->key;
}
void printSpecialNodes(Tree root){
    if(!root)
        return;
    printSpecialNodes(root->left);
    if(root->left && root->right){
        if(root->right->key % root->left->key == 0){
            cout << root->key << " ";
        }
    }
    printSpecialNodes(root->right);
}
int main(){
    Tree root = nullptr;
     int a[] = {10, 6, 23, 3, 8, 12, 36, 1, 4, 7, 9, 11, 22, 30, 60};
    for (int i = 0; i < sizeof(a)/4; ++i) {
        root = insertNode(root, a[i]);
    }
    deleteNode(root, 36);
    deleteNode(root, 12);
    deleteNode(root, 23);
    deleteNode(root, 22);
    cout << "Is the tree complete: " << isComplete(root) << endl;
    cout << "Is the tree full: " << isFull(root) << endl;
    cout << "Is the tree perfect: " << isPerfect(root) << endl;
    cout << "Ancestry of 1 and 4: " << findCommonAncestor(root, 1, 4) << endl;
    cout << "Ancestry of 1 and 7: " << findCommonAncestor(root, 1, 7) << endl;
    cout << "Ancestry of 1 and 13: " << findCommonAncestor(root, 1, 13) << endl;
    cout << "Ancestry of 1 and 11: " << findCommonAncestor(root, 1, 11) << endl;
    cout << "Ancestry of 1 and 12: " << findCommonAncestor(root, 1, 12) << endl;
    cout << "Special nodes: ";

    printSpecialNodes(root);
    cout << endl;
    cout << "Pre-order traversal (NLR): ";
    NLR(root);
    cout << endl;
    cout << "In-order traversal (LNR): ";
    LNR(root);
    cout << endl;
    cout << "Post-order traversal (LRN): ";
    LRN(root);
    cout << endl;
    cout << "Level-order traversal: ";
    levelOrder(root);
    cout << endl;
    
}
   