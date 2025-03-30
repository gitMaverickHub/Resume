#include "characters.hpp"

Character::Character(string moniker, vector<string> combatClass, vector<string> powers, vector<string> groups, vector<string> place, vector<string> genetics, vector<string> subgenetics) {
    name = moniker;
    combatRole = combatClass;
    magics = powers;
    affiliations = groups;
    locations = place;
    species = genetics;
    races = subgenetics;
}

string Character::getName() {
    return name;
}

vector<string> Character::getCombatRole() {
    return combatRole;
}

vector<string> Character::getMagics() {
    return magics;
}

vector<string> Character::getAffiliations() {
    return affiliations;
}

vector<string> Character::getLocations() {
    return locations;
}

vector<string> Character::getSpecies() {
    return species;
}

vector<string> Character::getRaces() {
    return races;
}

void Character::print() {
    cout << left << setw(25) << setfill(' ') << name;
    cout << "\033[36m"; // change color; refer to https://en.wikipedia.org/wiki/ANSI_escape_code
    int rolesize = size(combatRole);
    int i = 0;
    while (i < rolesize) {
        cout << "|";
        cout << left << setw(25) << setfill(' ') << combatRole[i];
        i++;
    }
    cout << "\033[35m";
    int magicssize = size(magics);
    i = 0;
    while (i < magicssize) {
        cout << "|";
        cout << left << setw(25) << setfill(' ') << magics[i];
        i++;
    }
    cout << "\033[32m";
    int affiliationssize = size(affiliations);
    i = 0;
    while (i < affiliationssize) {
        cout << "|";
        cout << left << setw(25) << setfill(' ') << affiliations[i];
        i++;
    }
    cout << "\033[34m";
    int locationssize = size(locations);
    i = 0;
    while (i < locationssize) {
        cout << "|";
        cout << left << setw(25) << setfill(' ') << locations[i];
        i++;
    }
    cout << "\033[31m";
    int speciessize = size(species);
    i = 0;
    while (i < speciessize) {
        cout << "|";
        cout << left << setw(25) << setfill(' ') << species[i];
        i++;
    }
    cout << "\033[33m";
    int racessize = size(races);
    i = 0;
    while (i < racessize) {
        cout << "|";
        cout << left << setw(25) << setfill(' ') << races[i];
        i++;
    }
    cout << "\n\n\033[0m";
}

void Character::print(int ref) {
    cout << "[" << ref << "]";
    cout << left << setw(25) << setfill(' ') << name;
    cout << "\033[36m"; // change color; refer to https://en.wikipedia.org/wiki/ANSI_escape_code
    int rolesize = size(combatRole);
    int i = 0;
    while (i < rolesize)
    {
        cout << "[" << ref << "]|";
        cout << left << setw(25) << setfill(' ') << combatRole[i];
        i++;
    }
    cout << "\033[35m";
    int magicssize = size(magics);
    i = 0;
    while (i < magicssize)
    {
        cout << "[" << ref << "]|";
        cout << left << setw(25) << setfill(' ') << magics[i];
        i++;
    }
    cout << "\033[32m";
    int affiliationssize = size(affiliations);
    i = 0;
    while (i < affiliationssize)
    {
        cout << "[" << ref << "]|";
        cout << left << setw(25) << setfill(' ') << affiliations[i];
        i++;
    }
    cout << "\033[34m";
    int locationssize = size(locations);
    i = 0;
    while (i < locationssize)
    {
        cout << "[" << ref << "]|";
        cout << left << setw(25) << setfill(' ') << locations[i];
        i++;
    }
    cout << "\033[31m";
    int speciessize = size(species);
    i = 0;
    while (i < speciessize)
    {
        cout << "[" << ref << "]|";
        cout << left << setw(25) << setfill(' ') << species[i];
        i++;
    }
    cout << "\033[33m";
    int racessize = size(races);
    i = 0;
    while (i < racessize)
    {
        cout << "[" << ref << "]|";
        cout << left << setw(25) << setfill(' ') << races[i];
        i++;
    }
    cout << "\n\n\033[0m";
}

CharactersTable::CharactersTable() {
    vector<Character> myVector;
    charactersTable = myVector;
}

void CharactersTable::insert(Character person) {
    if (size(charactersTable) < 1) {
        charactersTable.insert(charactersTable.begin(), person);
    }
    else if (size(charactersTable) == 1) {
        if (person.getName() < charactersTable[0].getName()) {
            charactersTable.insert(charactersTable.begin(), person);
        }
        else {
            charactersTable.insert(charactersTable.end(), person);
        }
    } 
    else {
        string personName = person.getName();
        int intLower = 0;
        int intMid = (size(charactersTable) - 1) / 2;
        int intUpper = size(charactersTable) - 1;
        string lower = charactersTable[intLower].getName();
        string mid = charactersTable[intMid].getName();
        string upper = charactersTable[intUpper].getName();
        if (personName < lower) {
            charactersTable.insert(charactersTable.begin(), person);
        }
        else if (personName >= upper) {
            charactersTable.insert(charactersTable.end(), person);
        }
        else {
            bool insert = false;
            while (insert == false) {
                if (personName == lower) {
                    charactersTable.insert(charactersTable.begin() + 1, person);
                    insert = true;
                }
                else if (personName == mid) {
                    charactersTable.insert(charactersTable.begin() + intMid + 1, person);
                    insert = true;
                }
                else if (personName == upper) {
                    charactersTable.insert(charactersTable.end(), person);
                    insert = true;
                }
                else if ((intUpper - intLower) == 1) {
                    charactersTable.insert(charactersTable.begin() + intLower + 1, person);
                    insert = true;
                }
                else if (personName < mid) {
                    intUpper = intMid;
                    upper = mid;
                    intMid = intLower + ((intUpper - intLower) / 2);
                    mid = charactersTable[intMid].getName();
                }
                else {
                    intLower = intMid;
                    lower = mid;
                    intMid = intLower + ((intUpper - intLower) / 2);
                    mid = charactersTable[intMid].getName();
                }
            }
        }
    }
}

void CharactersTable::filter(char filterby, string filter) {
    for (int i = 0; i < size(charactersTable); i++) {
        bool del = true;
        switch(filterby) {
            case '0':
                if (charactersTable[i].getName() != filter) {
                    charactersTable.erase(charactersTable.begin()+i);
                    i--;
                }
                break;
            case '1':
                for (int j = 0; j < size(charactersTable[i].getCombatRole()); j++) {
                    if (charactersTable[i].getCombatRole()[j] == filter) {
                        del = false;
                    }
                }
                if (del == true) {
                    charactersTable.erase(charactersTable.begin() + i);
                    i--;
                }
                break;
            case '2':
                for (int j = 0; j < size(charactersTable[i].getMagics()); j++) {
                    if (charactersTable[i].getMagics()[j] == filter) {
                        del = false;
                    }
                }
                if (del == true) {
                    charactersTable.erase(charactersTable.begin() + i);
                    i--;
                }
                break;
            case '3':
                for (int j = 0; j < size(charactersTable[i].getAffiliations()); j++) {
                    if (charactersTable[i].getAffiliations()[j] == filter) {
                        del = false;
                    }
                }
                if (del == true) {
                    charactersTable.erase(charactersTable.begin() + i);
                    i--;
                }
                break;
            case '4':
                for (int j = 0; j < size(charactersTable[i].getLocations()); j++) {
                    if (charactersTable[i].getLocations()[j] == filter) {
                        del = false;
                    }
                }
                if (del == true) {
                    charactersTable.erase(charactersTable.begin() + i);
                    i--;
                }
                break;
            case '5':
                for (int j = 0; j < size(charactersTable[i].getSpecies()); j++) {
                    if (charactersTable[i].getSpecies()[j] == filter) {
                        del = false;
                    }
                }
                if (del == true) {
                    charactersTable.erase(charactersTable.begin() + i);
                    i--;
                }
                break;
            case '6':
                for (int j = 0; j < size(charactersTable[i].getRaces()); j++) {
                    if (charactersTable[i].getRaces()[j] == filter) {
                        del = false;
                    }
                }
                if (del == true) {
                    charactersTable.erase(charactersTable.begin() + i);
                    i--;
                }
                break;
            default:
                cout << "Not an option. Please pick a correct option.";
        }
    }
}

void CharactersTable::print() {
    cout << "\n\n\n";
    int tableSize = size(charactersTable);
    int i = 0;
    while (i < tableSize) {
        charactersTable[i].print(i);
        i++;
    }
    cout << "\n\n";
}

Program::Program(CharactersTable originalTable) {
    data = {originalTable};
    currPos = 0;
}

void Program::showOriginal() {
    data[0].print();
}

void Program::insert() {
    data.insert(data.begin() + currPos + 1, data[currPos]);
    currPos++;
    while (size(data) > (currPos+1)) {
        erase();
    }
}

void Program::erase() {
    data.erase(data.end());
}

void Program::undo() {
    if (currPos > 0) {
        currPos--;
    }
    else {
        cout << "\nCan not undo any further.";
    }
}

void Program::redo() {
    if (currPos + 1 < size(data)) {
        currPos++;
    }
    else {
        cout << "\nCan not redo any further.";
    }
}

void Program::print() {
    data[currPos].print();
}

void Program::start() {
    bool end = false;
    while (end == false) {
        cout << "\nChoose an action:\n\n[1]Print Full List\n[2]Filter List\n[3]Undo\n[4]Redo\n[5]Print\n[6]Restart\n[7]End Program\n\n";
        char choice;
        cin >> choice;
        string filterWith; // Declared here because switch is weird and wouldn't let me declare it in the scope of the case 2
        switch (choice) {
            case '1':
                showOriginal();
                break;
            case '2': 
                insert();
                cout << "\nChoose a filter:\n\n[0]Name\n[1]Combat Role\n[2]Magic(s)\n[3]Affiliation(s)\n[4]Location(s)\n[5]Species\n[6]Race(s)\n\n";
                char toFilter;
                cin >> toFilter;
                cout << "\nChoose what to find. Be mindful of the spelling and punctuation.\n\n";
                cin >> filterWith;
                data[currPos].filter(toFilter, filterWith);
                break;
            case '3':
                undo();
                break;
            case '4':
                redo();
                break;
            case '5':
                print();
                break;
            case '6':
                currPos = 0;
                break;
            case '7':
                cout << "\n";
                end = true;
                break;
            default:
                cout << "\nPlease choose a correct option.";
                break;
        }
    }
}