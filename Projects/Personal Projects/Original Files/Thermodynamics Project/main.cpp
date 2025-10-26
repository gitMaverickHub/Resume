#include "ThermodynamicCode.cpp"

int main() {

    fstream inputFile;

//----------------------------------------------------------------------------------------------------------------------------------
//  Creating the Input.csv file if it does not already exist

    if (!fs::exists("Input.csv")) {
        inputFile.open("Input.csv", ios::out);
        if (!inputFile.is_open()) {
            cerr << "Failed to create file!" << endl;
            return 1;
        }
        inputFile << ",Velocity (km/h)\n";
        inputFile << ",Altitude (m)\n";
        inputFile << "Compressor,Inlet Area (m^2)\n,Number of Stages\n,Stage Efficiencies\n,Overall Pressure Ratio\n";
        inputFile << "Combustor,Fuel Heating Value (kJ/kg)\n";
        inputFile << "Turbine,Number of Stages\n,Stage Efficiencies\n";
        inputFile << "Nozzle,Exit Area (m^2)\n,Efficiency";
        cout << "An Input.csv file has been created in the current directory.";
        inputFile.close();
    }

//----------------------------------------------------------------------------------------------------------------------------------
//Waiting for the values to be put into the Input.csv file before continuing

    cout << "\nAn Input.csv file was created in the current directory if it did not already exist.\n"
         << "Put the desired input values into their respective locations in the 3rd column of the Input.csv file i.e. the C column when using excel.\n"
         << "Do not put any commas in the numbers. For instance, 1000 should be put in instead of 1,000.\n"
         << "All of the inputs must be positive.\n"
         << "The altitude can range from 0 to 10000 meters.\n"
         << "The pressure ratio must be greater than 1.\n"
         << "The stage efficiencies can range from 0 to 1.\n"
         << "The number of stages for the compressor and the tubrine must be integers.\n"
         << "Are the desired values in the Input.csv file?\n\n"
         << "Enter 'Y' for yes to continue.\n"
         << "Enter 'e' for exit to exit the program.\n";

    string continueString;

    while (1) {
        cin >> continueString;
        if (continueString == "Y") break;
        else if (continueString == "e") return 0;
        else cout << "\nEnter 'Y' for yes to continue.\n"
                  << "Enter 'e' for exit to exit the program.\n";
        
    }

    
//----------------------------------------------------------------------------------------------------------------------------------
//Taking the data, from the Input.csv file and putting it a Cycle object. The constructor automatically calculates, ptints, and plots.
//Finally, it waits for user's input for the program to be exited or reran.

    bool stopBool = 0;

    while (!stopBool) {

        vector<string> data;

        while(1) {
            inputFile.open("Input.csv", ios::in);
            if (!inputFile.is_open()) {
                cerr << "Failed to create or open Input.csv file!" << endl;
                return 1;
            }
            vector<string> column_data;
            string row;
            int target_column_index = 2;
            for (int i = 0; i < 11; i++) {
                getline(inputFile, row);
                stringstream ss(row);
                string cell;
                int current_column = 0;
                while (getline(ss, cell, ',')){
                    if (current_column >= target_column_index){
                        column_data.push_back(cell);
                    }
                    current_column++;
                }
            }
            inputFile.close();
            if (column_data.size() == 11 &&
                0 < stod(column_data[0]) &&
                0 < stod(column_data[1]) && stod(column_data[1]) <= 10000 &&
                0 < stod(column_data[2]) &&
                0 < stod(column_data[3]) &&
                floor(stod(column_data[3])) == stod(column_data[3]) &&
                0 < stod(column_data[4]) && stod(column_data[4]) <= 1 &&
                1 < stod(column_data[5]) &&
                0 < stod(column_data[6]) &&
                0 < stod(column_data[7]) &&
                floor(stod(column_data[7])) == stod(column_data[7]) &&
                0 < stod(column_data[8]) && stod(column_data[8]) <= 1 &&
                0 < stod(column_data[9]) &&
                0 < stod(column_data[10]) && stod(column_data[10]) <= 1) 
            {
                data = column_data;
                break;
            }
            else {
                cout << "\nReenter your values into the input.csv file making sure the values are in the correct cells.\n"
                     << "Do not put any commas in the numbers. For instance, 1000 should be put in instead of 1,000.\n"
                     << "All of the inputs must be positive\n"
                     << "The altitude can range from 0 to 10000 meters.\n"
                     << "The pressure ratio must be greater than 1.\n"
                     << "The stage efficiencies can range from 0 to 1.\n"
                     << "The number of stages for the compressor and the tubrine must be integers.\n";
                bool readyBool = 0;
                string readyString;
                while(!readyBool) {
                    cout << "\nEnter 'r' for ready for when the values have been put in correctly to continue the program.\n";
                    cout << "Enter 'e' to exit.\n";
                    cin >> readyString;
                    if (readyString == "r") readyBool = 1;
                    else if (readyString == "e") return 0;
                }
            }
        }

        Cycle cycle(data);

        while(1){
            cout << "\nEnter new  data into the Input.csv file and enter'r' in the terminal to rerun the program using the new data, or enter 'e' to exit.\n";
            cout << "If you are viewing or using the date in the Output.csv file, make sure to close the file before rerunning, or the Output.csv file will fail "
                 << "to open, and the date will not be updated.\n\n";
            cin >> continueString;
            if (continueString == "r") break;
            else if (continueString == "e") {
                stopBool = 1; 
                break;
            }
        }
    }
}