/* COP 3502C Programming Assignment 6
This program is written by: Zainab Syed*/

#include <stdio.h>
#include <stdlib.h>
#define MAX 2000000

typedef struct trie {
    int frequency;
    int sum_freq;
    int cur_max_freq;
    struct trie* next[26];
} trie;

// void printAll(trie* tree, char cur[]);
// trie* init_trie();
// void insert(trie* tree, char word[], int k, int freq) ;
// int search(trie* tree, char word[], int k) ;

void printAll(trie* tree, char cur[]) {

    // Stop!
    if (tree == NULL) return;

    // Print this node, if it's a word.
    if (tree->frequency)
        printf("current: %s frequency: %d sum_feq: %d cur_max: %d\n", cur, tree->frequency, tree->sum_freq, tree->cur_max_freq);

    // Safer if we store this.
    int len = strlen(cur);

    // Recursively print all words in each subtree,
    // in alpha order.
    int i;
    for (i=0; i<26; i++) {
        cur[len] = (char)('a'+i);
        cur[len+1] = '\0';
        printAll(tree->next[i], cur);
    }
}

trie* init_trie() {
    // printf("Initializing trie\n");
    trie* temp = malloc(sizeof(trie));
    if (temp == NULL) {
        printf("FAILED\n");
        return NULL;
    }
    //temp->count = 0;
    temp->cur_max_freq = 0;
    temp->frequency = 0;
    temp->sum_freq = 0;
    for (int i = 0; i < 26; i++) {
        temp->next[i] = NULL;
    }
    return temp;
}

void insert(trie* tree, char word[], int k, int freq) {
    if(k == strlen(word)) {
        tree->frequency += freq;
        tree->sum_freq+=freq;
        //printf("FINAL: %d: max:%d sum:%d freq: %d\n",k, tree->cur_max_freq, tree->sum_freq, tree->frequency);
        return;
    }
    tree->sum_freq+=freq;
    //printf("the sum freq at %c %d: %d\n",word[k], k, tree->sum_freq);

    int nextIndex = word[k] -'a';
    if(tree->next[nextIndex] == NULL) {
        tree->next[nextIndex] = init_trie();
    }
    
    if((tree->next[nextIndex]->sum_freq + freq) > tree->cur_max_freq) {
        //tree->cur_max_freq += freq;
        tree->cur_max_freq = tree->next[nextIndex]->sum_freq + freq;
        //printf("the cur max freq at %d: %d\n",k, tree->cur_max_freq);
    }
    
    insert(tree->next[nextIndex], word, k+1, freq);
}

int query(trie* tree, char word[], int k) {
    if (word[k] == '\0'){ // (k == strlen(word)) {
        //printf("END OF WORD REACHED %s\n", word);
        int len = strlen(word);
        int result = 0;
        for (int i = 0; i < 26; i++) {
            int nextIndex = word[i] -'a';
            //printf("at: %d next index is: %d \n", i, nextIndex);
            if(tree->next[i] != NULL) {
                //printf("not null at: %d\n", i);
                //printf("cur_max: %d next max: %d\n", tree->cur_max_freq, tree->next[i]->sum_freq);
                if(tree->cur_max_freq == tree->next[i]->sum_freq) {
                    word[len] = (char)('a'+ i);
                    printf("%c", word[len]);
                    result = 1;
                }
            }
        }
        
        return result;
    }
    int nextIndex = word[k] -'a';
    
    // not in tree
    if(tree->next[nextIndex] == NULL) {
        //printf("not in tree\n");
        return 0;
    }
    //printf("not end of word at: %d \n", k);
    return query(tree->next[nextIndex], word, k+1);
}

void freeTree(struct trie* tree) {

    int i;
    for (i=0; i<26; i++)
        if (tree->next[i] != NULL)
            freeTree(tree->next[i]);

    free(tree);
}

int main() {
    
    int num_commands = 0;
    int command = 0;

    //printf("in main %d %d\n", num_commands, command);

    trie* root = init_trie();
    if(!root) {
        printf("FAILED init\n");
    }

    //printf("Enter num of commands\n");
    scanf("%d", &num_commands);

    for(int i = 0; i < num_commands; i++) {
        //printf("enter 1 or 2\n");
        scanf("%d", &command);
        if(command == 1) {
            char word[MAX+1];
            int frequency;
            //printf("Enter word + frequency\n");
            scanf("%s %d", word, &frequency);
            insert(root, word, 0, frequency);
        }
        else if(command == 2) {
            char word[MAX+1];
            scanf("%s", word);
            int result = query(root, word, 0);
            if(result == 0) {
                printf("unrecognized prefix\n");
            }
            else{
                printf("\n");
            }
        }
        // char word[MAX+1];
        // word[0] = '\0';
        // printAll(root, word);
    } 
    freeTree(root);
    return 0;
}