/* 
    Using a map does not work for this instance because there is not a constructor for a map with the second parameter, the value, being the type, array<vector<string>, 2>
    While adding new character, try to alphabetize the various lists eg Character example = Character("exampleCharacter", {"a","b","c"},{"a","d","e"},...)

    Find a way to make the terminal print indefinitely to the right rather than start a new line. Also adjust setwidth size. Find a way to set links. Set a way to exit filter early.
    Also make a way to input classes for places and subraces where if I put Hill Dwarf, Dwarf is autopopulated and if I put Kror Forest Faedron is autopopulated
    Consider things like this with magics as well and combat roles.

    Find a way to print only desired data. For instance if I only want to know about a characters place and combat role, it prints only that

    Possibly make a template. Possibly make a recipes class similar to characters class and make an app from that. This would be good c++ project, app making, and web development ie c++, html, css, and javascript practice

    Possibly use multithreading if applicable

    The output needs to be made neater

    Turn Lionheart into a full website at some point with all the pages including character slides, location, locations slides, a home page, etc. Do not spend more time on this, but this a good template for how I would like a characters page structured. Make sure to add hyperlinks.
*/

#include "characters.cpp"

int main() {

    Character krakxulKror = Character("Krak'xul Kror", {"Lancer", "Knight"},{"Electromancy", "Physical Sourcery", "Polymorphism", "Enhancing", "Aeromancy", "Pyromancy", "Hydromancy"},{"Children of Kror", "Warlock Slayers"},{"Faedron", "Kror Forest", "Dunuth"},{"Elf"},{"Wild Elf"});
    Character dahliaKror = Character("Dahlia Kror", {"Augmentor", "Combat Medic", "Lancer", "Flex"},{"Polymorphism", "Healing", "Debilitating", "Aeromancy", "Cryomnacy", "Electromancy", "Hydromancy", "Thermomancy", "Pyromancy"},{"Children of Kror", "Warlock Slayers"},{"Faedron", "Kror Forest", "Dunuth"},{"Human"},{"Faedronic Human"});
    Character elijahKror = Character("Elijah Kror", {"Bulwark", "Fighter"}, {"Physcial Sourcery", "Polymorphism", "Enhancing", "Hex"}, {"Children of Kror", "Warlock Slayers"}, {"Faedron", "Kror Forest", "Dunuth"}, {"Dwarf"}, {"Hill Dwarf"});
    Character daevenKror = Character("Daeven Kror", {"Knight"}, {"Conjuration", "Hex"}, {"Children of Kror", "Warlock Slayers"}, {"Faedron", "Kror Forest", "Dunuth"}, {"Reptilian"}, {"Black Drakonian Reptilian"});

    CharactersTable ogTable = CharactersTable();

    ogTable.insert(krakxulKror);
    ogTable.insert(dahliaKror);
    ogTable.insert(elijahKror);
    ogTable.insert(daevenKror);

    Program program = Program(ogTable);
    program.start();
    
}
