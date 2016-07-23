//  main.cpp
//  CSA_Simulator
//
//  Created by Erik Sandgren on 21/12/15.
//  Copyright © 2015 Erik Sandgren. All rights reserved.

#include <stdlib.h>
#include <iostream>
#include <ctime>
#include <fstream>
#include <string>   
#include <vector>
#include <numeric>
#include <deque>
#include <thread>
#include <iomanip>
#include "Encoder.hpp"
#include "Node.hpp"
#include "Arrivals.hpp"
#include "Decoder.hpp"
using namespace std; // Using standard library namespace.
#define NUM_THREADS 4
// Declaration of function to evaluate PLR for a certain load, defined further down...
void evaluate_load(double g, int n, int n_rx,int d[],  double Lambda[],  int q,  int iter_max,  int how_often_to_decode,  bool boundary_effect,  bool first_slot_tx,  int num_packets_to_sim, int num_PL_to_sim, vector<double> &PLR,int index, int maximum_delay, vector<vector<int> > &delays, bool save_delays, bool progression, vector<int> &load_tracker,vector<int> &resolve_tracker,vector<int> &singleton_tracker);



int main () {
    // Define all parameters:
    int n,n_rx,maximum_delay,num_packets_to_sim,num_PL_to_sim,q, iter_max,how_often_to_decode, g_len, g_index;
    n = 120; // Frame length 'n'
    n_rx= 6*n; // Memory length 'n_rx'
    maximum_delay=n_rx+2*n;  // For a non max-delay constarined simulation, put maximum_delay to larger than n_rx+n

    double g[] ={0.65};

    g_len = sizeof(g) / sizeof(double);
    
    // vector to save packet loss rate results
    vector<double> PLR(g_len); // All elements are initially 0 this way
    
    num_packets_to_sim =(int)1e6;  // How many sent packets to simulate (at most) for each system-load value
    num_PL_to_sim=num_packets_to_sim/100; // How many packet losses to simulate (at most) for each system-load value
    
    // Defines the VN-degree distribution
    int d[] = {3};
    double Lambda[] ={1};
    q = sizeof(Lambda) / sizeof(double);
    
    // Defines the number of decoding iterations and how often to decode
    iter_max =(int)1e5;
    how_often_to_decode=1;
    
    bool boundary_effect=false; // Do you want to consider a system with boundary effect?
    bool save_delays=false;  // Do you want to save the packet-delay pdf?
    bool first_slot_tx=true; // Do you want to transmit in first slot or use uniform edge distribution?
    bool save_progression=false; // Do we want to keep track of the "instantaneous" load vs num. packets decoded...
    vector<int> singleton_tracker;
    vector<int> load_tracker; // Tracks number of active users over time.
    vector<int> resolve_tracker; // Tracks the number of active users that are resolved over time?
    vector<vector<int> > delays (0,vector<int>(0,0));
    if(save_delays)
    {
        delays.resize(g_len,vector<int>(maximum_delay,0));
    }
    // where to save the output and by which name:
    ofstream output_file;
    string filename = "FA-CSA-3-" + to_string(n) + "n_rx_" + to_string(n_rx)+"max_delay_" + to_string(maximum_delay) +"_BE_"+ to_string(boundary_effect)+"_firstslottx_" +to_string(first_slot_tx)+ "_singleton_tracker_g_083.txt";
    if (save_delays) {
        
    }
    cout << "Frame Asynchronous Coded Slotted ALOHA simulation \n";
    cout << "n = "<< n<<"\n";
    cout<< "d= "<< d<<"\n";
    cout<< "Lambda = "<< Lambda <<"\n";
    cout<< "Packets to simulate = "<< num_packets_to_sim <<"\n";
    cout<< "Please find the results in " << filename << "\n\n";
    cout<< "**************** Progress *************** " << "\n\n";

    
    //thread t[NUM_THREADS];
    
    cout<< "Load \t \t    Sent \t\tLost \t \t   PLR \n";
    cout<< "------------------------------------------------------- \n";
    // Start timer
    clock_t begin = clock();
   
    
    
    g_index=g_len-1;
    while(g_index>=0)
    {
        
        evaluate_load(g[g_index], n, n_rx, d, Lambda, q, iter_max, how_often_to_decode, boundary_effect, first_slot_tx, num_packets_to_sim, num_PL_to_sim, PLR, g_index, maximum_delay, delays, save_delays, save_progression,load_tracker,resolve_tracker,singleton_tracker);
        
        /*
        if(g_index+1<num_threads){
            num_threads=g_index+1;
        }
        
        for(int i=0; i<num_threads;i++){
            t[i]=thread( evaluate_load,g[g_index], n, n_rx, d, Lambda, q, iter_max, how_often_to_decode, boundary_effect, first_slot_tx, num_packets_to_sim, num_PL_to_sim, &PLR, g_index, maximum_delay, delays);
            g_index--;
        }
        
        for (int i=0; i<num_threads; i++) {
            t[i].join();
        }
         */
        g_index--;
    }
    
    
 ///////// WRITES TO OUTPUT ////////////////////////
    output_file.open(filename);
    output_file << "Lambda:  ";
    for (int i=0; i < q ; i++) {
        output_file << Lambda[i]<<" ";
    }
    output_file << endl;
    
    output_file<<"degrees:  ";
    for (int i=0; i < q ; i++) {
        output_file << d[i]<<" ";
    }
    output_file << endl;
    
    output_file << "n:  " << n << endl;
    
    output_file<< "n_rx:  " << n_rx << endl;
    
    output_file<< "maximum delay:  "<< maximum_delay<< endl;
    
    output_file<< "Transmit in first slot:  ";
    if (first_slot_tx) {
        output_file<< "Yes"<< endl;
    }
    else{
        output_file<<"No"<< endl;
    }
    
    output_file<< "Boundary Effect: ";
    if (boundary_effect) {
        output_file<< "Yes"<< endl;
    }
    else{
        output_file<<"No"<< endl;
    }
    
    output_file<< "Max number of packets to simulate:  "<< num_packets_to_sim<< endl;
    output_file<< "Max number of losses to simulate:  "<<num_PL_to_sim<< endl;
    
    output_file<< endl;output_file<< endl; output_file<< endl; output_file<< endl;
    output_file<<"Load \t PLR"<< endl;
    for (int i=g_len-1; i>=0; i--)
    {
        output_file << g[i]<<"\t"<<PLR[i]<<"\\\\"<<endl;
    }
    output_file << endl;
    output_file << endl;
    output_file <<"Delay PDF (one row per load and it is not normalized)" <<endl;
    
    if (save_delays)
    {
        for (int i=0; i<g_len; i++)
        {
            for (int ii=0; ii < maximum_delay; ii++)
            {
                output_file<< delays[i][ii]<<" ";
            }
            output_file <<"\\\\"<<endl;
        }
    }
    if(save_progression){
        output_file << endl;
        output_file << endl;
        output_file <<"Load tracker (number of active users over time)  size:"<< load_tracker.size() <<endl;
        for (int i=0; i < load_tracker.size();i++)
        {
                output_file<< load_tracker[i]<<" ";
        }
        output_file <<"\\\\"<<endl;
        output_file << endl;
        output_file << endl;
        output_file <<"Resolve tracker (number of resolved-active users over time)  size:" << resolve_tracker.size() <<endl;
        for (int i=0; i < resolve_tracker.size(); i++){
            output_file<< resolve_tracker[i]<<" ";
        }
        output_file <<"\\\\"<<endl;
        
        output_file <<"\\\\"<<endl;
        output_file << endl;
        output_file << endl;
        output_file <<"Singleton tracker (number of singleton slots over time)  size:" << singleton_tracker.size() <<endl;
        for (int i=0; i < singleton_tracker.size(); i++){
            output_file<< singleton_tracker[i]<<" ";
        }
        output_file <<"\\\\"<<endl;
    }
    
    output_file.close();
    clock_t end =clock();
    double elapsed_secs= double(end-begin)/CLOCKS_PER_SEC;
    
    cout<< "Elapsed time is: "<< elapsed_secs<<" seconds "<<endl;
    
    return 0;
}

// This function is used to evaluate the performance for the given parameters (loop is usually over g, the system load).
    void evaluate_load(double g, int n, int n_rx,int d[],  double Lambda[],  int q,  int iter_max,  int how_often_to_decode,  bool boundary_effect,  bool first_slot_tx,  int num_packets_to_sim, int num_PL_to_sim, vector<double> &PLR,int index, int maximum_delay, vector<vector<int> > &delays, bool save_delays, bool save_progression,vector<int> &load_tracker,vector<int> &resolve_tracker, vector<int> &singleton_tracker)
    {
        int samp;
        unsigned long int time_step=0;
        Arrivals poiss= Arrivals(g);
        deque<int> active(n_rx+n,0);
        int num_active=0;
        int num_resolved=0;
        int num_singletons;
        Encoder enc=Encoder(n, n_rx, d, Lambda,q);
        Decoder dec=Decoder(iter_max, how_often_to_decode, n, n_rx,maximum_delay);
        vector<Node*> VN;
        vector<Node*>CN(n+n_rx);
        int packets_sent=0;
        int packets_lost=0;
        Node* temp_CN;
        Node* temp_VN;
    
        // Initialize CNs
        for (int i=0; i<n_rx; i++) {
            temp_CN=new Node();
            CN[i]=temp_CN;
            }
        for (int i=n_rx; i<n_rx+n; i++) {
            time_step++;
            temp_CN=new Node(time_step);
            CN[i]=temp_CN;
            }
    
        time_step=0;
    
        // Running some steps without decoding in case the RX is not present at the start
        if(!boundary_effect){
            for (int i=0; i<2*n_rx+n; i++) {
                time_step++;
                // Create new CN and remove the oldest one.
                temp_CN=new Node(time_step);
                CN.push_back(temp_CN);
                temp_CN=CN[0];
            
                CN.erase(CN.begin());
                temp_CN->letGoOffNeighbours(temp_CN);
                delete temp_CN;
                //Create new VN and distribute its packets...
                samp=poiss.sample();
                active.push_front(samp);
                num_active-=active[n_rx+n];
                num_active+=samp;
                active.pop_back();
               // load_tracker.push_back(num_active);
                for (int i=0; i<samp; i++) {
                    temp_VN=new Node(time_step);
                    enc.distribute_repetitions(temp_VN, &CN,first_slot_tx);
                    VN.push_back(temp_VN);
                }
                num_resolved=0;
                int start_idx=-1;
                for(int i=int(VN.size()-1);  i>=0 && (time_step - VN[i]->getTimeOfArrival())<n ;i--){
                    start_idx=i;
                }
                for (int i=start_idx; i>=0 && (time_step - VN[i]->getTimeOfArrival())<2*n; i--)
                {
                    num_resolved+= (int) VN[i]->getDecoded();
                }
               // resolve_tracker.push_back(num_resolved);
            }
        }
    
    while(packets_sent<num_packets_to_sim && packets_lost<num_PL_to_sim)
    {
        time_step++;
        // Create new CN and remove the oldest one.
        temp_CN=new Node(time_step);
        CN.push_back(temp_CN);
        temp_CN=CN[0];
        CN.erase(CN.begin());
        temp_CN->letGoOffNeighbours(temp_CN);
        delete temp_CN;
        
        //Create new VN and distribute its packets...
        samp=poiss.sample();
        for (int i=0; i<samp; i++)
        {
            temp_VN=new Node(time_step);
            enc.distribute_repetitions(temp_VN, &CN,first_slot_tx);
            VN.push_back(temp_VN);
        }
        

        active.push_front(samp);
        num_active-=active.back();
        num_active+=samp;
        active.pop_back();
        if(!(time_step % 20)){
        load_tracker.push_back(num_active);
        
        // Decode
        dec.decode(&CN, &VN, time_step, num_singletons);
        // Counts the number of singletons slots found in this slot-decoding.
        singleton_tracker.push_back(num_singletons);
        num_resolved=0;
        int start_idx=int(VN.size()-1);
        for (int i=start_idx; i>=0; i--)
        {
            num_resolved+= (int) VN[i]->getDecoded();
        }
        resolve_tracker.push_back(num_resolved);
        }
        // Counting packets: different method depending on the time_initial variable
        dec.count_packets(&VN, time_step, boundary_effect);
        packets_sent=dec.getSentPackets();
        packets_lost=dec.getLostPackets();
    }
    VN.clear();
    CN.clear();
    if (save_delays)
    {
        delays.at(index) = dec.getDelays();
    }
    PLR.at(index)=(double)packets_lost / (double)packets_sent;
    streamsize normal_prec=cout.precision();
    cout<<setprecision(3)<<fixed;
    cout<< g << " \t \t "<< setw((int)log10(num_packets_to_sim)+1)<< packets_sent << " \t \t" <<setw((int)log10(num_PL_to_sim)+1) << packets_lost << " \t \t "<< scientific<<PLR.at(index)<<endl;
    cout<<setprecision(normal_prec);
}

