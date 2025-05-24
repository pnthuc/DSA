#include <iostream>
#include <queue>
using namespace std;
struct Node{
    int key;
    Node* left;
    Node* right;
};
typedef Node* Tree;
Tree newNode(int data){
    return new Node{data, nullptr, nullptr};
}
Tree insert(Tree root, int data){
    if(!root)
        return newNode(data);
    if(data < root->key)
        root->left = insert(root->left, data);
    else if(data > root->key)
        root->right = insert(root->right, data);
    return root;
}
Tree search(Tree root, int data){
    if(!root or root->key == data)
        return root;
    if(data < root->key)
        return search(root->left, data);
    else 
        return search(root->right, data);
}
void searchStandFor(Tree &p, Tree& q){
    if(q->left){
        searchStandFor(p, q->left);
    }else{
        p->key = q->key;
        p = q;
        q = q->right;
    }
}
Tree deleteNode(Tree root, int data){
    if(!root)
        return root;
    if(data < root->key)
        root->left = deleteNode(root->left, data);
    else if(data > root->key)
        root->right = deleteNode(root->right, data);
    else{
        Node* p = root;
        if(!root->left)
            root = root->right;
        else if(!root->right)
            root = root->left;
        else{
            searchStandFor(p, root->right);
        }
        delete p;
    }
    return root;
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
int height(Tree root){
    if(!root)
        return 0;
    return max(height(root->left), height(root->right)) + 1;
}
int countNodes(Tree root){
    if(!root)
        return 0;
    return countNodes(root->left) + countNodes(root->right) + 1;
}
int countInternal(Tree root){
    if(!root)
        return 0;
    if(!root->left && !root->right)
        return 0;
    return countInternal(root->left) + countInternal(root->right) + 1;
}
int countLeaf(Tree root){
    if(!root)
        return 0;
    if(!root->left && !root->right)
        return 1;
    return countLeaf(root->left) + countLeaf(root->right);
}
int sumNode(Tree root){
    if(!root)
        return 0;
    return sumNode(root->left) + sumNode(root->right) + root->key;
}
int countLess(Tree root, int data){
    if(!root)
        return 0;
    if(data < root->key)
        return countLess(root->left, data);
    else if(data > root->key)
        return countLess(root->right, data) + countNodes(root->left) + 1;
    else
        return countNodes(root->left);
}
int countGreater(Tree root, int data){
    if(!root)
        return 0;
    if(data < root->key)
        return countGreater(root->left, data) + countNodes(root->right) + 1;
    else if(data > root->key)
        return countGreater(root->right, data);
    else
        return countNodes(root->right);
}   
int main(){
    Tree root = nullptr;
    root = insert(root, 8);
    root = insert(root, 6);
    root = insert(root, 5);
    root = insert(root, 7);
    root = insert(root, 10);
    root = insert(root, 9);
    cout << "NLR: ";
    NLR(root);
    cout << endl;
    cout << "LNR: ";
    LNR(root);
    cout << endl;
    cout << "LRN: ";
    LRN(root);
    cout << endl;
    root = deleteNode(root, 8);
    root = deleteNode(root, 8);
    cout << "Level Order after delete 8: ";
    levelOrder(root);
    cout << endl;
    cout << "Height: " << height(root) << endl;
    cout << "countNodes: " << countNodes(root) << endl;
    cout << "countInternal: " << countInternal(root) << endl;
    cout << "countLeaf: " << countLeaf(root) << endl;
    cout << "sumNode: " << sumNode(root) << endl;
    cout << "countLess: " << countLess(root, 7) << endl;
    cout << "countGreater: " << countGreater(root, 7) << endl;
    return 0;
}