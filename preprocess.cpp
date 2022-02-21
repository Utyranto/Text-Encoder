/* Author: Riley Than
* Date: 2/6/2022
* Class: CSE 310
* Professor: Syrotiuk
* Description: Perform preprocessing
*/

#include <stdio.h> //why not.
#include <iostream> //Standard procedure 
#include <string> //string to make it easier 
#include "defns.h"
using namespace std;

int main(int argc, char** argv){ //takes in several args for file i/o
symbol* Symbols = new symbol[NSYMBOLS]; //symbol array. Has 128 entries in ASCII, each index corresponds to the ID number of an ASCII character with a frequency of 0.

    for (int i = 0; i < NSYMBOLS; i++){ //create the array
        Symbols[i].symbol = i;
        Symbols[i].freq = 0;
        Symbols[i].parent = NULL;
        Symbols[i].left = NULL;
        Symbols[i].right = NULL;
        Symbols[i].encoding[0] = '\0';
    }

    for (string text; getline(cin, text);){ //reads the lines one by one via file
        for (int i = 0; i < text.length(); i++)
        {
            Symbols[text[i]].freq += 1;

        }
        Symbols[10].freq += 1;
    }

    for (int i = 0; i < NSYMBOLS; i++){ //prints out the text. ID followed by frequency.
        if (Symbols[i].freq > 0){
            cout << i << '\t' << Symbols[i].freq << endl;
        }
    }
    
    return 0;
}
