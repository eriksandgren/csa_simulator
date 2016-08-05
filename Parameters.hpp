#include<string>
using namespace std;
int n = 120; // Frame length 'n'
int n_rx= 5*n; // Memory length 'n_rx'
int maximum_delay=n_rx+n;  // For a non max-delay constrained simulation, put maximum_delay to larger than n_rx+n
int g_index;
double g[] = {.45};
int g_len = sizeof(g) / sizeof(double);

int num_packets_to_sim =(int)1e5;  // How many sent packets to simulate (at most) for each system-load value
int num_PL_to_sim=num_packets_to_sim/100; // How many packet losses to simulate (at most) for each system-load value


// Defines the VN-degree distribution
int d[] = {3};
double Lambda[] ={1};
int q = sizeof(Lambda) / sizeof(double);

// Defines the number of decoding iterations and how often to decode
int iter_max =(int)1e7;
int how_often_to_decode=1;



bool boundary_effect=true; // Do you want to consider a system with boundary effect?
bool save_delays=false;  // Do you want to save the packet-delay pdf?
bool first_slot_tx=true; // Do you want to transmit in first slot or use uniform edge distribution?

string filename = "test" + to_string(n) + "n_rx_" + to_string(n_rx)+"max_delay_" + to_string(maximum_delay) +"_BE_"+ to_string(boundary_effect)+"_firstslottx_" +to_string(first_slot_tx)+ "_.txt";

// Vector in which simulated PLR is saved
vector<double> PLR(g_len); // All elements are initially 0 this way
// Matrix in which simulated delay pmf is saved
vector<vector<int> > delays (0,vector<int>(0,0));
