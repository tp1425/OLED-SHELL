//all the appropiate includes
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

//The template for the queue data type.
struct token_list
{
    int number_of_tokens;
    struct node *head;
    struct node *tail;
};

//The template for the node of the linked list.
struct node
{
    char *token;
    int token_size;
    bool is_special;
    struct node *next;
};

void terminate(const char *message);

struct token_list *create_list();
void destroy_list(struct token_list *list);
void empty_list(struct token_list *list);
bool is_empty(struct token_list *list);
void add_token(struct token_list *list, char *token, int token_size, bool in_quotes);
void remove_token(struct token_list *list);

// comment
