#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#define MAX_key 256 // maximum size of key allowed in bytes
#define TABLE_SIZE 10
//#define DELETED_NODE (data*)(0xFFFFFFFFFFFFFFFFUL)


//separate chaining

// creating the struct of type data
typedef struct data{
    char key[MAX_key];
    int value;
    struct data *next;
} data;

data *hash_table[TABLE_SIZE]; // an array of pointers of type struct data [the array size is 10]

// The function generates the hash value for the given input
unsigned int hash(char *key){
    int length = strnlen(key, MAX_key); // get the length of the input string
    unsigned int hash_value = 0;
    for (int i = 0; i < length; i++){ // loop through every character of the string
        hash_value += key[i];  // adding the value of each character to the hash_value
        // the added hash_values are multiplied by the value of the character and mod by the size of the hash table
        hash_value = (hash_value * key[i]) % TABLE_SIZE;
    }
    return hash_value;
}

// The function initialize the hash table
void initHashTable(){
    for (int i = 0; i < TABLE_SIZE; i++){   // the loop will run until the defined table size is reach
        hash_table[i] = NULL; // each index of the array is assigned by null
    }
}

// This function prints the key, value pairs to the console
void print_table(){
    printf("     Element\t\tKey\t\tValue\n");
    for (int i=0; i < TABLE_SIZE; i++){ // the loop goes through the table
        if(hash_table[i] == NULL){  // if the element is empty
            printf("\t%i\t\t---\n", i);
        }else{ // prints the key, value pair
            printf("\t%i\t", i);
            data *temp = hash_table[i];
            while (temp != NULL){
                printf("\t%s -", temp->key);
                printf("\t%d\n", temp->value);
                temp = temp->next;
            }
            printf("\n");
        }
    }
}

// This function insert elements into the hash table
bool insert(data *p){
    if(p == NULL) return NULL; // if the given element is null
    int index = hash(p->key); // the hash value is returned by the hash_function for the given string of the element
    p->next = hash_table[index];
    hash_table[index] = p;
    return true;
}

// this function find an element by its key value
data *search(char *key){
    int index = hash(key); // generate the hash value based on the key
    data *temp = hash_table[index];
    while(temp != NULL && strncmp(temp->key, key, MAX_key) != 0){
        temp = temp->next;
    }
    return temp;
}

// The function deletes an element based on the key value
data *deleteItem(char *key){
    int index = hash(key); // generate the hash value
    data *temp = hash_table[index];
    data *previous = NULL;
    while(temp != NULL && strncmp(temp->key, key, MAX_key) != 0){
        previous = temp;
        temp = temp->next;
    }
    if(temp == NULL) return NULL;
    if(previous == NULL){
        // delete the head
        hash_table[index] = temp->next;
    }else{
        previous->next = temp->next;
    }
    return temp;
}

int main()
{
    initHashTable();

    data laptop = {.key = "laptop", .value = 2000};
    data phone = {.key = "phone", .value = 1000};
    data tab = {.key = "tab", .value = 750};
    data desktop = {.key = "desktop", .value = 2500};

    insert(&laptop);
    insert(&phone);
    insert(&tab);
    insert(&desktop);

    deleteItem("laptop");


    data *lookupkey = search("laptop");
    if(lookupkey == NULL){
        printf("\ndata not found!\n");
    }else{
        printf("Found %s.\n", lookupkey->key);
    }

    print_table();

    printf("%u", hash("Bill"));

    return 0;
}
