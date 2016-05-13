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
unsigned char *input_text; //Input string 1 (for suffix tree)
unsigned char *input_text2; //Input string 2 (for generalized suffix tree)
int print_tree = 0;

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
                        print_tree = 1;
                        break;
                }
                case ARGP_KEY_ARG: //Process the command line arguments
                {
                        (*arg_count)--;
                        if (*arg_count == 1){
                                input_text = (unsigned char*)arg;
                        }
                        else if (*arg_count == 0){
                                input_text2 = (unsigned char*)arg;
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
                                buildSuffixTree(input_text, input_text2, print_tree);
                                freeSuffixTreeByPostOrder(root);
                                if (print_tree){
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
        char *tree_output =     "$ [3]\n"
                                "abc$ [0]\n"
                                "bc$ [1]\n"
                                "c$ [2]\n"; //Expected output
        printf("Building suffix tree for string: abc \n");
        buildSuffixTree((unsigned char *)"abc", NULL, 1); //Build Suffix tree for this string
        freeSuffixTreeByPostOrder(root);
        printf(tree_string, 's');
        int compare_result = strcmp(tree_string, tree_output);
        assert(compare_result == 0);
        memset(tree_string,0,strlen(tree_string)); //clear string from previous test
        char *tree_output2 =    "$ [10]\n"
                                "ab [-1]\n"
                                "c [-1]\n"
                                "abxabcd$ [0]\n"
                                "d$ [6]\n"
                                "xabcd$ [3]\n"
                                "b [-1]\n"
                                "c [-1]\n"
                                "abxabcd$ [1]\n"
                                "d$ [7]\n"
                                "xabcd$ [4]\n"
                                "c [-1]\n"
                                "abxabcd$ [2]\n"
                                "d$ [8]\n"
                                "d$ [9]\n"
                                "xabcd$ [5]\n"; //Expected output
        printf("Building suffix tree for string: abcabxabcd \n");
        buildSuffixTree((unsigned char *)"abcabxabcd", NULL, 1); //Build Suffix tree for this string
        freeSuffixTreeByPostOrder(root);
        printf(tree_string, 's');
        int compare_result2 = strcmp(tree_string, tree_output2);
        assert(compare_result2 == 0);
        memset(tree_string,0,strlen(tree_string)); //clear string from previous test
        printf("Suffix tree build test: Passed\n\n");

        printf("Longest substrings test: \n");
        char *lcs;

        printf("Longest Common Substring in xabxac and abcabxabcd is: ");
        lcs = getLongestCommonSubstring((unsigned char *)"xabxac",
                                        (unsigned char *)"abcabxabcd",
                                        0);
        printf(lcs, 's');
        int compare_result3 = strcmp(lcs, "abxa, of length: 4\n");
        assert(compare_result3 == 0);
        memset(lcs,0,strlen(lcs)); //clear string from previous test; //clear string from previous test

        printf("Longest Common Substring in xabxaabxa and babxba is: ");
        lcs = getLongestCommonSubstring((unsigned char *)"xabxaabxa",
                                        (unsigned char *)"babxba",
                                        0);

        printf(lcs, 's');
        int compare_result4 = strcmp(lcs, "abx, of length: 3\n");
        assert(compare_result4 == 0);
        printf("Longest Common Substring test: Passed\n\n");

        printf("Substrings test: \n");
        int is_substring = 0;

        printf("\"test\" is a substring of \"this is a test\" \n");
        is_substring = checkForSubString((unsigned char *)"test",
                                         (unsigned char *)"this is a test");
        assert(is_substring == 1);

        printf("\"foo\" is a substring of \"this is a test\" \n");
        is_substring = checkForSubString((unsigned char *)"foo",
                                         (unsigned char *)"this is a test");
        assert(is_substring == 0);
        printf("Substrings test: Passed\n\n");

        printf("All occurrences of Substring test: \n");
        int *all_substrings;
        size_t i = 0;

        printf("Text: AABAACAADAABAAABAA, Pattern to search: AABA\n");
        all_substrings = checkAllSubStringOccurences((unsigned char *)"AABA",
                                                     (unsigned char *)"AABAACAADAABAAABAA");
        assert(all_substrings[0] == 3);
        assert(all_substrings[1] == 13);
        assert(all_substrings[2] == 9);
        assert(all_substrings[3] == 0);

        printf("Substring found count: %d\n", all_substrings[0]);
        printf("Substrings found at positions: ");
        for( i = 1; i <all_substrings[0] + 1; i++)
        {
            printf ("%d,",all_substrings[i]);
        }
        free(all_substrings);

        printf("\n\nText: AABAACAADAABAAABAA, Pattern to search: AABAACAAD\n");
        all_substrings = checkAllSubStringOccurences((unsigned char *)"AABAACAAD",
                                                     (unsigned char *)"AABAACAADAABAAABAA");
        assert(all_substrings[0] == 1);
        assert(all_substrings[1] == 0);

        printf("Substring found count: %d\n", all_substrings[0]);
        printf("Substrings found at position: %d", all_substrings[1]);
        free(all_substrings);

        printf("\n\nText: AABAACAADAABAAABAA, Pattern to search: AA\n");
        all_substrings = checkAllSubStringOccurences((unsigned char *)"AA",
                                                     (unsigned char *)"AABAACAADAABAAABAA");
        assert(all_substrings[0] == 7);
        assert(all_substrings[1] == 16);
        assert(all_substrings[2] == 12);
        assert(all_substrings[3] == 13);
        assert(all_substrings[4] == 9);
        assert(all_substrings[5] == 0);
        assert(all_substrings[6] == 3);
        assert(all_substrings[7] == 6);

        printf("Substring found count: %d\n", all_substrings[0]);
        printf("Substrings found at positions: ");
        for( i = 1; i <all_substrings[0] + 1; i++)
        {
            printf ("%d,",all_substrings[i]);
        }
        free(all_substrings);


        printf("\n\nText: AABAACAADAABAAABAA, Pattern to search: ZZ\n");
        all_substrings = checkAllSubStringOccurences((unsigned char *)"ZZ",
                                                     (unsigned char *)"AABAACAADAABAAABAA");
        assert(all_substrings[0] == 0);
        printf("No Substrings found\n");
        printf("\nAll occurences of Substring test: Passed\n\n");

//        printf("All Common substrings test: \n");
//
//        printf("All Common Substrings in orangeisatypeoffruit and fruitsomestugfruitgoeshereorange are: \n");
//        getAllCommonSubstrings((unsigned char *)"orangeisatypeoffruit",
//                                        (unsigned char *)"fruitsomestugfruitgoeshereorange",
//                                        0);

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
