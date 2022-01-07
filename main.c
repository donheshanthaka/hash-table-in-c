#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#define MAX_NAME 256 // maximum size of name allowed in bytes
#define TABLE_SIZE 10
#define DELETED_NODE (person*)(0xFFFFFFFFFFFFFFFFUL)


// open addressing

// creating the  struct of type person
typedef struct {
    char name[MAX_NAME];
    int age;
} person;

person *hash_table[TABLE_SIZE]; // an array of pointers of type struct person [the array size is 10]

// The function generates the hash value for the given input
unsigned int hash(char *name){
    int length = strnlen(name, MAX_NAME); // get the length of the input string
    unsigned int hash_value = 0;
    for (int i = 0; i < length; i++){ // loop through every character of the string
        hash_value += name[i];  // adding the value of each character to the hash_value
        // the added hash_values are multiplied by the value of the character and mod by the size of the hash table
        hash_value = (hash_value * name[i]) % TABLE_SIZE;
    }
    return hash_value;
}

// The function initialize the hash table
void init_hash_table(){
    for (int i = 0; i < TABLE_SIZE; i++){   // the loop will run until the defined table size is reach
        hash_table[i] = NULL; // each index of the array is assigned by null
    }
}

// This function prints the key, value pairs to the console
void print_table(){
    for (int i=0; i < TABLE_SIZE; i++){ // the loop goes through the table
        if(hash_table[i] == NULL){  // if the element is empty
            printf("\t%i\t---\n", i);
        }else if (hash_table[i] == DELETED_NODE){ // if its a deleted node that was used previously
            printf("\t%i\t<deleted>\n", i);
        }else{ // prints the key, value pair
            printf("\t%i\t%s\n",i, hash_table[i]->name);
        }
    }
}

// This function insert elements into the hash table
bool hash_table_insert(person *p){
    if(p == NULL) return NULL; // if the given element is null
    int index = hash(p->name); // the hash value is returned by the hash_function for the given string of the element
    for (int i=0; i < TABLE_SIZE; i++){ // the loop will run upt to the size of the table
        /*
        since open addressing is used, the hash value is being incremented by 1 as the loop goes
        at the first iteration the value will be (hash_value + 0) % table_size, there for it will check the original hash position to see if it is
        null or a previous node but now that has been emptied, if either is true it will added the element there, if not
        it will check go to the second iteration (hash_value + 1) % table_size, so that it will iterate over the given size but wouldn't go above
        */
        int try = (i + index) % TABLE_SIZE;
        if(hash_table[try] == NULL || hash_table[try] == DELETED_NODE){
            hash_table[try] = p;
            return true;
        }
    }
    return false;
}

// this function find an element by its key value
person *hash_table_lookup(char *name){
    int index = hash(name); // generate the hash value based on the key
    for (int i = 0; i < TABLE_SIZE; i++){ // loop through the table
        int try = (index + i) % TABLE_SIZE; // the same algorithm is used as in the insert function
        // if the first hash value is null then its obvious that a key of similar hash value cannot be at another location, thereofore null is returned
        if (hash_table[try] == NULL){
            return false;
        }
        // if the selected hash_value index has been deleted, there is a possibility that more elements were added after, therefore the loop continues
        if(hash_table[try] == DELETED_NODE) continue;
        // if the selected key is matching then the pointer to that node is returned
        if(strncmp(hash_table[index]->name, name, TABLE_SIZE) == 0){
            return hash_table[try];
        }
    }
    return NULL;
}

// The function deletes an element based on the key value
person *hash_table_delete(char *name){
    int index = hash(name); // generate the hash value
    for (int  i = 0; i < TABLE_SIZE; i++){
        int try = (index + i) % TABLE_SIZE; // the same algorithm is used as in the insert function
        // if the first hash value is null then its obvious that a key of similar hash value cannot be at another location, thereofore null is returned
        if (hash_table[try] == NULL) return NULL;
        // if the selected hash_value index has been deleted, there is a possibility that more elements were added after, therefore the loop continues
        if (hash_table[try] == DELETED_NODE) continue;
        // if the selected key is matching then, the that node or index is assigned as a DELETED_NODE
        if(strncmp(hash_table[try]->name, name, TABLE_SIZE) == 0){
            person *tmp = hash_table[try];
            hash_table[try] = DELETED_NODE;
            return tmp;
        }
    }
    return NULL;
}

int main()
{
    init_hash_table();

    person jacob = {.name = "jacob", .age = 56};
    person mike = {.name = "jacob", .age = 56};
    person kate = {.name = "kate", .age = 26};
    person mpho = {.name = "Mpho", .age = 36};

    hash_table_insert(&jacob);
    hash_table_insert(&mike);
    hash_table_insert(&kate);
    hash_table_insert(&mpho);

    hash_table_delete("Mpho");
    hash_table_delete("jacob");
    hash_table_delete("jacob");

    person susan = {.name = "jacob", .age = 56};
    hash_table_insert(&susan);

    person *lookupName = hash_table_lookup("Mpho");
    if(lookupName == NULL){
        printf("\nPerson not found!\n");
    }else{
        printf("Found %s.\n", lookupName->name);
    }

    print_table();

    printf("%u", hash("Bill"));

    return 0;
}
