#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stddef.h> // For size_t

// Definition of a node in the phonebook tree
typedef struct TNode {
    char* name;
    struct TNode* children[10];
} TNode;

// Definition of the phonebook
typedef struct TPhoneBook {
    TNode* root;
    size_t size;
} TPhoneBook;

// Recursive helper function to delete all nodes
static void deleteNode(TNode* node) {
    if (node == NULL) return;
    for (size_t i = 0; i < 10; i++) {
        if (node->children[i] != NULL) {
            deleteNode(node->children[i]);
        }
    }
    if (node->name != NULL) {
        free(node->name);
    }
    free(node);
}

// Function to clear the entire phonebook
void delBook(TPhoneBook* phonebook) {
    if (phonebook == NULL || phonebook->root == NULL) return;
    deleteNode(phonebook->root);
    phonebook->root = NULL;
    phonebook->size = 0;
}

// Check if a node contains any child branches
static bool hasChildren(const TNode* node) {
    for (size_t i = 0; i < 10; i++) {
        if (node->children[i] != NULL) return true;
    }
    return false;
}

// Add a phone prefix with an associated name
bool addPhone(TPhoneBook* phonebook, const char* phone, const char* name) {
    if (phonebook == NULL || phone == NULL || name == NULL || strspn(phone, "0123456789") != strlen(phone)) {
        return false;
    }
    if (phonebook->root == NULL) {
        phonebook->root = (TNode*)malloc(sizeof(TNode));
        memset(phonebook->root, 0, sizeof(TNode));
    }
    TNode* current = phonebook->root;
    for (size_t i = 0; phone[i] != '\0'; i++) {
        int digit = phone[i] - '0';
        if (current->children[digit] == NULL) {
            current->children[digit] = (TNode*)malloc(sizeof(TNode));
            memset(current->children[digit], 0, sizeof(TNode));
        }
        current = current->children[digit];
    }
    if (current->name != NULL) {
        free(current->name);
    }
    current->name = strdup(name);
    phonebook->size++;
    return true;
}

// Remove a phone prefix from the phonebook
bool delPhone(TPhoneBook* phonebook, const char* phone) {
    if (phonebook == NULL || phone == NULL || strspn(phone, "0123456789") != strlen(phone)) {
        return false;
    }
    TNode* current = phonebook->root;
    TNode* parent = NULL;
    int branchIndex = -1;
    for (size_t i = 0; phone[i] != '\0'; i++) {
        int digit = phone[i] - '0';
        if (current->children[digit] == NULL) {
            return false;
        }
        parent = current;
        current = current->children[digit];
        branchIndex = digit;
    }
    if (current->name != NULL) {
        free(current->name);
        current->name = NULL;
    }
    phonebook->size--;
    if (!hasChildren(current) && parent != NULL) {
        free(current);
        parent->children[branchIndex] = NULL;
    }
    return true;
}

// Retrieve the most specific name for a given phone number
const char* findPhone(const TPhoneBook* phonebook, const char* phone) {
    if (phonebook == NULL || phone == NULL || strspn(phone, "0123456789") != strlen(phone)) {
        return NULL;
    }
    const TNode* current = phonebook->root;
    const char* matchedName = NULL;
    for (size_t i = 0; phone[i] != '\0'; i++) {
        int digit = phone[i] - '0';
        if (current->children[digit] == NULL) {
            break;
        }
        current = current->children[digit];
        if (current->name != NULL) {
            matchedName = current->name;
        }
    }
    return matchedName;
}
