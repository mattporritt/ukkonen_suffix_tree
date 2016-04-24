/*
 * Command line interface for suffix tree functions
 */
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <argp.h>
#include "suffix_tree.h"

void self_test();

// Process command line options and arguments
static int
parse_opt (int key, char *arg, struct argp_state *state)
{
        int *arg_count = state->input;

        switch (key)
        {
                case 't':
                {
                        // run the self test suite
                        self_test();
                        exit(0);
                }
                case 'p':
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

/**
 * run self tests
 */
void self_test(){
        printf("Running Self Tests... \n");
        printf("Build suffix tree test: \n");
        print_enabled = 1;
        unsigned char input[] = "abc"; //Input string
        text = (unsigned char*)input;
        buildSuffixTree();
        freeSuffixTreeByPostOrder(root);
        //printf("All common substrings test: \n");
        printf(tree_string, 's');
        printf("done");
}

int main(int argc, char **argv)
{
        struct argp_option options[] =
        {
                { "print", 'p', 0, 0, "Print the tree for the supplied string"},
                { "test", 't', 0, 0, "Run self tests"},
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
