#include <iostream>
#include <queue>
using namespace std;
const int MAX = 5;

struct Node
{
    int keys[MAX];
    Node* children[MAX+1];
    int numKeys;
    bool isLeaf;
};
Node* newNode(bool isLeaf){
    return new Node{{-1, -1, -1}, {0, 0, 0, 0}, 0, isLeaf};
}
Node *search(Node* root, int data){
    if(!root)
        return NULL;
    int i = 0;
    while(i < root->numKeys && data > root->keys[i])
        i++;
    if(i < root->numKeys && data == root->keys[i])
        return root;
    return search(root->children[i], data);
}
Node* merge(Node* root, Node* temp, int index){
    int key[MAX + 1] = {0};
    Node* child[MAX + 2] = {0};
    for(int i = 0; i < root->numKeys; i++)
        key[i] = root->keys[i], 
        child[i] = root->children[i];
    child[root->numKeys] = root->children[root->numKeys];
    for(int j = root->numKeys; j > index; j--)
        key[j] = key[j - 1], 
        child[j + 1] = child[j];
    key[index] = temp->keys[0];
    child[index] = temp->children[0];
    child[index+1] = temp->children[1];
    delete temp;
    root->numKeys++;
    if(root->numKeys > MAX){
        Node* newRoot = newNode(false);
        Node* newRoot_left = newNode(root->isLeaf);
        Node* newRoot_right = newNode(root->isLeaf);      
        newRoot_left->numKeys = MAX/2;
        for(int i = 0; i < MAX/2; i ++)
            newRoot_left->keys[i] = key[i], 
            newRoot_left->children[i] = child[i];
        newRoot_left->children[MAX/2] = child[MAX/2];
        newRoot->numKeys = 1, newRoot->keys[0] = key[MAX/2];
        newRoot->children[0] = newRoot_left;
        newRoot->children[1] = newRoot_right;
        newRoot_right->numKeys = MAX - MAX/2;
        for(int i = MAX/2 + 1; i <= MAX; i++)
            newRoot_right->keys[i - MAX/2 - 1] = key[i], 
            newRoot_right->children[i - MAX/2 - 1] = child[i];
        newRoot_right->children[MAX - MAX/2] = child[MAX + 1];
        newRoot->children[2] = newRoot;
        delete root;
        return newRoot;
    }else{
        for(int i = 0; i < root->numKeys; i++)
            root->keys[i] = key[i], 
            root->children[i] = child[i];
        root->children[root->numKeys] = child[root->numKeys];
        return root;
    }
}
Node *insert(Node* root, int data){
    if(!root){
        root = newNode(true);
        root->keys[0] = data;
        root->numKeys = 1;
        root->children[2] = root;
        return root;
    }
    int i = 0;
    while(i < root->numKeys && data > root->keys[i])
        i++;
    if(i < root->numKeys && data == root->keys[i])
        return root;
    Node* newRoot = insert(root->children[i], data);
    root->children[i] = newRoot;
    if(newRoot == newRoot->children[2])
        return newRoot = merge(root, newRoot, i);   
    return root;
}
int cnt = 0;
void levelOrder(Node* root){
    if (!root) return;
    queue<Node*> q;
    q.push(root);
    while (!q.empty()) {
        int n = q.size();
        for (int i = 0; i < n; ++i) {
            Node* node = q.front(); q.pop();
            cout << "[";
            for (int j = 0; j < node->numKeys; ++j) {
                cout << node->keys[j];
                if (j < node->numKeys - 1) cout << " ";
            }
            cout << "] ";
            if (!node->isLeaf) {
                for (int j = 0; j <= node->numKeys; ++j) {
                    if (node->children[j])
                        q.push(node->children[j]);
                }
            }
        }
        cout << endl;
    }
}
Node* adjust(Node* root, Node* temp, int index){
    Node* left, *right;
    left = right = NULL;
    if(index > 0)
        left = root->children[index-1];
    if(index < root->numKeys)
        right = root->children[index+1];
    bool isLeft = false, isRight = false;
    if(left)
        isLeft = left->numKeys > MAX / 2;
    if(right)
        isRight = right->numKeys > MAX / 2;
    if(isLeft){ // ae trái
        temp->numKeys++;
        temp->children[temp->numKeys] = temp->children[temp->numKeys - 1];
        for(int j = temp->numKeys - 1; j > 0; j--)
            temp->keys[j] = temp->keys[j-1], 
            temp->children[j] = temp->children[j-1];
        temp->keys[0] = root->keys[index - 1];
        temp->children[0] = left->children[left->numKeys];
        root->keys[index - 1] = left->keys[left->numKeys - 1];
        left->numKeys--;
    }
    else if(isRight){ // ae phải
        temp->numKeys++;
        temp->children[temp->numKeys] = right->children[0];
        temp->keys[temp->numKeys - 1] = root->keys[index];
        root->keys[index] = right->keys[0];
        for(int j = 0; j < right->numKeys - 1; j++)
            right->keys[j] = right->keys[j+1], 
            right->children[j] = right->children[j+1];
        right->children[right->numKeys - 1] = right->children[right->numKeys];
        right->numKeys--;
    }
    else{ // không có ae, bú của thằng cha
        Node* newRoot = newNode(false);
        if(index > 0){
            newRoot->numKeys = 1;
            newRoot->keys[0] = root->keys[index - 1];
            newRoot->children[0] = left->children[left->numKeys];
            newRoot->children[1] = temp->children[0];
            for(int i = left->numKeys; i < left->numKeys + temp->numKeys; i++)
                left->keys[i] = temp->keys[i - left->numKeys], 
                left->children[i] = temp->children[i - left->numKeys];
            left->children[left->numKeys + temp->numKeys] = temp->children[temp->numKeys];
            left->numKeys += temp->numKeys;
            left->children[left->numKeys - temp->numKeys] = newRoot;
            newRoot = merge(left, newRoot, left->numKeys - temp->numKeys);
            for(int j = index - 1; j < root->numKeys - 1; j++)
                root->keys[j] = root->keys[j + 1], 
                root->children[j] = root->children[j + 1];
            root->children[root->numKeys - 1] = root->children[root->numKeys];
            root->numKeys--;
            root->children[index - 1] = newRoot;
            delete temp;
        }else{
            newRoot->numKeys = 1;
            newRoot->keys[0] = root->keys[index];
            newRoot->children[0] = temp->children[temp->numKeys];
            newRoot->children[1] = right->children[0];
            for(int i = temp->numKeys; i < temp->numKeys + right->numKeys; i++)
                temp->keys[i] = right->keys[i - temp->numKeys], 
                temp->children[i] = right->children[i - temp->numKeys];
            temp->children[temp->numKeys + right->numKeys] = right->children[right->numKeys];
            temp->numKeys += right->numKeys;
            temp->children[temp->numKeys - right->numKeys] = newRoot;
            newRoot = merge(temp, newRoot, temp->numKeys - right->numKeys);
            for(int j = index; j < root->numKeys - 1; j++)
                root->keys[j] = root->keys[j + 1], 
                root->children[j] = root->children[j + 1];
            root->children[root->numKeys - 1] = root->children[root->numKeys];
            root->numKeys--;
            root->children[index] = newRoot;
            delete right;
        }
    }
    return root;
}
Node *deleteNode(Node* root, int data){
    if(!root)
        return NULL;
    int i = 0;
    while(i < root->numKeys && data > root->keys[i])
        i++;
    if(i < root->numKeys && data == root->keys[i]){
        if(root->isLeaf){
            root->numKeys--;
            for(int j = i; j < root->numKeys; j ++)
                root->keys[j] = root->keys[j + 1];
            return root;
        }else{
            Node* temp = root->children[i + 1];
            while(temp->children[0])
                temp = temp->children[0];
            root->keys[i ++] = temp->keys[0];        
            data = temp->keys[0];
        }
    }
    Node* newRoot = deleteNode(root->children[i], data);
    root->children[i] = newRoot;
    if(newRoot && newRoot->numKeys < MAX/2)
        return newRoot = adjust(root, newRoot, i);;
    return root;
}

void NLR(Node* root){
    if(!root) return;
    cout << "[";
    for(int i = 0; i < root->numKeys; i++)
        cout << root->keys[i] << " ";
    cout << "] ";
    if(!root->isLeaf)
        for(int i = 0; i <= root->numKeys; i++)
            NLR(root->children[i]);
}
void LNR(Node* root){
    if(!root) return;
    if(!root->isLeaf)
        for(int i = 0; i <= root->numKeys; i++)
            LNR(root->children[i]);
    cout << "[";
    for(int i = 0; i < root->numKeys; i++)
        cout << root->keys[i] << " ";
    cout << "] ";
}
void LRN(Node* root){
    if(!root) return;
    if(!root->isLeaf)
        for(int i = 0; i <= root->numKeys; i++)
            LRN(root->children[i]);
    cout << "[";
    for(int i = 0; i < root->numKeys; i++)
        cout << root->keys[i] << " ";
    cout << "] ";
}
int main(){
    Node* tree = NULL;
    int data[100] = {
        10, 20, 5, 6, 12, 30, 7, 17, 15, 25, 35, 40, 50, 60, 70, 80,
        1, 2, 3, 4, 8, 9, 11, 13, 14, 16, 18, 19, 21, 22, 23, 24,
        26, 27, 28, 29, 31, 32, 33, 34, 36, 37, 38, 39, 41, 42, 43, 44,
        45, 46, 47, 48, 49, 51, 52, 53, 54, 55, 56, 57, 58, 59, 61, 62,
        63, 64, 65, 66, 67, 68, 69, 71, 72, 73, 74, 75, 76, 77, 78, 79,
        81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96,
        97, 98, 99, 100
    };
    for(int i = 0; i < sizeof(data)/sizeof(data[0]); i++)
        tree = insert(tree, data[i]);
    for(int i = 0; i < sizeof(data)/sizeof(data[0]) / 2; i++){
        tree = deleteNode(tree, data[i]);
        while(tree && tree->numKeys == 0)
            tree = tree->children[0];
    }
    cout << "Final tree after deletions:" << endl;
    levelOrder(tree);
}