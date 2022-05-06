/*
Project 4
Vicki Young
student ID: 20591845
start date: 2021.11.30
end date: 2021.12.13 at 2359
CS 221-02
*/

/* doubly_linked_list.c
 *
 * Implement a sorted linked list of strings with operations Insert 
 * in alphabetical order, Print, Member, Delete, Free_list.
 * The list nodes are doubly linked.
 * 
 * Input:    Single character lower case letters to indicate operations, 
 *           possibly followed by value needed by operation -- e.g. 'i'
 *           followed by "hello" to insert the string "hello" -- no 
 *           double or single quotes.
 *
 * Output:   Results of operations.
 *
 * Compile:  gcc -g -Wall -o doubly_linked_list doubly_linked_list.c
 *           
 * Run:      ./doubly_linked_list
 *
 */

/* You may not add any new header file */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Max 99 chars + '\0' */
#define STRING_MAX 100

struct list_node_s {
   char*  data;
   struct list_node_s* prev_p;
   struct list_node_s* next_p;
};

/* Pointers to the head and tail of the list */
struct list_s {
   struct list_node_s* h_p;
   struct list_node_s* t_p;
};

void Insert(struct list_s* list_p, char string[]);
void Print(struct list_s* list_p);
int  Member(struct list_s* list_p, char string[]);
void Delete(struct list_s* list_p, char string[]);
void Free_list(struct list_s* list_p);
char Get_command(void);
void Get_string(char string[]);
void Free_node(struct list_node_s* node_p);
struct list_node_s* Allocate_node(int size);
void Print_node(char title[], struct list_node_s* node_p);


/*-----------------------------------------------------------------*/
int main(void) {
   char          command;
   char          string[STRING_MAX];
   struct list_s list;  

   list.h_p = list.t_p = NULL;
      /* start with empty list */

   command = Get_command();
   while (command != 'q' && command != 'Q') {
      switch (command) {
         case 'i': 
         case 'I': 
            Get_string(string);
            Insert(&list, string);
            break;
         case 'p':
         case 'P':
            Print(&list);
            break;
         case 'm': 
         case 'M':
            Get_string(string);
            if (Member(&list, string))
               printf("%s is in the list\n", string);
            else
               printf("%s is not in the list\n", string);
            break;
         case 'd':
         case 'D':
            Get_string(string);
            Delete(&list, string);
            break;
         case 'f':
         case 'F':
            Free_list(&list);
            break;
         default:
            printf("There is no %c command\n", command);
            printf("Please try again\n");
      }
      command = Get_command();
   }
   Free_list(&list);

   return 0;
}  /* main */


/*-----------------------------------------------------------------*/
/* Function:   Allocate_node
 * Purpose:    Allocate storage for a list node
 * Input arg:  size = number of chars needed in data member (including
 *                storage for the terminating null)
 * Return val: Pointer to the new node
 */
struct list_node_s* Allocate_node(int size) {
   struct list_node_s* new_node = (struct list_node_s*) malloc(sizeof(struct list_node_s));

   if (new_node == NULL) {
      printf("Memory not properly allocated for new node.\n");
      return NULL;
   }

   (*new_node).data = (char*) calloc(size, sizeof(char));

   if ((*new_node).data == NULL) {
      printf("Memory not properly allocated for new node data.\n");
      return NULL;
   }

   (*new_node).prev_p = NULL;
   (*new_node).next_p = NULL;

   return new_node;
}  /* Allocate_node */


/*-----------------------------------------------------------------*/
/* Function:   Insert
 * Purpose:    Insert new node in correct alphabetical location in list
 * Input arg:  string = new string to be added to list
 * In/out arg: list_p = pointer to struct storing head and tail ptrs
 * Note:       If the string is already in the list, print a message
 *                and return, leaving list unchanged
 */
void Insert(struct list_s* list_p, char string[]) {

   struct list_node_s* current_node = list_p->h_p;

   //find length of string
   int string_length;
   for (string_length = 0; string[string_length] != '\0'; ++string_length);

   //create new node with given string
   struct list_node_s* new_node = Allocate_node(string_length);
   strcpy(new_node->data, string);

   //if list_p is empty, given string will be the first node in the list
   if (current_node == NULL) {
      list_p->h_p = new_node;
      list_p->t_p = new_node;
      return;
   }

   //if list_p is not empty, will attempt to insert given string into list
   int result;
   while (current_node != NULL) {
      //result compares string to current_node's data
      result = strcmp(string, current_node->data);

      //strcmp() returns 0 if string matches current_node's data (duplicate)   
      if (result == 0) {
         printf("Given string to insert is already in the list.\n");
         Free_node(new_node);
         return;
      
      //strcmp() returns > 0 if string is greater than current_node's data (ex. if string is "efgh" and current_node's data is "abcd")   
      } else if (result > 0) {
         //INSERTING NEW NODE: inserting new node at the tail (after current_node which is the current tail node)
         if (current_node->next_p == NULL) {
            new_node->prev_p = current_node;
            current_node->next_p = new_node;
            list_p->t_p = new_node;
            return;
            
         //else traverse linked list, the next node becomes the new current_node
         } else {
            current_node = current_node->next_p;
         }

      //strcmp() returns < 0 if string is less than current_node's data (ex. if string is "abcd" and current_node's data is "efgh")
      } else {
         //INSERTING NEW NODE: inserting new node at the head (before current_node which is the current head node)
         if (current_node->prev_p == NULL) {
            new_node->next_p = current_node;
            current_node->prev_p = new_node;
            list_p->h_p = new_node;

         //INSERTING NEW NODE: inserting newnode in the middle (before current_node)
         } else {
            new_node->next_p = current_node;
            new_node->prev_p = current_node->prev_p;
            (current_node->prev_p)->next_p = new_node;
            current_node->prev_p = new_node;

         }
         return;
      }
   }
}  /* Insert */

/*-----------------------------------------------------------------*/
/* Function:   Print
 * Purpose:    Print the contents of the nodes in the list
 * Input arg:  list_p = pointers to first and last nodes in list
 *
 * Hint: The implementation of this function (below) shows how
 * to traverse a linked list.
 */
void Print(struct list_s* list_p) {
   struct list_node_s* curr_p = list_p->h_p;

   printf("list = ");

   while (curr_p != NULL) {
      printf("%s ", curr_p->data);
      curr_p = curr_p->next_p;
   }
   printf("\n");
}  /* Print */


/*-----------------------------------------------------------------*/
/* Function:   Member
 * Purpose:    Search list for string
 * Input args: string = string to search for
 *             list_p = pointers to first and last nodes in list
 * Return val: 1, if string is in the list, 0 otherwise
 */
int Member(struct list_s* list_p, char string[]) {

   struct list_node_s* current_node = list_p->h_p;

   //if list_p is empty, will not attempt to search list for given string, returns 0
   if (current_node == NULL) {
      printf("List is currently empty.\n");
      return 0;
   }

   //if list_p is not empty, will attempt to search for given string
   int result;
   while (current_node != NULL) {
      //result compares string to current_node's data
      result = strcmp(string, current_node->data);

      //strcmp() returns 0 if string matches current_node's data (string is in the list)   
      if (result == 0) {
         return 1;      
      
      //strcmp() returns < 0 if string is less than current_node's data (ex. if string is "hi" and current_node's data is "how", if string comes BEFORE current_node)
      } else if (result < 0) {
         return 0;
      }

      //otherwise continues to traverse list, the next node becomes the new current_node
      current_node = current_node->next_p;
   }

   //returns 0 if none of the above has applied
   return 0;
}  /* Member */


/*-----------------------------------------------------------------*/
/* Function:   Free_node
 * Purpose:    Free storage used by a node of the list
 * In/out arg: node_p = pointer to node to be freed
 */
void Free_node(struct list_node_s* node_p) {
   free(node_p->data);
   free(node_p);
}  /* Free_node */


/*-----------------------------------------------------------------*/
/* Function:   Delete
 * Purpose:    Delete node containing string.
 * Input arg:  string = string to be deleted
 * In/out arg  list_p = pointers to head and tail of list
 * Note:       If the string is in the list, it will be unique.  So
 *             at most one node will be deleted.  If the string isn't
 *             in the list, the function just prints a message and
 *             returns, leaving the list unchanged.
 */
void Delete(struct list_s* list_p, char string[]) {

   struct list_node_s* current_node = list_p->h_p;

   //if list_p is empty, will not attempt to delete a node from list
   if (current_node == NULL) {
      printf("List is currently empty.\n");
      return;
   }

   //if list_p is not empty, and given string is the only node in the list, will attempt to delete it
   if (current_node != NULL) {
      if (strcmp(current_node->data, string) == 0 && current_node->next_p == NULL) {
         Free_node(current_node);
         list_p->h_p = NULL;
         list_p->t_p = NULL;
         return;
      }
   }

   //if list_p is not empty, will attempt to delete given string node from list
   int result;
   while (current_node != NULL) {
      //result compares string to current_node's data
      result = strcmp(string, current_node->data);

      //strcmp() returns 0 if string matches current_node's data (if string to delete is current_node)   
      if (result == 0) {
         
         //DELETING NODE: current_node is the head node
         if (current_node->prev_p == NULL && current_node->next_p != NULL) {
            (current_node->next_p)->prev_p = NULL;
            list_p->h_p = current_node->next_p;
         
         //DELETING NODE: current_node is the tail node
         } else if (current_node->next_p == NULL && current_node->prev_p != NULL) {
            (current_node->prev_p)->next_p = NULL;
            list_p->t_p = current_node->prev_p;
         
         //DELETING NODE: current_node is neither the head nor tail node
         } else if (current_node->prev_p != NULL && current_node->next_p != NULL) {
            (current_node->prev_p)->next_p = current_node->next_p;
            (current_node->next_p)->prev_p = current_node->prev_p;
         }
         
         //free current_node, and return
         Free_node(current_node);
         return;
      
      //strcmp() returns < 0 if string is less than current_node's data (ex. if string is "hi" and current_node's data is "how", if string comes BEFORE current_node)
      } else if (result < 0) {
         printf("Given string is not in list.\n");
         return;
      }

      //otherwise continues to traverse list, the next node becomes the new current_node
      current_node = current_node->next_p;
   }
}  /* Delete */

/*-----------------------------------------------------------------*/
/* Function:   Free_list
 * Purpose:    Free storage used by list
 * In/out arg: list_p = pointers to head and tail of list
 */
void Free_list(struct list_s* list_p) {
   struct list_node_s* current_node = list_p->h_p;
   struct list_node_s* next_node;

   //frees each node individually
   while (current_node != NULL) {
      next_node = current_node->next_p;
      Free_node(current_node);
      current_node = next_node;
   }

   //makes head and tail node null
   list_p->h_p = NULL;
   list_p->t_p = NULL;   

}  /* Free_list */


/*-----------------------------------------------------------------*/
/* Function:   Get_command
 * Purpose:    Find and return the next non-whitespace character in
 *             the input stream
 * Return val: The next non-whitespace character in stdin
 */
char Get_command(void) {
   char c;

   printf("Please enter a command (i, d, m, p, f, q):  ");
   /* Put the space before the %c so scanf will skip white space */
   scanf(" %c", &c);
   return c;
}  /* Get_command */

/*-----------------------------------------------------------------*/
/* Function:   Get_string
 * Purpose:    Read the next string in stdin (delimited by whitespace)
 * Out arg:    string = next string in stdin
 */
void Get_string(char string[]) {

   printf("Please enter a string:  ");
   scanf("%s", string);
}  /* Get_string */


/*-----------------------------------------------------------------*/
/* Function:  Print_node
 * Purpose:   Print the data member in a node or NULL if the
 *            pointer is NULL
 * In args:   title:  name of the node
 *            node_p:  pointer to node
 */
void Print_node(char title[], struct list_node_s* node_p) {
   printf("%s = ", title);
   if (node_p != NULL)
      printf("%s\n", node_p->data);
   else
      printf("NULL\n");
}  /* Print_node */

