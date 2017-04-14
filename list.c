//All the appropriate include statements.
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "list.h"

void terminate(const char *message)
{
    fprintf(stderr,"%s\n", message);
    exit(EXIT_FAILURE);
}

//A function that creates and returns a queue. It creates 2 node pointers that
//keep track of the head and the tail of the queue.
struct token_list *create_list()
{
    struct token_list *list = malloc(sizeof(struct token_list));
    if (list == NULL)
    {
        terminate("Error in create_list: list could not be created.");
    }
    list->head = NULL;
    list->tail = NULL;
    list->number_of_tokens = 0;
    return list;
}

//A function that empties the queue and frees its memeory.
void destroy_list(struct token_list *list)
{
    empty_list(list);
    free(list);
}

//A function that empties the queue by dequeueing every node from the head.
void empty_list(struct token_list *list)
{
    while (!is_empty(list))
    {
        remove_token(list);
    }
}

//A function that returns true if the queue is empty and fasle if it is not.
bool is_empty(struct token_list *list)
{
    if(list->head == NULL && list->tail == NULL)
    {
        return true;
    }
    else
    {
        return false;
    }
    
}

/*
 * A function that takes an argument of the data that needs to be enqueued and
 * enqueues that data on to the tail of the queue. A detailed explanation of the 
 * pointer manipulation is outlined bellow:
 * 
 * 1) A new node is created (if there isn't room in RAM then return an error).
 * 2) Store the inputted data into the new node.
 * 3) Set the next pointer in the new node to NULL.
 * 4) If this is the first node being added to the queue then set the pointers of * both the head and tail to the new node.
 * 5) Set the next pointer in the current tail to the new node.
 * 6) Set the current tail to the new node.
 */
void add_token(struct token_list *list, char *token, int token_size, bool in_quotes)
{     
    struct node *new_node = malloc(sizeof(struct node));
    if (new_node == NULL)
    {
        terminate("Error in add: node cannot be created");
    }

    new_node->token = calloc(token_size + 1, sizeof(char));
    if (new_node->token == NULL)
    {
        terminate("Error in add: token can not be alloced");
    }
    
    strcpy(new_node->token,token);
    new_node->token_size = token_size;

    // creepy line of code coming up (dont know if this is a hard code)
    if( in_quotes == false && (new_node->token[0] == '<' || new_node->token[0] == '>'))
    {
        new_node->is_special = true;
    }
 
    new_node->next = NULL;

    if(list->head == NULL && list->tail == NULL)
    {
        list->head = list->tail = new_node;
        return;   
    }
    
    list->tail->next = new_node;
    list->tail = new_node;
    list->number_of_tokens++;
}

/*
 * A function that returns the data that it dequeued from the queue. A detailed
 * explanation of the pointer manipulation is outlined bellow:
 *
 * 1) A temporary node is crated to replace the head node. The data from the 
 * head node is copied into this temporary node.
 * 2) If the queue is empty return an error.
 * 3) If there is only one node in the queue (the head = the tail) set both the 
 * head and the tail equal to NULL
 * 4) Otherwise set the head to point at the node that is to the right of the
 * current head.
 * 5) Free the memory of the old head.
 * 6) Return the data previously saved in the old top. 
 */
void remove_token(struct token_list *list)
{
    struct node *old_head = list->head;

    if (is_empty(list))
    {
        terminate("Error in remove: list is already empty.");
    }
    
    if(list->head == list->tail)
    {
       list->head = list->tail = NULL;
    }
    else
    {
       list->head = list->head->next;
    }
    
    free(old_head->token);
    free(old_head);

    list->number_of_tokens--;
}

