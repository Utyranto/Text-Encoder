/* Author: Riley Than
* Date: 2/6/2022
* Class: CSE 310
* Professor: Syrotiuk
* Description: Encode text via two functions (insertion/merge)
*/

#include <iostream>
#include <sstream>
#include <fstream>
#include <stdio.h>
#include "defns.h"
using namespace std;

void insertionSort(tree array[], int size){
    for (int i = 1; i < size; i++){ //make a loop for the size of the array
        tree temp = array[i]; //set the temporary value as whatever index the loop is on
        int j = i - 1; //to compare the index to the previous index
        while((j >= 0) && (array[j].freq > temp.freq)){ //keep comparing the frequency values down until the freq at arr[j] is less than temp
            array[j + 1] = array[j];
            j--; 
        }
        array[j + 1] = temp; //set a new temp value
    }

}

void mergeSort(tree arr[], int size){
    insertionSort(arr, size); //merge sort has not been done yet so this will send to insertion sort
}

int main(int argc, char** argv){

symbol* Symbols = new symbol[NSYMBOLS];

    for (int i = 0; i < NSYMBOLS; i++){ //set up the array like in 1.1
        Symbols[i].symbol = i;
        Symbols[i].freq = 0;
        Symbols[i].parent = NULL;
        Symbols[i].left = NULL;
        Symbols[i].right = NULL;
        Symbols[i].encoding[0] = '\0';
    }

    ifstream infile;
    infile.open(argv[1]); //Open the input file to read.

    int ID;
    int frequency;

    int alphaSize = 0; //to increment the size of the array.
    int nonAlphaSize = 0;

    for(string line; getline(infile, line);){ //uses \t as a delimiter so it can separate 
        istringstream ss(line); //hoping this is allowed. The purpose for this is to use as a delimiter.
        string token;
        int even = 0;
            while(getline(ss, token, '\t')) { //delimiter
                if ((even % 2) == 0){ 
                    ID = stoi(token); //converts the part of the text into ID
                } else {
                    frequency = stoi(token); // converts other part of text into freq.
                }
                even = 1; //best way to switch the reading to frequency every other loop.
        }
        Symbols[ID].freq = frequency;
        if ((65 <= ID && ID <= 90) || (97 <= ID && ID <= 122)){
            alphaSize++; //increments when it detects an ID number that corresponds with an alphabet character.
        } else {
            nonAlphaSize++; //increments when it does not detect that the ID number is an alphabet character.
        }

    }
    infile.close(); //memory leak prevention.

    tree* Alpha = new tree[alphaSize]; //create two arrays
    tree* NonAlpha = new tree[nonAlphaSize];

    int increm = 0;

    for (int i = 0; i < NSYMBOLS; i++){
        if (((65 <= i && i <= 90) || (97 <= i && i <= 122)) && Symbols[i].freq != 0){ 
            Alpha[increm].index = i;
            Alpha[increm].symbol = Symbols[i].symbol;
            Alpha[increm].freq = Symbols[i].freq;
            Alpha[increm].root = &Symbols[i];
            increm++;
        }
    } //fill the array indexes with values of characters.

    increm = 0;
    for (int i = 0; i < NSYMBOLS; i++){
        if (((0 <= i && i <= 64) || (91 <= i && i <= 96) || (123 <= i && i <= 127)) && Symbols[i].freq != 0){
            NonAlpha[increm].index = i;
            NonAlpha[increm].symbol = Symbols[i].symbol;
            NonAlpha[increm].freq = Symbols[i].freq;
            NonAlpha[increm].root = &Symbols[i];
            increm++;
        }
    } //ditto

    string choice = argv[2]; //reads second argument and performs either sorting method. Merge sort has not been completed yet so mergeSort only calls insertion.
    if (choice.compare("insertion") == 0){
        insertionSort(Alpha, alphaSize); //Throws both alpha and non alpha into sorting.
        insertionSort(NonAlpha, nonAlphaSize);
    } else if(choice.compare("merge") == 0){
        mergeSort(Alpha, nonAlphaSize);
        mergeSort(NonAlpha, nonAlphaSize);
    } else {
        cout << "Not a valid choice." << endl;
    }
    

    for (int i = 0; i < alphaSize - 1; i++) { //constructs the binary tree. Placed into a loop since it's needed to link the other child nodes.
        tree tL = Alpha[0]; //establishes the parent node of the Alpha tree.
        tree tR = Alpha[1];
        symbol* t = new symbol;
        t->parent = NULL;

        tL.root->parent = t;
        tR.root->parent = t;

        t->left = tL.root;
        t->right = tR.root;
        t->freq = tL.freq + tR.freq;

        for (int i = 1; i < alphaSize; i++){ //removes the first two indexes of the array and pushes everything else to the front.
            Alpha[i-1] = Alpha[i];
        }

        for (int i = 1; i < alphaSize; i++){
            Alpha[i-1] = Alpha[i];
        } //Really not the most optimal way of doing this but I'm too monkey-brained to make it any better.

        int pos = 0;
        int found = 0;

        while (found == 0){ //looks for the proper position to place the new tree
            if(t->freq > Alpha[pos].freq){
                found = 1;
            } else {
                pos++;
            }
        }

        for (int i = alphaSize; i > pos; i--){
            Alpha[i] = Alpha[i-1];
        }

        tree newTree;
        newTree.freq = t->freq;
        newTree.root = t;

        Alpha[pos - 1] = newTree; //places the new tree into the proper position
        alphaSize--;
    }

    for (int i = 0; i < nonAlphaSize - 1; i++) { //ditto. Don't expect any changes compared to alpha whatsoever.
        tree tL = NonAlpha[0];
        tree tR = NonAlpha[1];
        symbol* t = new symbol;
        t->parent = NULL;

        tL.root->parent = t;
        tR.root->parent = t;

        t->left = tL.root;
        t->right = tR.root;
        t->freq = tL.freq + tR.freq;

        for (int i = 1; i < nonAlphaSize; i++){
            NonAlpha[i-1] = NonAlpha[i];
        }

        for (int i = 1; i < nonAlphaSize; i++){
            NonAlpha[i-1] = NonAlpha[i];
        }

        int pos = 0;
        int found = 0;

        while (found == 0){
            if(t->freq > NonAlpha[pos].freq){
                found = 1;
            } else {
                pos++;
            }
        }

        for (int i = nonAlphaSize; i > pos; i--){
            NonAlpha[i] = NonAlpha[i-1];
        }

        tree newTree;
        newTree.freq = t->freq;

        NonAlpha[pos - 1] = newTree;

        nonAlphaSize--;
    }

    //Segmentation faults happen occasionally.
    //TODO: Where's the source? How does this happen? Too stressed to even think ...depressed_wojak.jpg

    
    symbol* Root = new symbol; //joins both Alpha and NonAlpha trees together under a single root node.
    tree tL = Alpha[0];
    tree tR = NonAlpha[0];

    Root->left = tL.root;
    Root->right = tR.root;
    Root->parent = NULL;

    Alpha[0].root->parent = Root;
    NonAlpha[0].root->parent = Root;

    //Not sure if it's the best way to do it but I have no other options.


    //project ends here because no one in the tutoring center knows what the f*ck is step 7-8 and the print encode part.
    //I apologize for my profound mental retardation and I can't help but think of the most monkey-brained sh*t ever possible. 
    //I get even more depressed writing this stuff it's not even funny.

    return 0;
}

