/*
 * Command line interface for suffix tree functions
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <argp.h>
#include "suffix_tree.h"

// Process command line options and arguements
static int
parse_opt (int key, char *arg, struct argp_state *state)
{
        int *arg_count = state->input;

        switch (key)
        {
                case 't':
                {
                        print_enabled = 1;
                        break;
                }
                case ARGP_KEY_ARG:
                {
                        (*arg_count)--;
                        //unsigned char input[] = arg; //Input string
                        text = (unsigned char*)arg;
                        buildSuffixTree();
                }
                break;
                case ARGP_KEY_END:
                {
                        printf ("\n");
                        if (*arg_count >= 1){
                                argp_failure (state, 1, 0, "too few arguments");
                        }
                        else if (*arg_count < 0){
                                argp_failure (state, 1, 0, "too many arguments");
                        }
                }
                break;
        }
        return 0;
}

int main(int argc, char **argv)
{
        struct argp_option options[] =
        {
                { "tree", 't', 0, 0, "Print the tree for the supplied string"},
                { 0 }
        };

        int arg_count = 1;
        struct argp argp = { options, parse_opt, "STRING" };
        return argp_parse (&argp, argc, argv, 0, 0, &arg_count);

//        print_enabled = 0;
//        unsigned char input[] = "abc"; //Input string
//        text = (unsigned char*)input;
//        buildSuffixTree();
//        freeSuffixTreeByPostOrder(root);
//        printf("\n");
//        strcpy(text, "abcabxabcd$"); buildSuffixTree(); freeSuffixTreeByPostOrder(root);
//        size1 = 21;
//        printf("All Common Substrings in orangeisatypeoffruit and fruitsomestugfruitgoeshereorange are: \n");
//        strcpy((char*)text, "orangeisatypeoffruit#fruitsomestugfruitgoeshereorange$");
//        buildSuffixTree();
//        getLongestCommonSubstring();
//        //Free the dynamically allocated memory
//        freeSuffixTreeByPostOrder(root);

//    return 0;
}
