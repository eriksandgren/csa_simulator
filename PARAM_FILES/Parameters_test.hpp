#include<string>
using namespace std;

enum CsaType{FA_FNB, FA_FB, FA_UNB, FA_UB, FS, SC};

CsaType typeOfSimulation = FA_FNB;
int n = 100; // Frame length 'n'
int n_rx= 5*n; // Memory length 'n_rx'
int maximumDelay=n_rx + n;  // For a non max-delay constrained simulation, put maximum_delay to larger than n_rx+n

//double g[] = {0.05, 0.10, 0.15, 0.20, 0.25, 0.30, 0.35, 0.40, 0.45, 0.50, 0.55, 0.60, 0.65, 0.70, 0.75, 0.80, 0.85, 0.90, 0.95, 1.0};
double g[] = {0.7};
int gLength = sizeof(g) / sizeof(double);
int gIndex;

int numberOfPacketsToSimulateMax =(int)1e7;  // How many sent packets to simulate (at most) for each system-load value
int numberOfPacketLossesToSimulateMax=numberOfPacketsToSimulateMax; // How many packet losses to simulate (at most) for each system-load value


// Defines the VN-degree distribution
// Example : int d[] = {3, 8}; double Lambda = {0.86, 0.14};
int d[] = {2};
double Lambda[] ={1};
int q = sizeof(Lambda) / sizeof(double);

// Defines the number of decoding iterations and how often to decode
int numDecodingIterations = (int)1e7;
int slotsBetweenDecoding = 1;
bool saveDelays=true;  // Do you want to save the packet-delay pdf?
bool saveAvgNumberOfIterations = true;
string fileName = "test.txt";

// Vector in which simulated PLR is saved
vector<double> plr (gLength); // All elements are initially 0 this way
// Matrix in which simulated delay pmf is saved
vector<vector<int> > delays (0,vector<int>(0,0));
vector<vector<int> > iterations (0,vector<int>(0,0));
