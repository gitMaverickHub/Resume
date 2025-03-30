#ifndef CHARACTERS_H
#define CHARACTERS_H
#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <iomanip>
using namespace std;

class Character {

    private:
        string name;
        vector<string> combatRole;
        vector<string> magics;
        vector<string> affiliations;
        vector<string> locations;
        vector<string> species;
        vector<string> races;
        // string link; // this will be the link to the appropriate character slide

    public:
        Character(string moniker, vector<string> combatClass, vector<string> powers, vector<string> groups, vector<string> place, vector<string> genetics, vector<string> subgenetics);
        string getName();
        vector<string> getCombatRole();
        vector<string> getMagics();
        vector<string> getAffiliations();
        vector<string> getLocations();
        vector<string> getSpecies();
        vector<string> getRaces();
        void print();
        void print(int ref);
};

class CharactersTable {

    private:
        vector<Character> charactersTable;

    public:
        CharactersTable();
        void insert(Character person);
        void filter(char filterby, string filter);
        void print();
};

class Program {

    private:
        vector<CharactersTable> data;
        int currPos;

    public:
        Program(CharactersTable originalTable);
        void showOriginal();
        void insert();
        void erase();
        void undo();
        void redo();
        void print();
        void start();

};

#endif