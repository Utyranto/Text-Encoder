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

void merge(tree arr[], int p, int q, int r){
    // Create L ← A[p..q] and M ← A[q+1..r]
  int n1 = q - p + 1;
  int n2 = r - q;

  tree L[n1], M[n2];

  for (int i = 0; i < n1; i++)
    L[i] = arr[p + i];
  for (int j = 0; j < n2; j++)
    M[j] = arr[q + 1 + j];

  // Maintain current index of sub-arrays and main array
  int i, j, k;
  i = 0;
  j = 0;
  k = p;

  // Until we reach either end of either L or M, pick larger among
  // elements L and M and place them in the correct position at A[p..r]
  while (i < n1 && j < n2) {
    if (L[i].freq <= M[j].freq) {
      arr[k] = L[i];
      i++;
    } else {
      arr[k] = M[j];
      j++;
    }
    k++;
  }

  // When we run out of elements in either L or M,
  // pick up the remaining elements and put in A[p..r]
  while (i < n1) {
    arr[k] = L[i];
    i++;
    k++;
  }

  while (j < n2) {
    arr[k] = M[j];
    j++;
    k++;
  }
}

void mergeSort(tree arr[], int left, int right){
    if (left < right) {
    int middle = left + (right - left) / 2;

    mergeSort(arr, left, middle);
    mergeSort(arr, middle + 1, right);

    // Merge the sorted subarrays
    merge(arr, left, middle, right);
  }
}

void encoder(symbol *root, string encoded){
    if (root == NULL){
        return;
    }

    encoder(root->right, encoded + "1");

    encoder(root->left, encoded + "0");
    
    for (int i = 0; i < encoded.length(); i++){
        root->encoding[i] = encoded[i];
    }

    root->encoding[encoded.length()] = '\0';
    
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

    increm = 0; //reset increment so I don't need to take up more space than ususal.
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
        mergeSort(Alpha, 0, nonAlphaSize);
        mergeSort(NonAlpha, 0, nonAlphaSize);
    } else {
        cout << "Not a valid choice." << endl;
        abort();
    }

    for (int j = 0; j < alphaSize - 1; j++) { //constructs the binary tree. Placed into a loop since it's needed to link the other child nodes.
        tree tL = Alpha[0]; //establishes the parent node of the Alpha tree.
        tree tR = Alpha[1];
        symbol* t = new symbol;
        t->parent = NULL;

        tL.root->parent = t;
        tR.root->parent = t;

        t->left = tL.root;
        t->right = tR.root;
        t->freq = tL.freq + tR.freq;

        // move the pointer of Alpha forward by 1, effectively decreasing the length of Alpha by 1
        Alpha++;

        // overwrite the first element with the new parent tree's values.
        // This method effectively "deletes" the first two elements and "inserts" the new tree at the first position.
        Alpha[0].root = t;
        Alpha[0].freq = t->freq;
        //FINALLY SOMETHING EFFICIENT!!!!

        Alpha[0].symbol = -64; // garbage symbol, denotes non-leaf node

          for (int i = 1; Alpha[i].freq <= Alpha[i - 1].freq && i < alphaSize - j - 1; i++) {
            tree temp = Alpha[i];
            Alpha[i] = Alpha[i - 1];
            Alpha[i - 1] = temp;
        }
    }

    for (int j = 0; j < nonAlphaSize - 1; j++) { //ditto. Don't expect any changes compared to alpha whatsoever.
        tree tL = NonAlpha[0];
        tree tR = NonAlpha[1];
        symbol* t = new symbol;
        t->parent = NULL;

        tL.root->parent = t;
        tR.root->parent = t;

        t->left = NonAlpha[0].root;
        t->right = NonAlpha[1].root;
        t->freq = NonAlpha[0].freq + NonAlpha[1].freq;

        NonAlpha++;

        NonAlpha[0].root = t;
        NonAlpha[0].freq = t->freq;

        NonAlpha[0].symbol = -64;

          for (int i = 1; NonAlpha[i].freq <= NonAlpha[i - 1].freq && i < nonAlphaSize - j - 1; i++) {
            tree temp = NonAlpha[i];
            NonAlpha[i] = NonAlpha[i - 1];
            NonAlpha[i - 1] = temp;
        }
    }
 
    symbol* Root = new symbol; //joins both Alpha and NonAlpha trees together under a single root node.
    tree tL = Alpha[0];
    tree tR = NonAlpha[0];

    Root->left = tL.root;
    Root->right = tR.root;
    Root->parent = NULL;

    tL.root->parent = Root;
    tR.root->parent = Root;

    encoder(Root, "");

    symbol* current = Root; //establish a pointer to traverse through the tree.
    string decoded = ""; //store the decoded string.
    for(string encoded; getline(cin, encoded);){
        for (int i = 0; i <= encoded.length(); i++) { //using <= to add in the last symbol before exiting.
            if (current->left == NULL || current->right == NULL) { //have we reached a leaf node?
                decoded += current->symbol; //adds in the symbol contained in current.
                current = Root; //resets back to the root of the tree.
                if (i == encoded.length()) //Stop the loop if we reached the end of the line.
                    break;
            } 
            if (encoded[i] == '0') { //check if it's 0
                current = current->left; //move to the left.
            }
            if (encoded[i] == '1') { //check if it's 1
                current= current->right; //move to the right
            }
            
        }
    }
    
    cout << decoded; //This prints the decoded line

    cout << "\n";
    
    Alpha = Alpha - alphaSize + 1;
    NonAlpha = NonAlpha - nonAlphaSize + 1; 

    delete[] Alpha;
    delete[] NonAlpha;

    return 0;
}

