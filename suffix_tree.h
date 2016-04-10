/*
 * A C program to implement Ukkonen's Suffix Tree Construction
 * Including string operations bassed on the Suffix Tree
 */

#define MAX_CHAR 256

struct SuffixTreeNode {
    struct SuffixTreeNode *children[MAX_CHAR];

    //pointer to other node via suffix link
    struct SuffixTreeNode *suffixLink;

    /*(start, end) interval specifies the edge, by which the
     node is connected to its parent node. Each edge will
     connect two nodes,  one parent and one child, and
     (start, end) interval of a given edge  will be stored
     in the child node. Lets say there are two nods A and B
     connected by an edge with indices (5, 8) then this
     indices (5, 8) will be stored in node B. */
    int start;
    int *end;

    /*for leaf nodes, it stores the index of suffix for
      the path  from root to leaf*/
    int suffixIndex;
};

typedef struct SuffixTreeNode Node;

struct substring {
        int stringstart;
        int stringend;
};

struct substring subtrings[MAX_CHAR];

/**
 * Create new Suffix Tree Node
 */
Node *newNode(int start, int *end);

/*
 * Return the edge length of given node
 */
int edgeLength(Node *n);

/*activePoint change for walk down (APCFWD) using
 Skip/Count Trick  (Trick 1). If activeLength is greater
 than current edge length, set next  internal node as
 activeNode and adjust activeEdge and activeLength
 accordingly to represent same activePoint*/
int walkDown(Node *currNode);

void extendSuffixTree(int pos);

void print(int i, int j);

void setSuffixIndexByDFS(Node *n, int labelHeight);

void freeSuffixTreeByPostOrder(Node *n);

/*Build the suffix tree and print the edge labels along with
suffixIndex. suffixIndex for leaf edges will be >= 0 and
for non-leaf edges will be -1*/
void buildSuffixTree();

int doTraversal(Node *n, int labelHeight, int* maxHeight,
int* substringStartIndex);

void getLongestCommonSubstring();
