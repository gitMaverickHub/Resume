/*
--------------------------------------------------------------------------------------------------------------------------------------
Variables


Velocity                                    - velocity
Exit Velocity                               - velocity_exit
Specific Gas Constant of Air                - R
Altitude                                    - altitude
    This will effect free stream (inlet) pressure and temperature
Compressor:
    Inlet Area                              - A_in
    Number of stages                        - num_of_comp_stages
    Stage efficiencies                      - comp_stage_efficiencies
    Overall Pressure Ratio                  - pressure_ratio
Combustor:
    Fuel heating value                      - heating_value
    Mass flow rate                          - comb_mass_flow_rate
Turbine:
    Number of stages                        - num_of_turbine_stages
    Stage efficiencies                      - turb_stage_efficiencies
Nozzle:
    Exit area                               - A_out
    Efficiency                              - nozz_efficiency
Afterburner:
    Mass flow rate                          - afterburner_mass_flow_rate
To find:
    Pressure (at all states)                - pressure
    Specific Volume (at all states)         - specific_volume
    Temperature (at all states)             - temperature
    Entropy (at all states)                 - entropy
    Enthalpy (at all states)                - enthalpy
    Thrust Without Afterburner              - thrust_wo_ab
    Thrust With Afterburner                 - thrust_w_ab

-------------------------------------------------------------------------------------------------------------------------------------
*/

#ifndef TABLES_H
#define TABLES_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <tuple>
using namespace std;

class Cycle
{
    public:
        Cycle(vector<string> data);

    private:

        // Air Standard Functions
        void cycleAS();
        void calculateAS();
        void precompressorAS();
        void compressorAS();
        void combustorAS();
        void turbineAS();
        void nozzleAS();
        void afterburnerAS();
        double calc_thrustAS();
        void sMethod();
        void hMethod();
        void printAS();
        bool print_rowAS(int i);
        void print_line(); // This function is used for both air and cold air standard
        void output2fileAS();
        bool output2file_rowAS(ofstream& ofile, int i);
        void plotAS();

        // Cold Air Standard Functions
        void cycleCAS();
        void calculateCAS();
        void precompressorCAS();
        void compressorCAS();
        void combustorCAS();
        void turbineCAS();
        void nozzleCAS();
        void afterburnerCAS();
        double calc_thrustCAS();
        void entropy_equation();
        void isen_rel_temp_from_pressure();
        void printCAS();
        void print_rowCAS(int i);
        void output2fileCAS();
        bool output2file_rowCAS(ofstream& ofile, int i);
        void plotCAS();
        
        double velocity, altitude, A_in, air_mass_flow_rateAS, air_mass_flow_rateCAS, comp_stage_efficiencies, pressure_ratio, heating_value, turb_stage_efficiencies,
                A_out, nozz_efficiency, exit_mass_flow_rateAS, exit_mass_flow_rateCAS, thrust_wo_abAS, thrust_wo_abCAS, thrust_w_abAS, thrust_w_abCAS, velocity_exitAS,
                velocity_exitCAS;
        int num_of_comp_stages, num_of_turbine_stages;
        vector<double> pressureAS, specific_volumeAS, temperatureAS, enthalpyAS, entropyAS, pressureCAS, specific_volumeCAS, temperatureCAS, enthalpyCAS, entropyCAS;
        double R = 0.28716; //kJ/(kg*K) 
        double cp = 1.005; // kJ/(kg*K) 
        double gamma = 1.4; // specific heat ratio  

};

class InitialPT
{
    public:
        tuple<double, double> interpolate(double altitude);

    private:
        const double altitudeTable[21][3] = 

        //  altitude (m),    p (kPa),     T (C)

        {
         {0,        111,    15},

         {500,     95,     12},
         {1000,    90,     8},
         {1500,    85,     6},
         {2000,    80,     2},

         {2500,    75,     -1},
         {3000,    70,     -4},
         {3500,    65,     -8},
         {4000,    61,     -11},

         {4500,    58,     -14},
         {5000,    54,     -18},
         {5500,    50,     -20},
         {6000,    47,     -24},

         {6500,    44,     -27},
         {7000,    41,     -30},
         {7500,    38,     -34},
         {8000,    35,     -37},

         {8500,    33,     -40},
         {9000,    31,     -43},
         {9500,    29,     -46},
         {10000,   27,     -50}
        };
};

class AirStandard
{
    public:
        double interpolate(double x, int inputColumn, int outputColumn);

    private:
        double preconversion(double x, int outputColumn);
        double postconversion(double y, int inputColumn);
        const double ASTable[112][4] =

        //  T (R),   h (Btu/lb),  u (Btu/lb),  s (Btu/(lb*R))
        //  Note: The 3rd column will never be used.

        {
         {360,      85.79,      61.29,      0.50369},
         {380,      90.79,      64.70,      0.51663},
         {400,      95.53,      68.11,      0.52890},
         {420,      100.32,     71.52,      0.54058},
         {440,      105.11,     74.93,      0.55172},
        
         {460,      109.90,     78.36,      0.56235},
         {480,      114.69,     81.77,      0.57255},
         {500,      119.48,     85.20,      0.58233},
         {520,      124.27,     88.62,      0.59172},
         {537,      128.34,     91.53,      0.59945},   
         
         
         {540,      129.06,     92.04,      0.60078},
         {560,      133.86,     95.47,      0.60950},
         {580,      138.66,     98.90,      0.61793},
         {600,      143.47,     102.34,     0.62607},
         {620,      148.28,     105.78,     0.63395},
        
         {640,      153.09,     109.21,     0.64159},
         {660,      157.92,     112.67,     0.64902},
         {680,      162.73,     116.12,     0.65621},
         {700,      167.56,     119.58,     0.66321},
         {720,      172.39,     123.04,     0.67002},
        
         
         {740,      177.23,     126.51,     0.67665},
         {760,      182.08,     129.99,     0.68312},
         {780,      186.94,     133.47,     0.68941},
         {800,      191.81,     136.97,     0.69558},
         {820,      196.69,     140.47,     0.70160},
         
         {840,      201.56,     143.98,     0.70747},
         {860,      206.46,     147.50,     0.71323},
         {880,      211.35,     151.02,     0.71886},
         {900,      216.26,     154.57,     0.72438},
         {920,      221.18,     158.12,     0.72979},


         {940,      226.11,     161.68,     0.73509},
         {960,      231.06,     165.26,     0.74030},
         {980,      236.02,     168.83,     0.74540},
         {1000,     240.98,     172.43,     0.75042},
         {1040,     250.95,     179.66,     0.76019},

         {1080,     260.97,     186.93,     0.76964},
         {1120,     271.03,     194.25,     0.77880},
         {1160,     281.14,     201.63,     0.78767},
         {1200,     291.30,     209.05,     0.79628},
         {1240,     301.52,     216.53,     0.80466},


         {1280,     311.79,     224.05,     0.81280},
         {1320,     322.79,     231.63,     0.82075},
         {1360,     332.48,     239.25,     0.82848},
         {1400,     342.90,     246.93,     0.83604},
         {1440,     353.37,     254.66,     0.84341},

         {1480,     363.89,     262.44,     0.85062},
         {1520,     374.47,     270.26,     0.85767},
         {1560,     385.08,     278.13,     0.86456},
         {1600,     395.74,     286.06,     0.87130},
         {1650,     409.13,     296.03,     0.87954},


         {1700,     422.59,     306.06,     0.88758},
         {1750,     436.12,     316.16,     0.89542},
         {1800,     449.71,     326.32,     0.90308},
         {1850,     463.37,     336.55,     0.91056},
         {1900,     477.09,     346.85,     0.91788},

         {1950,     490.88,     357.20,     0.92504},
         {2000,     504.71,     367.61,     0.93205},
         {2050,     518.61,     378.08,     0.93891},
         {2100,     532.55,     388.60,     0.94564},
         {2150,     546.54,     399.17,     0.95222},


         {2200,     560.59,     409.78,     0.95868},
         {2250,     574.69,     420.46,     0.96501},
         {2300,     588.82,     431.16,     0.97123},
         {2350,     603.00,     441.91,     0.97732},
         {2400,     617.22,     452.70,     0.98331},

         {2450,     631.48,     463.54,     0.98919},
         {2500,     645.78,     474.40,     0.99497},
         {2550,     660.12,     485.31,     1.00064},
         {2600,     674.49,     496.26,     1.00623},
         {2650,     688.90,     507.25,     1.01172},


         {2700,     703.35,     518.26,     1.01712},
         {2750,     717.83,     529.31,     1.02244},
         {2800,     732.33,     540.40,     1.02767},
         {2850,     746.88,     551.52,     1.03282},
         {2900,     761.45,     562.66,     1.03788},

         {2950,     776.05,     573.84,     1.04288},
         {3000,     790.68,     585.04,     1.04779},
         {3050,     805.34,     596.28,     1.05264},
         {3100,     820.03,     607.53,     1.05741},
         {3150,     834.75,     618.82,     1.06212},


         {3200,     849.48,     630.12,     1.06676},
         {3250,     864.24,     641.46,     1.07134},
         {3300,     879.02,     652.81,     1.07585},
         {3350,     893.83,     664.20,     1.08470},
         {3400,     908.66,     675.60,     1.08470},

         {3450,     923.52,     687.04,     1.08904},
         {3500,     938.40,     698.48,     1.08904},
         {3550,     953.30,     709.95,     1.09755},
         {3600,     968.21,     721.44,     1.10172},
         {3650,     983.15,     732.95,     1.10584},


         {3700,     998.11,     744.48,     1.10991},
         {3750,     1013.1,     756.04,     1.11393},
         {3800,     1028.1,     767.60,     1.11791},
         {3850,     1043.1,     779.19,     1.12183},
         {3900,     1058.1,     790.80,     1.12571},

         {3950,     1073.2,     802.43,     1.12955},
         {4000,     1088.3,     814.06,     1.13334},
         {4050,     1103.4,     825.72,     1.13709},
         {4100,     1118.5,     837.40,     1.14079},
         {4150,     1133.6,     849.09,     1.14446},

         
         {4200,     1148.7,     860.81,     1.14809},
         {4300,     1179.0,     884.28,     1.15522},
         {4400,     1209.4,     907.81,     1.16221},
         {4500,     1239.9,     931.39,     1.16905},
         {4600,     1270.4,     955.04,     1.17575},

         {4700,     1300.9,     978.73,     1.18232},
         {4800,     1331.5,     1002.5,     1.18876},
         {4900,     1362.2,     1026.3,     1.19508},
         {5000,     1392.9,     1050.1,     1.20129},
         {5100,     1423.6,     1074.0,     1.20738},


         {5200,     1454.4,     1098.0,     1.21336},
         {5300,     1485.3,     1122.0,     1.21923}
        };
};

#endif