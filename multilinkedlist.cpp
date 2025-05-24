#include <iostream>
#include <stdio.h>
#include <vector>
using namespace std;
typedef struct leader* lref;
typedef struct trailer* tref;

struct trailer {
    lref id;
    tref next;
};

struct leader{
    int key;
    int cnt;
    lref next;
    tref trail;
};

lref addList(lref head, lref& tail, int w, int& z) {
    lref h = head; tail->key = w;
    while (h->key != w) h = h->next;
    if (h == tail) {
        tail = new leader(); z++;
        h->cnt = 0; h->trail = 0; h->next = tail;
    }
    return h;
}
// void free_leader(lref head) {
//     if(head -> cnt == -1) return;
//     cout << head -> key << ' ';
//     tref tmp_t = head->trail;
//     head -> cnt = -1;
//     for(tref t = tmp_t; t;){
//         tref tmp = t;
//         t = t->next;
//         free_leader(tmp -> id);
//         delete tmp;
//     }
//     delete head;
// }
int main() {    
    lref head = new leader();
    lref tail = head; int z = 0;
    FILE* f = fopen("input.txt", "r");
    FILE *o = fopen("output.txt", "w");
    int x, y; char trash;
    while(fscanf(f, "%c%d%c%d%c", &trash, &x, &trash, &y, &trash) == 5) {
        lref p = addList(head, tail, x, z);
        lref q = addList(head, tail, y, z);
        tref t = new trailer();
        t->id = q;
        t->next = p->trail;
        p->trail = t;
        q->cnt++;
    }
    fclose(f);
    lref save_head = head;
    lref p = head, q; head = NULL;
    while (p != tail) {
        q = p; p = p->next;
        if (q->cnt == 0) 
            q->next = head, head = q;
    }
    q = head; tref t = 0;
    lref tail_topo = NULL, head_topo = NULL;
    while (q) {
        lref k = q;
        z--;
        t = q->trail;
        q = q->next;
        while (t) {
            p = t->id;
            p->cnt--;
            if (p->cnt == 0) {
                p->next = q;
                q = p;
            }
            t = t->next;
        }
        if(!head_topo) {
            head_topo = tail_topo = k;
            tail_topo->next = NULL;
        }else{
            tail_topo->next = k;
            tail_topo = k;
            tail_topo->next = NULL;
        }
    }
    if(z){
        // if(!head){
        //     for(lref i = save_head; i != tail; ) {
        //         for(tref t = i -> trail; i; ) {
        //             tref tmp = t;
        //             t = t->next;
        //             delete tmp;
        //         }
        //         lref tmp = i;
        //         cout << tmp->key << " ";
        //         i = i->next;
        //         delete tmp;
        //     }
        //     return cout << -1, 0;
        // }
        // for (lref p = head; p != tail; ) {
        //     lref tmp = p;
        //     temp -> cnt = -1;
        //     cout << tmp->key << " ";
        //     p = p->next;
            
        //     for (tref t = tmp->trail; t; ) {
        //         tref tmp_t = t;
        //         free_leader(tmp_t->id);
        //         t = t->next;
        //         delete tmp_t;
        //     }

        //     delete tmp;
        // }
        for(lref i = head_topo; i; i = q) {
            for(tref t = i->trail; t; ) {
                tref tmp = t;
                t = t->next;
                delete tmp;
            }
            lref tmp = i;
            q = i -> next;
            delete tmp;
        }
        fprintf(o, "-1");
        return 0;
    }
    ////////////////////////////////////////////////////////////
    for(lref i = head_topo; i; i = q) {
        for(tref t = i->trail; t; ) {
            tref tmp = t;
            t = t -> next;
            delete tmp;
        }
        q = i -> next;
        fprintf(o, "%d ", i->key);
        delete i;
    }    
    fclose(o);
    return 0;
}
