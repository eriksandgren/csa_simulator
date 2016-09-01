#include<string>
using namespace std;

enum CsaType{FA_FNB, FA_FB, FA_UNB, FA_UB, FS, SC};

CsaType typeOfSimulation = FA_FB;
int n = 200; // Frame length 'n'
int n_rx= 5*n; // Memory length 'n_rx'
int maximumDelay=n_rx + n;  // For a non max-delay constrained simulation, put maximum_delay to larger than n_rx+n

double g[] = {0.10, 0.20, 0.30,0.50,.40,0.60,0.70,0.80,0.90,1.0};
int gLength = sizeof(g) / sizeof(double);
int gIndex;

int numberOfPacketsToSimulateMax =(int)1e5;  // How many sent packets to simulate (at most) for each system-load value
int numberOfPacketLossesToSimulateMax=numberOfPacketsToSimulateMax/100; // How many packet losses to simulate (at most) for each system-load value


// Defines the VN-degree distribution
// Example : int d[] = {3, 8}; double Lambda = {0.86, 0.14};
int d[] = {3, 8};
double Lambda[] ={0.86, 0.14};
int q = sizeof(Lambda) / sizeof(double);

// Defines the number of decoding iterations and how often to decode
int numDecodingIterations = (int)1e7;
int slotsBetweenDecoding = 1;
bool saveDelays=false;  // Do you want to save the packet-delay pdf?
bool saveAvgNumberOfIterations = true;
string fileName = "results.txt";

// Vector in which simulated PLR is saved
vector<double> plr (gLength); // All elements are initially 0 this way
// Matrix in which simulated delay pmf is saved
vector<vector<int> > delays (0,vector<int>(0,0));
vector<vector<int> > iterations (0,vector<int>(0,0));
