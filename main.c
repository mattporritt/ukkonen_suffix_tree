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
                case 't': //Run the self test suite
                {
                        self_test();
                        exit(0);
                }
                case 'p': //Print the generated tree
                {
                        print_enabled = 1;
                        break;
                }
                case ARGP_KEY_ARG: //Process the command line arguments
                {
                        (*arg_count)--;
                        if (*arg_count == 1){
                                text = (unsigned char*)arg;
                        }
                        else if (*arg_count == 0){
                                text2 = (unsigned char*)arg;
                        }

                }
                break;
                case ARGP_KEY_END:
                {
                        printf ("\n");
                        if (*arg_count >= 2){
                                argp_failure (state, 1, 0, "too few arguments");
                        }
                        else if (*arg_count < 0){
                                argp_failure (state, 1, 0, "too many arguments");
                        }
                        else {
                                // Construct the tree and process based on supplied options
                                buildSuffixTree();
                                freeSuffixTreeByPostOrder(root);
                                if (print_enabled){
                                        printf(tree_string, 's');
                                }
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
        unsigned char input[] = "abc"; //Build Suffix tree for this string
        char *tree_output = "abc [0]\nbc [1]\nc [2]\n"; //Expected output
        text = (unsigned char*)input;
        printf("Building suffix tree for string: %s \n", text);
        buildSuffixTree();
        freeSuffixTreeByPostOrder(root);
        printf(tree_string, 's');
        int compare_result = strcmp(tree_string, tree_output);
        assert(compare_result == 0);
        printf("Suffix tree build test: Passed\n");

        //printf("All common substrings test: \n");
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

        int arg_count = 2;
        struct argp argp = { options, parse_opt, "STRING [STRING]" };
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
