// A C program to implement Ukkonen's Suffix Tree Construction
// Here we build generalized suffix tree for two strings
// And then we find longest common substring of the two input strings
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "suffix_tree.h"
#include "vector.h"

unsigned char print_enabled = 0;
unsigned char *text = NULL; //Input string 1 (for suffix tree)
unsigned char *text2 = NULL; //Input string 2 (for generalized suffix tree)
char *tree_string = NULL; //String to hold text version of tree
Node *root = NULL; //Pointer to root node
Node *lastNewNode = NULL;
Node *activeNode = NULL;

/*activeEdge is represented as input string character
  index (not the character itself)*/
int activeEdge = -1;
int activeLength = 0;

// remainingSuffixCount tells how many suffixes yet to
// be added in tree
int remainingSuffixCount = 0;
int leafEnd = -1;
int *rootEnd = NULL;
int *splitEnd = NULL;
int totalStringLength = -1; //Length of input string
int string1Length = 0; //totalStringLength of 1st string

Node *newNode(int start, int *end)
{
    Node *node =(Node*) malloc(sizeof(Node));
    int i;
    for (i = 0; i < MAX_CHAR; i++)
          node->children[i] = NULL;

    /*For root node, suffixLink will be set to NULL
    For internal nodes, suffixLink will be set to root
    by default in  current extension and may change in
    next extension*/
    node->suffixLink = root;
    node->start = start;
    node->end = end;

    /*suffixIndex will be set to -1 by default and
      actual suffix index will be set later for leaves
      at the end of all phases*/
    node->suffixIndex = -1;
    return node;
}

int edgeLength(Node *n) {
    if(n == root)
        return 0;
    return *(n->end) - (n->start) + 1;
}

int walkDown(Node *currNode)
{
    /*activePoint change for walk down (APCFWD) using
     Skip/Count Trick  (Trick 1). If activeLength is greater
     than current edge length, set next  internal node as
     activeNode and adjust activeEdge and activeLength
     accordingly to represent same activePoint*/
    if (activeLength >= edgeLength(currNode))
    {
        activeEdge += edgeLength(currNode);
        activeLength -= edgeLength(currNode);
        activeNode = currNode;
        return 1;
    }
    return 0;
}

void extendSuffixTree(int pos)
{
    /*Extension Rule 1, this takes care of extending all
    leaves created so far in tree*/
    leafEnd = pos;

    /*Increment remainingSuffixCount indicating that a
    new suffix added to the list of suffixes yet to be
    added in tree*/
    remainingSuffixCount++;

    /*set lastNewNode to NULL while starting a new phase,
     indicating there is no internal node waiting for
     it's suffix link reset in current phase*/
    lastNewNode = NULL;

    //Add all suffixes (yet to be added) one by one in tree
    while(remainingSuffixCount > 0) {

        if (activeLength == 0)
            activeEdge = pos; //APCFALZ

        // There is no outgoing edge starting with
        // activeEdge from activeNode
        if (activeNode->children[text[activeEdge]] == NULL)
        {
            //Extension Rule 2 (A new leaf edge gets created)
            activeNode->children[text[activeEdge]] =
                                          newNode(pos, &leafEnd);

            /*A new leaf edge is created in above line starting
             from  an existng node (the current activeNode), and
             if there is any internal node waiting for it's suffix
             link get reset, point the suffix link from that last
             internal node to current activeNode. Then set lastNewNode
             to NULL indicating no more node waiting for suffix link
             reset.*/
            if (lastNewNode != NULL)
            {
                lastNewNode->suffixLink = activeNode;
                lastNewNode = NULL;
            }
        }
        // There is an outgoing edge starting with activeEdge
        // from activeNode
        else
        {
            // Get the next node at the end of edge starting
            // with activeEdge
            Node *next = activeNode->children[text[activeEdge]];
            if (walkDown(next))//Do walkdown
            {
                //Start from next node (the new activeNode)
                continue;
            }
            /*Extension Rule 3 (current character being processed
              is already on the edge)*/
            if (text[next->start + activeLength] == text[pos])
            {
                //If a newly created node waiting for it's
                //suffix link to be set, then set suffix link
                //of that waiting node to curent active node
                if(lastNewNode != NULL && activeNode != root)
                {
                    lastNewNode->suffixLink = activeNode;
                    lastNewNode = NULL;
                }

                //APCFER3
                activeLength++;
                /*STOP all further processing in this phase
                and move on to next phase*/
                break;
            }

            /*We will be here when activePoint is in middle of
              the edge being traversed and current character
              being processed is not  on the edge (we fall off
              the tree). In this case, we add a new internal node
              and a new leaf edge going out of that new node. This
              is Extension Rule 2, where a new leaf edge and a new
            internal node get created*/
            splitEnd = (int*) malloc(sizeof(int));
            *splitEnd = next->start + activeLength - 1;

            //New internal node
            Node *split = newNode(next->start, splitEnd);
            activeNode->children[text[activeEdge]] = split;

            //New leaf coming out of new internal node
            split->children[text[pos]] = newNode(pos, &leafEnd);
            next->start += activeLength;
            split->children[text[next->start]] = next;

            /*We got a new internal node here. If there is any
              internal node created in last extensions of same
              phase which is still waiting for it's suffix link
              reset, do it now.*/
            if (lastNewNode != NULL)
            {
            /*suffixLink of lastNewNode points to current newly
              created internal node*/
                lastNewNode->suffixLink = split;
            }

            /*Make the current newly created internal node waiting
              for it's suffix link reset (which is pointing to root
              at present). If we come across any other internal node
              (existing or newly created) in next extension of same
              phase, when a new leaf edge gets added (i.e. when
              Extension Rule 2 applies is any of the next extension
              of same phase) at that point, suffixLink of this node
              will point to that internal node.*/
            lastNewNode = split;
        }

        /* One suffix got added in tree, decrement the count of
          suffixes yet to be added.*/
        remainingSuffixCount--;
        if (activeNode == root && activeLength > 0) //APCFER2C1
        {
            activeLength--;
            activeEdge = pos - remainingSuffixCount + 1;
        }
        else if (activeNode != root) //APCFER2C2
        {
            activeNode = activeNode->suffixLink;
        }
    }
}

void print(int i, int j)
{
    int k;
    char *printer;
    for (k=i; k<=j && text[k] != '+'; k++){
            asprintf(&printer, "%c", text[k]);
            buildString(&tree_string, printer);
            free(printer);
    }
    if(k<=j){
        buildString(&tree_string, "+");
    }
}

//Print the suffix tree as well along with setting suffix index
//So tree will be printed in DFS manner
//Each edge along with it's suffix index will be printed
void setSuffixIndexByDFS(Node *n, int labelHeight)
{
    if (n == NULL)  return;

    if (n->start != -1) //A non-root node
    {
            if (print_enabled == 1){
                    //Print the label on edge from parent to current node
                    print(n->start, *(n->end));
            }
    }
    int leaf = 1;
    int i;
    char *printer;
    for (i = 0; i < MAX_CHAR; i++)
    {
        if (n->children[i] != NULL)
        {
                if (print_enabled == 1){
                        //print suffix index
                        if (leaf == 1 && n->start != -1){
                                asprintf(&printer, " [%d]\n", n->suffixIndex);
                                buildString(&tree_string, printer);
                                free(printer);
                        }
                }

            //Current node is not a leaf as it has outgoing
            //edges from it.
            leaf = 0;
            setSuffixIndexByDFS(n->children[i], labelHeight +
                                  edgeLength(n->children[i]));
        }
    }
    if (leaf == 1)
    {
        for(i= n->start; i<= *(n->end); i++)
        {
            if(text[i] == '+')
            {
                n->end = (int*) malloc(sizeof(int));
                *(n->end) = i;
            }
        }
        n->suffixIndex = totalStringLength - labelHeight;
        if (print_enabled == 1){
                //print suffix index
                asprintf(&printer, " [%d]\n", n->suffixIndex);
                buildString(&tree_string,printer);
                free(printer);
        }
    }
}

void freeSuffixTreeByPostOrder(Node *n)
{
    if (n == NULL)
        return;
    int i;
    for (i = 0; i < MAX_CHAR; i++)
    {
        if (n->children[i] != NULL)
        {
            freeSuffixTreeByPostOrder(n->children[i]);
        }
    }
    if (n->suffixIndex == -1)
        free(n->end);
    free(n);
}

/*Build the suffix tree and print the edge labels along with
suffixIndex. suffixIndex for leaf edges will be >= 0 and
for non-leaf edges will be -1*/
int buildSuffixTree(unsigned char *string1, unsigned char *string2, unsigned char print_tree){
        //First check if we have at least one valid string
        if(string1 == NULL){
                return 1;
        }
        // Check if we are printing suffix tree
        print_enabled = print_tree;

        //Check if we have two strings
        //Two strings mean we are building a generalized suffix tree
        if(string2 != NULL){
                asprintf(&text, "%s%c%s%c", string1, 43,  string2, 36);
                string1Length = strlen((const char*) string1);
        }
        else{
                asprintf(&text, "%s%c", string1, 36);
        }

        totalStringLength = strlen((const char*) text);
    int i;
    rootEnd = (int*) malloc(sizeof(int));
    *rootEnd = - 1;

    /*Root is a special node with start and end indices as -1,
    as it has no parent from where an edge comes to root*/
    root = newNode(-1, rootEnd);

    activeNode = root; //First activeNode will be root
    for (i=0; i<totalStringLength; i++)
        extendSuffixTree(i);
    int labelHeight = 0;
    setSuffixIndexByDFS(root, labelHeight);

    return 0;
}

int doTraversal(Node *n, int labelHeight, int* maxHeight,
int* substringStartIndex)
{
    if(n == NULL)
    {
        return 0;
    }
    int i=0;
    int ret = -1;
    if(n->suffixIndex < 0) //If it is internal node
    {
        for (i = 0; i < MAX_CHAR; i++)
        {
            if(n->children[i] != NULL)
            {
                ret = doTraversal(n->children[i], labelHeight +
                    edgeLength(n->children[i]),
                    maxHeight, substringStartIndex);

                if(n->suffixIndex == -1)
                    n->suffixIndex = ret;
                else if((n->suffixIndex == -2 && ret == -3) ||
                    (n->suffixIndex == -3 && ret == -2) ||
                    n->suffixIndex == -4)
                {
                    n->suffixIndex = -4;//Mark node as XY
                    //Keep track of deepest node
//                    if(labelHeight > 2)
                    if(*maxHeight < labelHeight)
                    {
                        *maxHeight = labelHeight;
                        *substringStartIndex = *(n->end) -
                            labelHeight + 1;
//                            if (subtrings[i].stringend == 0 || n->start < subtrings[i].stringstart){
//                                    subtrings[i].stringstart = n->start;
//                                    subtrings[i].stringend = *(n->end);
//                            }

                    }
                }
            }
        }
    }
    else if(n->suffixIndex > -1 && n->suffixIndex < string1Length)//suffix of X
        return -2;//Mark node as X
    else if(n->suffixIndex >= string1Length)//suffix of Y
        return -3;//Mark node as Y
    return n->suffixIndex;
}

int allCommonSubstringsTraversal(Node *n, int labelHeight)
{
    if(n == NULL)
    {
        return 0;
    }
    int i=0;
    int ret = -1;
    if(n->suffixIndex < 0) //If it is internal node
    {
        for (i = 0; i < MAX_CHAR; i++)
        {
            if(n->children[i] != NULL)
            {
                ret = allCommonSubstringsTraversal(n->children[i], labelHeight + edgeLength(n->children[i]));

                if(n->suffixIndex == -1)
                    n->suffixIndex = ret;
                else if((n->suffixIndex == -2 && ret == -3) ||
                    (n->suffixIndex == -3 && ret == -2) ||
                    n->suffixIndex == -4)
                {
                    n->suffixIndex = -4;//Mark node as XY
                    if(labelHeight > 2){
                            if (subtrings[i].stringend == 0 || n->start < subtrings[i].stringstart){
                                    subtrings[i].stringstart = n->start;
                                    subtrings[i].stringend = *(n->end);
                            }
                    }
                }
            }
        }
    }
    else if(n->suffixIndex > -1 && n->suffixIndex < string1Length)//suffix of X
        return -2;//Mark node as X
    else if(n->suffixIndex >= string1Length)//suffix of Y
        return -3;//Mark node as Y
    return n->suffixIndex;
}

char *getLongestCommonSubstring(unsigned char *string1, unsigned char *string2, unsigned char print_tree)
{
    int maxHeight = 0;
    int substringStartIndex = 0;


    //First check if we have two strings
    if((string1 == NULL) || (string2 == NULL)){
            return NULL;
    }
    buildSuffixTree(string1, string2, print_tree);
    doTraversal(root, 0, &maxHeight, &substringStartIndex);
    static char *longest_substring = NULL;
    char *printer;
    int k;
    buildString(&longest_substring,"");
    for (k=0; k<maxHeight; k++){
            asprintf(&printer, "%c", text[k + substringStartIndex]);
            buildString(&longest_substring,printer);
            free(printer);

    }
    if(k == 0){
            buildString(&longest_substring,"No common substring");
    }
    else{
            asprintf(&printer, ", of length: %d",maxHeight);
            buildString(&longest_substring,printer);
            free(printer);
    }
    buildString(&longest_substring,"\n");


    //Free the dynamically allocated memory
    freeSuffixTreeByPostOrder(root);

    return longest_substring;
}

char *getAllCommonSubstrings(unsigned char *string1, unsigned char *string2, unsigned char print_tree){
        //First check if we have two strings
        if((string1 == NULL) || (string2 == NULL)){
                return NULL;
        }
        buildSuffixTree(string1, string2, print_tree);

        int i = 0;
        allCommonSubstringsTraversal(root, 0);

        for (i = 0; i < MAX_CHAR; i++){
                if(subtrings[i].stringend != 0){
                        printf("string start: %d \n",subtrings[i].stringstart);
                        printf("string end: %d \n",subtrings[i].stringend);
                }
        }

        return NULL;
}

int traverseEdge(unsigned char *str, int idx, int start, int end){
    int k = 0;
    //Traverse the edge with character by character matching
    for(k=start; k<=end && str[idx] != '\0'; k++, idx++)
    {
        if(text[k] != str[idx])
            return -1;  // mo match
    }
    if(str[idx] == '\0')
        return 1;  // match
    return 0;  // more characters yet to match
}

int substringTraversal(Node *n,unsigned char* str, int idx){
    if(n == NULL)
    {
        return -1; // no match
    }
    int res = -1;
    //If node n is not root node, then traverse edge
    //from node n's parent to node n.
    if(n->start != -1)
    {
        res = traverseEdge(str, idx, n->start, *(n->end));
        if(res != 0)
            return res;  // match (res = 1) or no match (res = -1)
    }
    //Get the character index to search
    idx = idx + edgeLength(n);
    //If there is an edge from node n going out
    //with current character str[idx], travrse that edge
    if(n->children[str[idx]] != NULL)
        return substringTraversal(n->children[str[idx]], str, idx);
    else
        return -1;  // no match
}

int doTraversalToCountLeaf(Node *n){
    if(n == NULL)
        return 0;
    if(n->suffixIndex > -1)
    {
        printf("\nFound at position: %d", n->suffixIndex);
        return 1;
    }
    int count = 0;
    int i = 0;
    for (i = 0; i < MAX_CHAR; i++)
    {
        if(n->children[i] != NULL)
        {
            count += doTraversalToCountLeaf(n->children[i]);
        }
    }
    return count;
}

int countLeaf(Node *n){
    if(n == NULL)
        return 0;
    return doTraversalToCountLeaf(n);
}

int substringAllOccurenceTraversal(Node *n, unsigned char* str, int idx){
    if(n == NULL)
    {
        return -1; // no match
    }
    int res = -1;
    //If node n is not root node, then traverse edge
    //from node n's parent to node n.
    if(n->start != -1)
    {
        res = traverseEdge(str, idx, n->start, *(n->end));
        if(res == -1)  //no match
            return -1;
        if(res == 1) //match
        {
            if(n->suffixIndex > -1)
                printf("\nsubstring count: 1 and position: %d",
                               n->suffixIndex);
            else
                printf("\nsubstring count: %d", countLeaf(n));
            return 1;
        }
    }
    //Get the character index to search
    idx = idx + edgeLength(n);
    //If there is an edge from node n going out
    //with current character str[idx], travrse that edge
    if(n->children[str[idx]] != NULL)
        return substringAllOccurenceTraversal(n->children[str[idx]], str, idx);
    else
        return -1;  // no match
}

int checkForSubString(unsigned char* search_string, unsigned char* source_string){
        buildSuffixTree(source_string, NULL, 0);
        int res = substringTraversal(root, search_string, 0);
        //Free the dynamically allocated memory
        freeSuffixTreeByPostOrder(root);

        if(res == 1)
                return 1;
        else
                return 0;
}

int checkAllSubStringOccurences(unsigned char* search_string, unsigned char* source_string){
        buildSuffixTree(source_string, NULL, 0);
        int res = substringAllOccurenceTraversal(root, search_string, 0);
        //Free the dynamically allocated memory
        freeSuffixTreeByPostOrder(root);

        if(res == 1)
                return 1;
        else
                return 0;
}

void buildString(char** current_text, const char *new_text){
        size_t new_len = strlen(new_text) + 1; /* + 1 for terminating NULL */
        if (*current_text == NULL){
                *current_text = malloc(new_len);
        }
        size_t current_len = strlen(*current_text);
        *current_text = realloc(*current_text, (new_len + current_len));
        strncat(*current_text, new_text, new_len);

}

/*
 * Vector (Dynamic Array) helper functions
 */

void vector_init(Vector *vector) {
  // initialize size and capacity
  vector->size = 0;
  vector->capacity = VECTOR_INITIAL_CAPACITY;

  // allocate memory for vector->data
  vector->data = malloc(sizeof(int) * vector->capacity);
}

void vector_append(Vector *vector, int value) {
  // make sure there's room to expand into
  vector_double_capacity_if_full(vector);

  // append the value and increment vector->size
  vector->data[vector->size++] = value;
}

int vector_get(Vector *vector, int index) {
  if (index >= vector->size || index < 0) {
    printf("Index %d out of bounds for vector of size %d\n", index, vector->size);
    exit(1);
  }
  return vector->data[index];
}

void vector_set(Vector *vector, int index, int value) {
  // zero fill the vector up to the desired index
  while (index >= vector->size) {
    vector_append(vector, 0);
  }

  // set the value at the desired index
  vector->data[index] = value;
}

void vector_double_capacity_if_full(Vector *vector) {
  if (vector->size >= vector->capacity) {
    // double vector->capacity and resize the allocated memory accordingly
    vector->capacity *= 2;
    vector->data = realloc(vector->data, sizeof(int) * vector->capacity);
  }
}

void vector_free(Vector *vector) {
  free(vector->data);
}
