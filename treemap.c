#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) {

    TreeMap* newTree = (TreeMap *) malloc(sizeof(TreeMap));
    newTree->root = NULL;
    newTree->current = NULL;
    newTree->lower_than = lower_than;
    return newTree;
}


void insertTreeMap(TreeMap * tree, void* key, void * value) {
    
    Pair* pair = searchTreeMap(tree,key);
    if(pair != NULL) {
        return;
    } 

    TreeNode* insert = createTreeNode(key,value);

    if(tree->root == NULL){
        tree->root = insert;
        tree->current = insert;
        return;
    } 

    TreeNode* current = tree->root;
    TreeNode* parent = NULL;
    while(current != NULL){
        if(tree->lower_than(key,current->pair->key)){
            parent = current;
            current = current->left;
        } else{
            parent = current;
            current = current->right;
        }
        if(current == NULL) break;
        
    }

    insert->parent = parent;
    if(tree->lower_than(key,parent->pair->key)){
        parent->left = insert;
    } else {
        parent->right = insert;
    }

    tree->current = insert;

    
}

TreeNode * minimum(TreeNode * x){
    if (x == NULL) return NULL;
    while(x->left != NULL)
        x = x->left;
    
    return x;
}


void removeNode(TreeMap * tree, TreeNode* node) {
    
    if (node->left == NULL && node->right == NULL) {
        if (node->parent == NULL)
            tree->root = NULL;
         else {
            if (node == node->parent->left)
                node->parent->left = NULL;
            else
                node->parent->right = NULL;
         }
        free(node->pair);
        free(node);
    }
    else {
        if (node->left == NULL || node->right == NULL)
        {
            TreeNode* child;
            if(node->left == NULL){
                child = node->right;
            }
            else {
                child = node->left;
            }

            if (node->parent == NULL)
                tree->root = child;
            else
                if (node == node->parent->left)
                    node->parent->left = child;
                else
                    node->parent->right = child;

            child->parent = node->parent;
            free(node->pair);
            free(node);
        }
        else {
            TreeNode* next = minimum(node->right);
            node->pair->key = next->pair->key;
            node->pair->value = next->pair->value;
            removeNode(tree,next);
        }
    }
    
}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}


Pair * searchTreeMap(TreeMap * tree, void* key) {
    TreeNode* nodeTree = tree->root;

    while(nodeTree != NULL){
        
        if(tree->lower_than(key,nodeTree->pair->key) == 1){
            nodeTree = nodeTree->left;
        } else if (tree->lower_than(nodeTree->pair->key,key) == 1){
            nodeTree = nodeTree->right;
        } else{
            tree->current = nodeTree;
            return nodeTree->pair;
        }
    }
    
    return NULL;
}


Pair * upperBound(TreeMap * tree, void* key) {
    TreeNode* current = tree->root;
    TreeNode* ub_node = NULL;

    while(current != NULL){
        if (tree->lower_than(current->pair,key))
            current = current->right;
        
    }
}

Pair * firstTreeMap(TreeMap * tree) {
    if(tree->root == NULL) return NULL;
    TreeNode* min = minimum(tree->root);
    tree->current = min;
    return (min->pair);
}

Pair * nextTreeMap(TreeMap * tree) {

    if (tree->current == NULL) return NULL;
    
    if (tree->current->right != NULL){
        tree->current = minimum(tree->current->right);
        return (tree->current->pair);
    }

    TreeNode* parent = tree->current->parent;
    while(parent != NULL && tree->current == parent->right)
    {
        tree->current = parent;
        parent = parent->parent;
    }

    tree->current = parent;

    if(parent != NULL) return parent->pair;

    return NULL;
    
}
