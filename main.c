/*
 * Command line interface for suffix tree functions
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "suffix_tree.h"
int main(int argc, char *argv[])
{

        unsigned char input[] = "abc"; //Input string
        text = (unsigned char*)input;
        buildSuffixTree();
        freeSuffixTreeByPostOrder(root);
        printf("\n");
//        strcpy(text, "abcabxabcd$"); buildSuffixTree(); freeSuffixTreeByPostOrder(root);
//        size1 = 21;
//        printf("All Common Substrings in orangeisatypeoffruit and fruitsomestugfruitgoeshereorange are: \n");
//        strcpy((char*)text, "orangeisatypeoffruit#fruitsomestugfruitgoeshereorange$");
//        buildSuffixTree();
//        getLongestCommonSubstring();
//        //Free the dynamically allocated memory
//        freeSuffixTreeByPostOrder(root);

    return 0;
}
