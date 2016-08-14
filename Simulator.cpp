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
#include <assert.h>

using namespace std;

// Declaration of function to evaluate PLR for a certain load, defined further down...
void evaluate_load_FA(int index);
void evaluate_load_FS(int index);
void evaluate_load_SC(int index);

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
		switch(typeOfSimulation)
		{
		case FA_FNB:
		case FA_FB:
		case FA_UB:
		case FA_UNB:
			evaluate_load_FA(g_index);
			break;
		case FS:
			evaluate_load_FS(g_index);
			break;
		case SC:
			evaluate_load_SC(g_index);
			break;
		}
		g_index--;
	}

	write_to_output_file();
	clock_t end =clock();
	double elapsed_secs= double(end-begin)/CLOCKS_PER_SEC;
	cout<< "Elapsed time is: "<< elapsed_secs<<" seconds "<<endl;
	return 0;
}

// This function is used to evaluate the performance for the given parameters (loop is usually over g, the system load).
void evaluate_load_FA(int index)
{
	int samp;
	unsigned long int time_step=0;
	Arrivals poiss= Arrivals(g[index]);
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
	if(typeOfSimulation == FA_FNB || typeOfSimulation == FA_UNB){
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
			for (int i=0; i<samp; i++)
			{
				temp_VN=new Node(time_step);
				if(typeOfSimulation == FA_FNB || typeOfSimulation == FA_FB)
				{
					enc.distribute_repetitions_first_slot(temp_VN, &CN);
				}
				else

				{
					enc.distribute_repetitions_uniformly(temp_VN, &CN);
				}
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
			if(typeOfSimulation == FA_FNB || typeOfSimulation == FA_FB)
			{
				enc.distribute_repetitions_first_slot(temp_VN, &CN);
			}
			else

			{
				enc.distribute_repetitions_uniformly(temp_VN, &CN);
			}
			VN.push_back(temp_VN);
		}

		dec.decode(&CN, &VN, time_step);

		// Counting packets
		if(typeOfSimulation == FA_FB || typeOfSimulation == FA_UB )
		{
			dec.count_packets_boundary_effect(&VN, time_step);
		}
		else
		{
			dec.count_packets_no_boundary_effect(&VN, time_step);
		}

		packets_sent=dec.getSentPackets();
		packets_lost=dec.getLostPackets();

	}
	VN.clear();
	CN.clear();
	if (save_delays)
	{
		delays.at(index) = dec.getDelays();
	}
	PLR.at(index) = (double)packets_lost / (double)packets_sent;
	streamsize normal_prec=cout.precision();
	cout << setprecision(3) << fixed;
	cout<< g[index] << " \t \t " << setw((int) log10(num_packets_to_sim) + 1) << packets_sent << " \t \t" << setw((int)log10(num_PL_to_sim)+1) << packets_lost << " \t \t "<< scientific << PLR.at(index) << endl;
	cout << setprecision(normal_prec);

}

void evaluate_load_FS(int index){
		n_rx = n;
        int samp;
        unsigned long int time_step=0;
        Arrivals poiss= Arrivals(g[index]);
    	Encoder enc=Encoder(n, n_rx, d, Lambda,q);
    	Decoder dec=Decoder(iter_max, how_often_to_decode, n, n_rx, maximum_delay);
        vector<Node*> VN;
        vector<Node*> VN_for_next_frame;
        vector<Node*>CN(2*n);
        int packets_sent=0;
        int packets_lost=0;
        Node* temp_CN;
        Node* temp_VN;
        // Initialize CNs
        for (int i=0; i<2*n; i++)
        {
            time_step++;
            temp_CN=new Node(time_step);
            CN[i]=temp_CN;
        }
        // time_step=;
        while(packets_sent<num_packets_to_sim && packets_lost<num_PL_to_sim)
        {
            time_step++;
            //Create new VN and wait.
            samp=poiss.sample();
            for (int i=0; i<samp; i++)
            {
                temp_VN=new Node(time_step);
                VN_for_next_frame.push_back(temp_VN);
            }

            if(save_delays){dec.decode(&CN, &VN, time_step);}
            if (time_step%n == 0 )
            {
                if(!save_delays){
                    dec.decode_frame(&CN, &VN, time_step);
                }

                dec.count_packets_FS(&VN, time_step);
                packets_sent=dec.getSentPackets();
                packets_lost=dec.getLostPackets();
                for (int i=0; i<VN_for_next_frame.size(); i++)
                {
                    temp_VN = VN_for_next_frame.at(i);
                    enc.distribute_repetitions_uniformly(temp_VN,&CN);
                    VN.push_back(temp_VN);
                }
                VN_for_next_frame.clear();
            }

            // Create new CN and remove the oldest one.
            temp_CN=new Node(time_step);
            CN.push_back(temp_CN);
            temp_CN=CN[0];
            CN.erase(CN.begin());
            temp_CN->letGoOffNeighbours(temp_CN);
            delete temp_CN;
        }
        VN.clear();
        CN.clear();
        PLR[index]=(double)packets_lost/(double) packets_sent;
        if(save_delays){
        delays.at(index) = dec.getDelays();
        }
        streamsize normal_prec=cout.precision();
        cout<<setprecision(3)<<fixed;
        cout<< g[index] << " \t \t "<< setw((int)log10(num_packets_to_sim)+1)<< packets_sent << " \t \t" <<setw((int)log10(num_PL_to_sim)+1) << packets_lost << " \t \t "<< scientific<<PLR[index]<<endl;
        cout<<setprecision(normal_prec);
};

void evaluate_load_SC(int index)
	{
		assert(1 == sizeof(d)/sizeof(int));
		int rep = d[0];
		int n_sub_frame = n / rep;
		n_rx = 5*rep*n_sub_frame; // makes the memory "sufficiently" large
	    int samp;
	    unsigned long int time_step=0;
	    Arrivals poiss= Arrivals(g[index]);
    	Encoder enc=Encoder(n_sub_frame, n_rx, d, Lambda,q);
	    Decoder dec=Decoder(iter_max, how_often_to_decode, n_sub_frame, n_rx, maximum_delay);

	    vector<Node*> VN;
	    vector<Node*> VNs_for_next_frame;
	    vector<Node*>CN(rep*n_sub_frame+n_rx);
	    int packets_sent=0;
	    int packets_lost=0;
	    Node* temp_CN;
	    Node* temp_VN;

	    // Initialize CNs
	    for (int i=0; i<n_rx; i++)
	    {
	        temp_CN=new Node();
	        CN[i]=temp_CN;
	    }
	    for (int i=n_rx; i<n_rx+rep*n_sub_frame; i++)
	    {
	        time_step++;
	        temp_CN=new Node(time_step);
	        CN[i]=temp_CN;
	    }

	    time_step=0;


	    while(packets_sent<num_packets_to_sim && packets_lost<num_PL_to_sim)    {
	        // Arrivals at this time step, saved in the VNs_for_next_frame.
	        samp=poiss.sample();
	        for (int i=0; i<samp; i++)
	        {
	            temp_VN=new Node(time_step);
	            VNs_for_next_frame.push_back(temp_VN);
	        }
	        time_step++;

	        // If its time for a new frame, we let the activated VNs distribute their 'd' replicas...
	        if((time_step % n_sub_frame)==0)
	        {
	            for (int i = 0; i < VNs_for_next_frame.size();i++)
	            {
	                enc.distribute_repetitions_SC(VNs_for_next_frame[i], &CN);
	            }
	            VN.insert(VN.end(), VNs_for_next_frame.begin(), VNs_for_next_frame.end());
	            VNs_for_next_frame.clear();
	        }


	        // Shifts in a new node at the back of the memory and removes the  node at the front (the oldest one)
	        temp_CN=new Node(time_step+rep*n_sub_frame);
	        CN.push_back(temp_CN);
	        temp_CN=CN[0];
	        temp_CN->letGoOffNeighbours(temp_CN);
	        CN.erase(CN.begin());
	        delete temp_CN;
	        dec.decode(&CN, &VN, time_step);

	        // Counting packets: different method depending on the time_initial variable
	        dec.count_packets_SC(&VN, time_step, rep);
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
	    cout<< g[index] << " \t \t "<< setw((int)log10(num_packets_to_sim)+1)<< packets_sent << " \t \t" <<setw((int)log10(num_PL_to_sim)+1) << packets_lost << " \t \t "<< scientific<<PLR.at(index)<<endl;
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

	output_file << "Type of simulation: ";
	switch(typeOfSimulation){
	case FA_FNB:
		output_file << "FA_FNB" << endl;
		break;
	case FA_FB:
		output_file << "FA_FB" << endl;
		break;
	case FA_UNB:
		output_file << "FA_UNB" << endl;
		break;
	case FA_UB:
		output_file << "FA_UB" << endl;
		break;
	case FS:
		output_file << "FS" << endl;
		break;
	case SC:
		output_file << "SC" << endl;
		break;
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
