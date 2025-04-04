/*
 * File: RandomWriter.cpp
 * ----------------------
 * Name: [TODO: enter name here]
 * Section: [TODO: enter section leader here]
 * This file is the starter project for the Random Writer problem.
 * [TODO: rewrite the documentation]
 */

#include <iostream>
#include <fstream>
#include "console.h"
#include "simpio.h"
#include "map.h"
#include "vector"
#include "random.h"
using namespace std;

// constants
const int MIN_MARKOV_NUM = 1;
const int MAX_MARKOV_NUM = 10;
const int NUM_OF_GENERATED_CHARS = 2000;

// function prototypes
ifstream getFile();
int getMarkovNum();
bool valid(int);
void fillMarkovInfo(Map<string, vector<char>>&, ifstream&, int&);
string getMostFrequentSequence(Map<string, vector<char>>& markovMap);
void generateText(Map<string, vector<char>>&, string&);
void printNextChar(vector<char>&, string&, int&);

int main1() {
    while (true) { // testing
        ifstream file = getFile();
        int k = getMarkovNum();
        Map<string, vector<char>> markovMap;
        fillMarkovInfo(markovMap, file, k);
        string mostFrequent = getMostFrequentSequence(markovMap);
        generateText(markovMap, mostFrequent);
        file.close(); // closing file is vital
    }
    return 0;
}

/*
* Makes user enter valid file name
*/
ifstream getFile() {
    while (true) {
        string fileName = getLine("Enter file name : ");
        ifstream file(fileName.c_str());
        if (file.is_open()) {
            return file;
        }
        cout << "Please enter valid file name" << endl;
    }
}

/*
* Makes user enter valid Markov number
*/
int getMarkovNum() {
    while (true) {
        int k = getInteger("Enter number of Markov : ");
        if (valid(k)) {
            return k;
        }
        cout << "Please enter valid Markov number (1-10)" << endl;
    }
}

bool valid(int k) {
    return k >= MIN_MARKOV_NUM && k <= MAX_MARKOV_NUM;
}

/*
* This function fills our map with information about char sequences
* and chars that are followed
*/
void fillMarkovInfo(Map<string, vector<char>>& markovMap, ifstream& file, int& k) {
    string current;
    char ch;
    for (int i = 0; i < k; i++) { // initialize first k chars
        if (file.get(ch)) {
            current += ch;
        }
        else {
            return; // not enough characters 
        }
    }
    while (file.get(ch)) {
        markovMap[current].push_back(ch); // store which chars come after string
        current = current.substr(1) + ch; // shift string one times right
    }
}

/*
* This function gets the string(sequence of chars) that has
* most followed chars placed in vector as values of markovMap
*/
string getMostFrequentSequence(Map<string, vector<char>>& markovMap) {
    int maxSize = 0;
    string resultSequence="";
    for (string sequence : markovMap) {
        int sizeOfVector = markovMap[sequence].size();
        if (sizeOfVector > maxSize) {
            maxSize = sizeOfVector;
            resultSequence = sequence;
        }
    }
    return resultSequence;
}

/*
* This is main part of our program that prints generates text
* with markov logic
*/
void generateText(Map<string, vector<char>>& markovMap, string& mostFrequent) {
    if (markovMap.isEmpty()) {
        cout << "No information in markov map"<<endl;
        return;
    }
    cout << mostFrequent;
    int totalCharsUsed = mostFrequent.size();
    string currentSeed = mostFrequent;
    while (totalCharsUsed < NUM_OF_GENERATED_CHARS) {
        vector<char> followedChars = markovMap[currentSeed];
        if (followedChars.empty()) {
            break; // if seed appears at the end of text(early exit)
        }
        printNextChar(followedChars, currentSeed, totalCharsUsed);
    }
}

/*
* This function chooses next char randomly from vector( probabilities
* play an integral part here), then we print it and modify currentSeed
*/
void printNextChar(vector<char>& followedChars, string& currentSeed, int& totalCharsUsed) {
    int randomIndex = randomInteger(0, followedChars.size() - 1);
    char randomChar = followedChars[randomIndex];
    cout << randomChar;
    currentSeed = currentSeed.substr(1) + randomChar;
    totalCharsUsed++; // we should not exceed max chars(2000)
}