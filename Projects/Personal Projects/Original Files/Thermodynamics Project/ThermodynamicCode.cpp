#include "ThermodynamicCode.hpp"
#include <sstream>
#include <cmath>
#include <iomanip>
#include <filesystem>
//#include <matplot/matplot.h>
namespace fs = filesystem;

Cycle::Cycle(vector<string> data) {

    velocity = stod(data[0])/3.6;
    altitude = stod(data[1]);
    A_in = stod(data[2]);
    num_of_comp_stages = stoi(data[3]);
    comp_stage_efficiencies = stod(data[4]);
    pressure_ratio = stod(data[5]);
    heating_value = stod(data[6]);
    num_of_turbine_stages = stoi(data[7]);
    turb_stage_efficiencies = stod(data[8]);
    A_out = stod(data[9]);
    nozz_efficiency = stod(data[10]);

    cycleAS();
    cycleCAS();

}

void Cycle::cycleAS() {

    calculateAS();
    printAS();
    output2fileAS();
    plotAS();

}

void Cycle::calculateAS() {

    precompressorAS();
    compressorAS();
    combustorAS();
    turbineAS();
    nozzleAS(); //This one is to find the state without the afterburner
    thrust_wo_abAS = calc_thrustAS();
    afterburnerAS();
    nozzleAS(); //This is to the find the state with the afterburner
    thrust_w_abAS = calc_thrustAS();

}

void Cycle::precompressorAS() {
    while(1) {
        InitialPT ptTable;
        tuple initialpt = ptTable.interpolate(altitude);
        pressureAS.push_back(get<0>(initialpt));
        temperatureAS.push_back(get<1>(initialpt));
        break;
    }
    while(1) {
        AirStandard asTable;
        enthalpyAS.push_back(asTable.interpolate(temperatureAS.back(), 0, 1));
        break;
    }
    sMethod();
    specific_volumeAS.push_back(R * temperatureAS.back() / pressureAS.back());
    air_mass_flow_rateAS = A_in * velocity / specific_volumeAS.back();
}

void Cycle::compressorAS() {
    AirStandard asTable;
    for (int i = 0; i < num_of_comp_stages; i++) {
        entropyAS.push_back(entropyAS.back());
        pressureAS.push_back(pressureAS.front()*(1 + (i+1)*(pressure_ratio-1)/num_of_comp_stages));
        hMethod();
        temperatureAS.push_back(asTable.interpolate(enthalpyAS.back(), 1, 0));
        specific_volumeAS.push_back(R * temperatureAS.back() / pressureAS.back());
        pressureAS.push_back(pressureAS.back());
        double h1 = enthalpyAS[enthalpyAS.size()-2];
        double h2i = enthalpyAS[enthalpyAS.size()-1]; //i.e. enthalpyAS.back()
        double h2 = (h2i-h1)/comp_stage_efficiencies + h1;
        enthalpyAS.push_back(h2);
        sMethod();
        temperatureAS.push_back(asTable.interpolate(enthalpyAS.back(), 1, 0));
        specific_volumeAS.push_back(R * temperatureAS.back() / pressureAS.back());
    }
}

void Cycle::combustorAS() {
    pressureAS.push_back(pressureAS.back());
    double fuel_mass_flow_rate = (500 + 55 * air_mass_flow_rateAS)/3600;
    enthalpyAS.push_back(enthalpyAS.back()+(heating_value*fuel_mass_flow_rate/air_mass_flow_rateAS));
    sMethod();
    AirStandard asTable;
    temperatureAS.push_back(asTable.interpolate(enthalpyAS.back(), 1, 0));
    specific_volumeAS.push_back(R * temperatureAS.back() / pressureAS.back());
    exit_mass_flow_rateAS = air_mass_flow_rateAS + fuel_mass_flow_rate;
}

void Cycle::turbineAS() {
    AirStandard asTable;
    for (int i = 0; i < num_of_turbine_stages; i++) {
        entropyAS.push_back(entropyAS.back());
        double new_pressure = pressureAS[num_of_comp_stages*2+1] - (pressureAS[num_of_comp_stages*2+1]-pressureAS.front()) * (i+1) / (num_of_turbine_stages+1);
        pressureAS.push_back(new_pressure);
        hMethod();
        temperatureAS.push_back(asTable.interpolate(enthalpyAS.back(), 1, 0));
        specific_volumeAS.push_back(R * temperatureAS.back() / pressureAS.back());
        pressureAS.push_back(pressureAS.back());
        double h3 = enthalpyAS[enthalpyAS.size()-2];
        double h4i = enthalpyAS[enthalpyAS.size()-1]; //i.e. enthalpy.back()
        double h4 = (h4i-h3)*turb_stage_efficiencies + h3;
        enthalpyAS.push_back(h4);
        sMethod();
        temperatureAS.push_back(asTable.interpolate(enthalpyAS.back(), 1, 0));
        specific_volumeAS.push_back(R * temperatureAS.back() / pressureAS.back());
    }
}

void Cycle::nozzleAS() {
    entropyAS.push_back(entropyAS.back());
    pressureAS.push_back(pressureAS.front());
    hMethod();
    AirStandard asTable;
    temperatureAS.push_back(asTable.interpolate(enthalpyAS.back(), 1, 0));
    specific_volumeAS.push_back(R * temperatureAS.back() / pressureAS.back());
    pressureAS.push_back(pressureAS.back());
    double c5i = sqrt(2000*(enthalpyAS[enthalpyAS.size()-2]-enthalpyAS[enthalpyAS.size()-1]));
    velocity_exitAS = c5i * nozz_efficiency;
    double new_enthalpy = enthalpyAS[enthalpyAS.size()-2] - (velocity_exitAS*velocity_exitAS/2000);
    enthalpyAS.push_back(new_enthalpy);
    sMethod();
    temperatureAS.push_back(asTable.interpolate(enthalpyAS.back(), 1, 0));
    specific_volumeAS.push_back(R * temperatureAS.back() / pressureAS.back());
}

void Cycle::afterburnerAS() {
    pressureAS.push_back(pressureAS[pressureAS.size()-3]);
    double fuel_mass_flow_rate = (-400 + (110 * air_mass_flow_rateAS))/3600;
    enthalpyAS.push_back(enthalpyAS[enthalpyAS.size() - 3]+(heating_value*fuel_mass_flow_rate/air_mass_flow_rateAS));
    sMethod();
    AirStandard asTable;
    temperatureAS.push_back(asTable.interpolate(enthalpyAS.back(), 1, 0));
    specific_volumeAS.push_back(R * temperatureAS.back() / pressureAS.back());
    exit_mass_flow_rateAS = air_mass_flow_rateAS + fuel_mass_flow_rate;
}

double Cycle::calc_thrustAS() {
    double thrust = exit_mass_flow_rateAS*velocity_exitAS + A_out*pressureAS.back()*1000 - (air_mass_flow_rateAS*velocity + A_out*pressureAS.front()*1000);
    return thrust/1000;
}

void Cycle::sMethod() {
    AirStandard asTable;
    double entropy_prime = asTable.interpolate(enthalpyAS.back(), 1, 3);
    entropyAS.push_back(entropy_prime-R*log(pressureAS.back()));
}

void Cycle::hMethod() {
    double entropy_prime = entropyAS.back() + R*log(pressureAS.back());
    AirStandard asTable;
    enthalpyAS.push_back(asTable.interpolate(entropy_prime, 3, 1));
}

void Cycle::printAS() {
    cout << "\nAir Standard:";
    cout << "\n\t\t\t\t\t\t|Temperature(K)\t\t|Entropy(kJ/(kg*K))\t|Pressure(kPa)\t\t|Specific Volume(m^3/kg)\t\t|Enthalpy(kJ/kg)\n";
    cout << "State 0: "; if (!print_rowAS(0)) return; 
    int i = 1;
    for (i; i < num_of_comp_stages+1; i++) {
        print_line();
        cout << "State " << i << "i:"; if (!print_rowAS(2*i-1)) return;
        cout << "State " << i << ":"; if (!print_rowAS(2*i)) return;
    }
    print_line();
    cout << "State " << i << ":"; if (!print_rowAS(2*i-1)) return;
    int j = i;
    for (j; j < num_of_turbine_stages+i; j++) {
        print_line();
        cout << "State " << j+1 << "i:"; if (!print_rowAS(2*j)) return;
        cout << "State " << j+1 << ":"; if (!print_rowAS(2*j+1)) return;
    }
    print_line();
    cout << "State " << j+1 << "i:"; if (!print_rowAS(2*j)) return;
    cout << "State " << j+1 << ":"; if (!print_rowAS(2*j+1)) return;
    print_line();
    cout << "State " << j+1 << " (with afterburner):"; if (!print_rowAS(2*j+2)) return;
    print_line();
    cout << "State " << j+2 << "i (with afterburner):"; if (!print_rowAS(2*j+3)) return;
    cout << "State " << j+2 << " (with afterburner):"; if (!print_rowAS(2*j+4)) return;
    cout << "\nThrust (without afterburner): " << thrust_wo_abAS << " kN" << endl;
    cout << "Thrust (with afterburner): " << thrust_w_abAS << " kN" << endl;
}

bool Cycle::print_rowAS(int i) {        
    
    /*
    The print row function is not only a function to print the rows, but it is a bool function that will be used to exit out out of the print function early.
    If it returns true, that means it is good to continue, and the print function will continue.
    */

    if (i < temperatureAS.size()-3) cout << "\t\t\t\t\t|";
    else cout << "\t\t\t|";

    bool cont = 1;

    if (temperatureAS[i] < 0) {
        cout << "-\t\t\t|";
        cont = 0;
    }
    else cout << fixed << setprecision(3) << temperatureAS[i] << "\t\t|";

    if (entropyAS[i] < 0) {
        cout << "-\t\t\t|";
        cont = 0;
    }
    else cout << fixed << setprecision(5) << entropyAS[i] << "\t\t|";

    if (pressureAS[i] < 0) {
        cout << "-\t\t\t|";
        cont = 0;
    }
    else if (pressureAS[i] >= 1000) cout << fixed << setprecision(2) << pressureAS[i] << "\t\t|";
    else cout << fixed << setprecision(2) << pressureAS[i] << "\t\t\t|";

    if (specific_volumeAS[i] < 0) {
        cout << "-\t\t\t\t\t|";
        cont = 0;
    }
    else cout << fixed << setprecision(5) << specific_volumeAS[i] << "\t\t\t\t|";

    if (enthalpyAS[i] < 0) {
        cout << "-";
        cont = 0;
    }
    else cout << fixed << setprecision(3) << enthalpyAS[i] << endl;

    return cont;

}

void Cycle::print_line() {
    for (int i = 0; i < 180; i++) {
        cout << "-";
    }
    cout << endl;
}

void Cycle::output2fileAS() {
    ofstream outputFile;
    outputFile.open("OutputAS.csv", ios::out);
    if (!outputFile.is_open())
        cerr << "Failed to create or open OutputAS.csv file!\n If the OutputAS.csv has already been created, make sure it is closed before running the program." << endl;
    outputFile << ",Temperature (K), Entropy (kJ/(kg*K)), Pressure (kPa), Specific Volume (m^3/kg), Enthalpy (kJ/kg)\n";
    outputFile << "State 0,"; if (!output2file_rowAS(outputFile, 0)) return;
    int i = 1;
    for (i; i < num_of_comp_stages+1; i++) {
        outputFile << "State " << i << "i:,"; if (!output2file_rowAS(outputFile, 2*i-1)) return;
        outputFile << "State " << i << ":,"; if (!output2file_rowAS(outputFile, 2*i)) return;
    }
    outputFile << "State " << i << ":,"; if (!output2file_rowAS(outputFile, 2*i-1)) return;
    int j = i;
    for (j; j < num_of_turbine_stages+i; j++) {
        outputFile << "State " << j+1 << "i:,"; if (!output2file_rowAS(outputFile, 2*j)) return;
        outputFile << "State " << j+1 << ":,"; if (!output2file_rowAS(outputFile, 2*j+1)) return;
    }
    outputFile << "State " << j+1 << "i:,"; if (!output2file_rowAS(outputFile, 2*j)) return;
    outputFile << "State " << j+1 << ":,"; if (!output2file_rowAS(outputFile, 2*j+1)) return;
    outputFile << "State " << j+1 << " (with afterburner):,"; if (!output2file_rowAS(outputFile, 2*j+2)) return;
    outputFile << "State " << j+2 << "i (with afterburner):,"; if (!output2file_rowAS(outputFile, 2*j+3)) return;
    outputFile << "State " << j+2 << " (with afterburner):,"; if (!output2file_rowAS(outputFile, 2*j+4)) return;
    outputFile << "\nThrust (without afterburner): " << thrust_wo_abAS << " kN" << endl;
    outputFile << "Thrust (with afterburner): " << thrust_w_abAS << " kN" << endl;
}

bool Cycle::output2file_rowAS(ofstream& ofile, int i) {

    //This function is similar to the print rows functions

    bool cont = 1;

    if (temperatureAS[i] < 0){
        ofile << "-,";
        cont = 0;
    }
    else ofile << temperatureAS[i] << ",";

    if (entropyAS[i] < 0)
    {
        ofile << "-,";
        cont = 0;
    }
    else ofile << entropyAS[i] << ",";

    if (pressureAS[i] < 0)
    {
        ofile << "-,";
        cont = 0;
    }
    else ofile << pressureAS[i] << ",";

    if (specific_volumeAS[i] < 0)
    {
        ofile << "-,";
        cont = 0;
    }
    else ofile << specific_volumeAS[i] << ",";

    if (enthalpyAS[i] < 0)
    {
        ofile << "-,";
        cont = 0;
    }
    else ofile << enthalpyAS[i] << endl;

    return cont;

}

void Cycle::plotAS() {
    return;
}

void Cycle::cycleCAS() {

    calculateCAS();
    printCAS();
    output2fileCAS();
    plotCAS();

}

void Cycle::calculateCAS() {

    precompressorCAS();
    compressorCAS();
    combustorCAS();
    turbineCAS();
    nozzleCAS();
    thrust_wo_abCAS = calc_thrustCAS();
    afterburnerCAS();
    nozzleCAS();
    thrust_w_abCAS = calc_thrustCAS();

}

void Cycle::precompressorCAS() {
    while (1) {
        InitialPT ptTable;
        tuple initialpt = ptTable.interpolate(altitude);
        pressureCAS.push_back(get<0>(initialpt));
        temperatureCAS.push_back(get<1>(initialpt));
        break;
    }
    enthalpyCAS.push_back(cp*temperatureCAS[0]);
    entropyCAS.push_back(entropyAS[0]); // Since I do not know the reference point that the AS Tables used to find entropy, I will make the first entropy values equal to make comparison of the data easier
    specific_volumeCAS.push_back(R * temperatureCAS.back() / pressureCAS.back());
    air_mass_flow_rateCAS = A_in * velocity / specific_volumeCAS.back();
}

void Cycle::compressorCAS() {
    for (int i=0; i < num_of_comp_stages; i++) {
        entropyCAS.push_back(entropyCAS.back());
        pressureCAS.push_back(pressureCAS.front() * (1 + (i+1) * (pressure_ratio-1) / num_of_comp_stages));
        isen_rel_temp_from_pressure();
        enthalpyCAS.push_back(cp*temperatureCAS.back());
        specific_volumeCAS.push_back(R * temperatureCAS.back() / pressureCAS.back());
        pressureCAS.push_back(pressureCAS.back());
        double h1 = enthalpyCAS[enthalpyCAS.size()-2];
        double h2i = enthalpyCAS[enthalpyCAS.size()-1]; //i.e. enthalpyCAS.back()
        double h2 = (h2i-h1)/comp_stage_efficiencies + h1;
        enthalpyCAS.push_back(h2);
        temperatureCAS.push_back(enthalpyCAS.back()/cp);
        entropy_equation();
        specific_volumeCAS.push_back(R * temperatureCAS.back() / pressureCAS.back());
    }
}

void Cycle::combustorCAS() {
    pressureCAS.push_back(pressureCAS.back());
    double fuel_mass_flow_rate = (500 + 55 * air_mass_flow_rateCAS) / 3600;
    enthalpyCAS.push_back(enthalpyCAS.back() + (heating_value * fuel_mass_flow_rate / air_mass_flow_rateCAS));
    temperatureCAS.push_back(enthalpyCAS.back()/cp);
    entropy_equation();
    specific_volumeCAS.push_back(R * temperatureCAS.back() / pressureCAS.back());
    exit_mass_flow_rateCAS = air_mass_flow_rateCAS + fuel_mass_flow_rate;
}

void Cycle::turbineCAS() {
    for (int i = 0; i < num_of_turbine_stages; i++) {
        entropyCAS.push_back(entropyCAS.back());
        double new_pressure = pressureCAS[num_of_comp_stages*2+1] - (pressureCAS[num_of_comp_stages*2+1]-pressureCAS.front()) * (i+1) / (num_of_turbine_stages+1);
        pressureCAS.push_back(new_pressure);
        isen_rel_temp_from_pressure();
        enthalpyCAS.push_back(cp*temperatureCAS.back());
        specific_volumeCAS.push_back(R * temperatureCAS.back() / pressureCAS.back());
        pressureCAS.push_back(pressureCAS.back());
        double h3 = enthalpyCAS[enthalpyCAS.size()-2];
        double h4i = enthalpyCAS[enthalpyCAS.size()-1]; // i.e. enthalpyCAS.back()
        double h4 = (h4i-h3) * turb_stage_efficiencies + h3;
        enthalpyCAS.push_back(h4);
        temperatureCAS.push_back(enthalpyCAS.back()/cp);
        entropy_equation();
        specific_volumeCAS.push_back(R * temperatureCAS.back() / pressureCAS.back());
    }
}

void Cycle::nozzleCAS() {
    entropyCAS.push_back(entropyCAS.back());
    pressureCAS.push_back(pressureCAS.front());
    isen_rel_temp_from_pressure();
    enthalpyCAS.push_back(cp*temperatureCAS.back());
    specific_volumeCAS.push_back(R * temperatureCAS.back() / pressureCAS.back());
    pressureCAS.push_back(pressureCAS.back());
    double c5i = sqrt(2000 * (enthalpyCAS[enthalpyCAS.size() - 2] - enthalpyCAS[enthalpyCAS.size() - 1]));
    velocity_exitCAS = c5i * nozz_efficiency;
    double new_enthalpy = enthalpyCAS[enthalpyCAS.size() - 2] - (velocity_exitCAS * velocity_exitCAS / 2000);
    enthalpyCAS.push_back(new_enthalpy);
    temperatureCAS.push_back(enthalpyCAS.back()/cp);
    entropy_equation();
    specific_volumeCAS.push_back(R * temperatureCAS.back() / pressureCAS.back());
}

void Cycle::afterburnerCAS() {
    pressureCAS.push_back(pressureCAS[pressureCAS.size() - 3]);
    double fuel_mass_flow_rate = (-400 + (110 * air_mass_flow_rateCAS)) / 3600;
    enthalpyCAS.push_back(enthalpyCAS[enthalpyCAS.size() - 3] + (heating_value * fuel_mass_flow_rate / air_mass_flow_rateCAS));
    temperatureCAS.push_back(enthalpyCAS.back()/cp);
    entropy_equation();
    specific_volumeCAS.push_back(R * temperatureCAS.back() / pressureCAS.back());
    exit_mass_flow_rateCAS = air_mass_flow_rateCAS + fuel_mass_flow_rate;
}

double Cycle::calc_thrustCAS() {
    double thrust = exit_mass_flow_rateCAS * velocity_exitCAS + A_out * pressureCAS.back() * 1000 - (air_mass_flow_rateCAS * velocity + A_out * pressureCAS.front() * 1000);
    return thrust / 1000;
}

void Cycle::entropy_equation() {

    double s1 = entropyCAS.back();
    double T1 = temperatureCAS[temperatureCAS.size()-2];
    double T2 = temperatureCAS[temperatureCAS.size()-1]; //i.e. temperatureCAS.back()
    double p1 = pressureCAS[pressureCAS.size()-2];
    double p2 = pressureCAS[pressureCAS.size()-1]; //i.e. pressureCAS.back()
    double s2 = cp*log(T2/T1) - R*log(p1/p2) + s1;
    entropyCAS.push_back(s2);

}

void Cycle::isen_rel_temp_from_pressure() {

    double p1 = pressureCAS[pressureCAS.size()-2];
    double p2 = pressureCAS[pressureCAS.size()-1]; //i.e. pressureCAS.back()
    double T1 = temperatureCAS[temperatureCAS.size()-1]; //i.e. temperatureCAS.back()
    double T2 = T1 * pow(p2/p1, (gamma-1)/gamma);
    temperatureCAS.push_back(T2);

}

void Cycle::printCAS() {
    cout << "\nCold-Air Standard:";
    cout << "\n\t\t\t\t\t\t|Temperature(K)\t\t|Entropy(kJ/(kg*K))\t|Pressure(kPa)\t\t|Specific Volume(m^3/kg)\t\t|Enthalpy(kJ/kg)\n";
    cout << "State 0: "; print_rowCAS(0);
    int i = 1;
    for (i; i < num_of_comp_stages + 1; i++) {
        print_line();
        cout << "State " << i << "i:"; print_rowCAS(2*i-1);
        cout << "State " << i << ":"; print_rowCAS(2*i);
    }
    print_line();
    cout << "State " << i << ":"; print_rowCAS(2 * i - 1);
    int j = i;
    for (j; j < num_of_turbine_stages + i; j++) {
        print_line();
        cout << "State " << j + 1 << "i:"; print_rowCAS(2*j);
        cout << "State " << j + 1 << ":"; print_rowCAS(2*j+1);
    }
    print_line();
    cout << "State " << j + 1 << "i:"; print_rowCAS(2*j);
    cout << "State " << j + 1 << ":"; print_rowCAS(2*j+1);
    print_line();
    cout << "State " << j + 1 << " (with afterburner):"; print_rowCAS(2*j+2);
    print_line();
    cout << "State " << j + 2 << "i (with afterburner):"; print_rowCAS(2*j+3);
    cout << "State " << j + 2 << " (with afterburner):"; print_rowCAS(2*j+4);
    cout << "\nThrust (without afterburner): " << thrust_wo_abCAS << " kN" << endl;
    cout << "Thrust (with afterburner): " << thrust_w_abCAS << " kN" << endl;
}

void Cycle::print_rowCAS(int i){

    if (i < temperatureCAS.size() - 3) cout << "\t\t\t\t\t|";
    else cout << "\t\t\t|";

    cout << fixed << setprecision(3) << temperatureCAS[i] << "\t\t|";
    cout << fixed << setprecision(5) << entropyCAS[i] << "\t\t|";
    if (pressureCAS[i] >= 1000) cout << fixed << setprecision(2) << pressureCAS[i] << "\t\t|";
    else cout << fixed << setprecision(2) << pressureCAS[i] << "\t\t\t|";
    cout << fixed << setprecision(5) << specific_volumeCAS[i] << "\t\t\t\t|";
    cout << fixed << setprecision(3) << enthalpyCAS[i] << endl;

}

void Cycle::output2fileCAS() {
    ofstream outputFile;
    outputFile.open("OutputCAS.csv", ios::out);
    if (!outputFile.is_open())
        cerr << "Failed to create or open OutputCAS.csv file!\n If the OutputCAS.csv has already been created, make sure it is closed before running the program." << endl;
    outputFile << ",Temperature (K), Entropy (kJ/(kg*K)), Pressure (kPa), Specific Volume (m^3/kg), Enthalpy (kJ/kg)\n";
    outputFile << "State 0,"; if (!output2file_rowCAS(outputFile, 0)) return;
    int i = 1;
    for (i; i < num_of_comp_stages + 1; i++) {
        outputFile << "State " << i << "i:,"; if (!output2file_rowCAS(outputFile, 2 * i - 1)) return;
        outputFile << "State " << i << ":,"; if (!output2file_rowCAS(outputFile, 2 * i)) return;
    }
    outputFile << "State " << i << ":,"; if (!output2file_rowCAS(outputFile, 2 * i - 1)) return;
    int j = i;
    for (j; j < num_of_turbine_stages + i; j++) {
        outputFile << "State " << j + 1 << "i:,"; if (!output2file_rowCAS(outputFile, 2 * j)) return;
        outputFile << "State " << j + 1 << ":,"; if (!output2file_rowCAS(outputFile, 2 * j + 1)) return;
    }
    outputFile << "State " << j + 1 << "i:,"; if (!output2file_rowCAS(outputFile, 2 * j)) return;
    outputFile << "State " << j + 1 << ":,"; if (!output2file_rowCAS(outputFile, 2 * j + 1)) return;
    outputFile << "State " << j + 1 << " (with afterburner):,"; if (!output2file_rowCAS(outputFile, 2 * j + 2)) return;
    outputFile << "State " << j + 2 << "i (with afterburner):,"; if (!output2file_rowCAS(outputFile, 2 * j + 3)) return;
    outputFile << "State " << j + 2 << " (with afterburner):,"; if (!output2file_rowCAS(outputFile, 2 * j + 4)) return;
    outputFile << "\nThrust (without afterburner): " << thrust_wo_abCAS << " kN" << endl;
    outputFile << "Thrust (with afterburner): " << thrust_w_abCAS << " kN" << endl;
}

bool Cycle::output2file_rowCAS(ofstream& ofile, int i) {

    bool cont = 1;

    if (temperatureCAS[i] < 0) {
        ofile << "-,";
        cont = 0;
    }
    else ofile << temperatureCAS[i] << ",";

    if (entropyCAS[i] < 0) {
        ofile << "-,";
        cont = 0;
    }
    else ofile << entropyCAS[i] << ",";

    if (pressureCAS[i] < 0) {
        ofile << "-,";
        cont = 0;
    }
    else ofile << pressureCAS[i] << ",";

    if (specific_volumeCAS[i] < 0) {
        ofile << "-,";
        cont = 0;
    }
    else ofile << specific_volumeCAS[i] << ",";

    if (enthalpyCAS[i] < 0) {
        ofile << "-,";
        cont = 0;
    }
    else ofile << enthalpyCAS[i] << endl;

    return cont;
}

void Cycle::plotCAS() {
    return;
}

tuple<double, double> InitialPT::interpolate(double altitude) {
    double pressure, temperature, x, x1, x2, p1, p2, t1, t2;
    int u = 20;
    int l = 0;
    if (altitudeTable[u][0] == altitude){
        pressure = altitudeTable[u][1];
        temperature = altitudeTable[u][2];
    }
    else if (altitudeTable[l][0] == altitude){
        pressure = altitudeTable[l][1];
        temperature = altitudeTable[l][2];
    }
    else {
        while(1) {
            int m = (u+l)/2;
            if (altitudeTable[m][0] == altitude) {
                pressure = altitudeTable[m][1];
                temperature = altitudeTable[m][2];
                break;
            }
            else if (altitudeTable[m][0] < altitude) l = m;
            else u = m; 
            if (u-l == 1) {
                x = altitude;
                x1 = altitudeTable[l][0];
                x2 = altitudeTable[u][0];
                p1 = altitudeTable[l][1];
                p2 = altitudeTable[u][1];
                t1 = altitudeTable[l][2];
                t2 = altitudeTable[u][2];
                pressure = (p2-p1)*(x-x1)/(x2-x1) + p1;
                temperature = (t2-t1)*(x-x1)/(x2-x1) + t1;
                break;
            }
        }
    }
    temperature = temperature + 273.15; //Conversion to Kelvin
    tuple<double, double> pt(pressure, temperature);
    return pt;
}

double AirStandard::interpolate(double x, int inputColumn, int outputColumn) {
    double y, x1, x2, y1, y2;
    x = preconversion(x, inputColumn);
    int u = 111;
    int l = 0;
    if (x < ASTable[l][inputColumn] ||
        x > ASTable[u][inputColumn]) {
            y = -1;
        }
    else if (ASTable[u][inputColumn] == x) {
        y = ASTable[u][outputColumn];
    }
    else if (ASTable[l][inputColumn] == x) {
        y = ASTable[l][outputColumn];
    }
    else {
        while (1) {
            int m = (u + l) / 2;
            if (ASTable[m][inputColumn] == x) {
                y = ASTable[m][outputColumn];
                break;
            }
            else if (ASTable[m][inputColumn] < x)
                l = m;
            else
                u = m;
            if (u - l == 1) {
                x1 = ASTable[l][inputColumn];
                x2 = ASTable[u][inputColumn];
                y1 = ASTable[l][outputColumn];
                y2 = ASTable[u][outputColumn];
                y = (y2 - y1) * (x - x1) / (x2 - x1) + y1;
                break;
            }
        }
    }
    y = postconversion(y, outputColumn);
    return y;
}

double AirStandard::preconversion(double x, int inputColumn) {
    if (inputColumn == 0) {
        x = 1.8 * x;
    }
    if (inputColumn == 1) {
        x = 0.42992 * x;
    }
    if (inputColumn == 3) {
        x = 0.23885 * x;
    }
    return x;
}

double AirStandard::postconversion(double y, int outputColumn) {
    if (outputColumn == 0) {
        y = (5.0/9.0) * y;
    }
    if (outputColumn == 1) {
        y = 2.32601 * y;
    }
    if (outputColumn == 3) {
        y = 4.18681 * y;
    }
    return y;
}