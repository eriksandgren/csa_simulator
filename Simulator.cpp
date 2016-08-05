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
#include <iomanip>
#include "Encoder.hpp"
#include "Node.hpp"
#include "Arrivals.hpp"
#include "Decoder.hpp"
#include "Parameters.hpp"

using namespace std;

// Declaration of function to evaluate PLR for a certain load, defined further down...
void evaluate_load(double g, int n, int n_rx,int d[],  double Lambda[],  int q,  int iter_max,  int how_often_to_decode,  bool boundary_effect,  bool first_slot_tx,  int num_packets_to_sim, int num_PL_to_sim, vector<double> &PLR,int index, int maximum_delay, vector<vector<int> > &delays, bool save_delays);
void write_to_output_file();


int main () {
	clock_t begin =clock();

	if(save_delays)
	{
		delays.resize(g_len,vector<int>(maximum_delay,0));
	}

	cout << "Frame Asynchronous Coded Slotted ALOHA simulation \n";
	cout << "n = "<< n<<"\n";
	cout<< "d= "<< d<<"\n";
	cout<< "Lambda = "<< Lambda <<"\n";
	cout<< "Packets to simulate = "<< num_packets_to_sim <<"\n";
	cout<< "Please find the results in " << filename << "\n\n";
	cout<< "**************** Progress *************** " << "\n\n";


	cout<< "Load \t \t    Sent \t\tLost \t \t   PLR \n";
	cout<< "------------------------------------------------------- \n";

	g_index=g_len-1;
	while(g_index>=0)
	{
		evaluate_load(g[g_index], n, n_rx, d, Lambda, q, iter_max, how_often_to_decode, boundary_effect, first_slot_tx, num_packets_to_sim, num_PL_to_sim, PLR, g_index, maximum_delay, delays, save_delays);
		g_index--;
	}

	write_to_output_file();

	clock_t end =clock();
	double elapsed_secs= double(end-begin)/CLOCKS_PER_SEC;

	cout<< "Elapsed time is: "<< elapsed_secs<<" seconds "<<endl;

	return 0;
}

// This function is used to evaluate the performance for the given parameters (loop is usually over g, the system load).
void evaluate_load(double g, int n, int n_rx,int d[],  double Lambda[],  int q,  int iter_max,  int how_often_to_decode,  bool boundary_effect,  bool first_slot_tx,  int num_packets_to_sim, int num_PL_to_sim, vector<double> &PLR,int index, int maximum_delay, vector<vector<int> > &delays, bool save_delays)
{
	int samp;
	unsigned long int time_step=0;
	Arrivals poiss= Arrivals(g);
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
			for (int i=0; i<samp; i++) {
				temp_VN=new Node(time_step);
				enc.distribute_repetitions(temp_VN, &CN,first_slot_tx);
				VN.push_back(temp_VN);
			}

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
		dec.decode(&CN, &VN, time_step);

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

void write_to_output_file(){
	///////// WRITES TO OUTPUT ////////////////////////
	// where to save the output and by which name:
	ofstream output_file;

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
	output_file.close();
}
