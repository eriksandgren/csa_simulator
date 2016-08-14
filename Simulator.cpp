//  main.cpp
//  CSA_Simulator
//
//  Created by Erik Sandgren on 21/12/15.
//  Copyright © 2015 Erik Sandgren. All rights reserved.

#include <stdlib.h>
#include <iostream>
#include <ctime>
#include <cmath>
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
void evalLoadFa(int index);
void evalLoadFs(int index);
void evalLoadSc(int index);

void writeToOutputFile();

int main () {
	clock_t beginTime = clock();
	if(saveDelays)
	{
		delays.resize(gLength, vector<int>(maximumDelay, 0));
	}
	cout << "Frame Asynchronous Coded Slotted ALOHA simulation \n";
	cout << "n = " << n << "\n";
	cout << "d= " << d << "\n";
	cout << "Lambda = " << Lambda << "\n";
	cout << "Packets to simulate = " << numberOfPacketsToSimulateMax << "\n";
	cout << "Please find the results in " << fileName << "\n\n";
	cout << "**************** Progress *************** " << "\n\n";
	cout << "Load \t \t    Sent \t\t Lost \t \t   PLR \n";
	cout << "------------------------------------------------------- \n";
	gIndex = gLength-1;

	while(gIndex >= 0)
	{
		switch(typeOfSimulation)
		{
		case FA_FNB:
		case FA_FB:
		case FA_UB:
		case FA_UNB:
			evalLoadFa(gIndex);
			break;
		case FS:
			evalLoadFs(gIndex);
			break;
		case SC:
			evalLoadSc(gIndex);
			break;
		}
		gIndex--;
	}

	writeToOutputFile();
	clock_t endTime = clock();
	double elapsedSeconds = double(endTime-beginTime)/CLOCKS_PER_SEC;
	cout<< "Elapsed time is: " << elapsedSeconds << " seconds " << endl;
	return 0;
}

// This function is used to evaluate the performance for the given parameters (loop is usually over g, the system load).
void evalLoadFa(int index)
{
	int samp;
	unsigned long int timeStep = 0;
	Arrivals poiss = Arrivals(g[index]);
	Encoder enc = Encoder(n, n_rx, d, Lambda,q);
	Decoder dec = Decoder(numDecodingIterations, slotsBetweenDecoding, n, n_rx, maximumDelay);
	vector<Node*> VN;
	vector<Node*>CN(n + n_rx);
	int sentPacketsCount = 0;
	int lostPacketsCount = 0;
	Node* tempCn;
	Node* temp_VN;

	// Initialize CNs
	for (int i = 0; i < n_rx; i++) {
		tempCn = new Node();
		CN[i] = tempCn;
	}
	for (int i = n_rx; i < n_rx + n; i++) {
		timeStep++;
		tempCn = new Node(timeStep);
		CN[i] = tempCn;
	}

	timeStep = 0;

	// Running some steps without decoding in case the RX is not present at the start
	if(typeOfSimulation == FA_FNB || typeOfSimulation == FA_UNB){
		for (int i = 0; i < 2 * n_rx + n; i++) {
			timeStep++;
			// Create new CN and remove the oldest one.
			tempCn = new Node(timeStep);
			CN.push_back(tempCn);
			tempCn = CN[0];

			CN.erase(CN.begin());
			tempCn -> letGoOffNeighbours();
			delete tempCn;
			//Create new VN and distribute its packets...
			samp = poiss.sample();
			for (int i = 0; i < samp; i++)
			{
				temp_VN = new Node(timeStep);
				if(typeOfSimulation == FA_FNB || typeOfSimulation == FA_FB)
				{
					enc.distributeRepsFirstSlot(temp_VN, &CN);
				}
				else

				{
					enc.distributeRepsUniform(temp_VN, &CN);
				}
				VN.push_back(temp_VN);
			}

		}
	}

	while(sentPacketsCount < numberOfPacketsToSimulateMax && lostPacketsCount < numberOfPacketLossesToSimulateMax)
	{
		timeStep++;
		// Create new CN and remove the oldest one.
		tempCn = new Node(timeStep);
		CN.push_back(tempCn);
		tempCn = CN[0];
		CN.erase(CN.begin());
		tempCn -> letGoOffNeighbours();
		delete tempCn;

		//Create new VN and distribute its packets...
		samp = poiss.sample();

		for (int i = 0; i < samp; i++)
		{
			temp_VN = new Node(timeStep);
			if(typeOfSimulation == FA_FNB || typeOfSimulation == FA_FB)
			{
				enc.distributeRepsFirstSlot(temp_VN, &CN);
			}
			else

			{
				enc.distributeRepsUniform(temp_VN, &CN);
			}
			VN.push_back(temp_VN);
		}

		dec.decode(&CN, &VN, timeStep);

		// Counting packets
		if(typeOfSimulation == FA_FB || typeOfSimulation == FA_UB )
		{
			dec.countPacketsBoundaryEffect(&VN, timeStep);
		}
		else
		{
			dec.countPacketsNoBoundaryEffect(&VN, timeStep);
		}

		sentPacketsCount = dec.getSentPacketsCount();
		lostPacketsCount = dec.getLostPacketsCount();

	}
	VN.clear();
	CN.clear();
	if (saveDelays)
	{
		delays.at(index) = dec.getDelays();
	}
	plr.at(index) = (double) lostPacketsCount / (double) sentPacketsCount;
	streamsize normal_prec = cout.precision();
	cout << setprecision(3) << fixed;

	cout << g[index] << " \t \t " <<
	setw((int) log10(numberOfPacketsToSimulateMax) + 1)
	<< sentPacketsCount << " \t \t"
	<< setw( (int) log10(numberOfPacketLossesToSimulateMax) + 1 )
	<< lostPacketsCount << " \t \t " << scientific << plr.at(index) << endl;

	cout << setprecision(normal_prec);

}

void evalLoadFs(int index){
		n_rx = n;
        int samp;
        unsigned long int timeStep = 0;
        Arrivals poiss = Arrivals(g[index]);
    	Encoder enc = Encoder(n, n_rx, d, Lambda, q);
    	Decoder dec = Decoder(numDecodingIterations, slotsBetweenDecoding, n, n_rx, maximumDelay);
        vector<Node*> VN;
        vector<Node*> VNForNextFrame;
        vector<Node*>CN(2 * n);
        int sentPacketsCount = 0;
        int lostPacketsCount = 0;
        Node* tempCN;
        Node* tempVN;
        // Initialize CNs
        for (int i = 0; i < 2 * n; i++)
        {
            timeStep++;
            tempCN = new Node(timeStep);
            CN[i] = tempCN;
        }
        // time_step=;
        while(sentPacketsCount < numberOfPacketsToSimulateMax && lostPacketsCount < numberOfPacketLossesToSimulateMax)
        {
            timeStep++;
            //Create new VN and wait.
            samp = poiss.sample();
            for (int i = 0; i < samp; i++)
            {
                tempVN = new Node(timeStep);
                VNForNextFrame.push_back(tempVN);
            }

            if(saveDelays){dec.decode(&CN, &VN, timeStep);}
            if ( (timeStep % n) == 0 )
            {
                if(!saveDelays){
                    dec.decodeFrame(&CN, &VN, timeStep);
                }

                dec.countPacketsFS(&VN, timeStep);
                sentPacketsCount = dec.getSentPacketsCount();
                lostPacketsCount = dec.getLostPacketsCount();
                for (int i = 0; i < VNForNextFrame.size(); i++)
                {
                    tempVN = VNForNextFrame.at(i);
                    enc.distributeRepsUniform(tempVN, &CN);
                    VN.push_back(tempVN);
                }
                VNForNextFrame.clear();
            }

            // Create new CN and remove the oldest one.
            tempCN = new Node(timeStep);
            CN.push_back(tempCN);
            tempCN = CN[0];
            CN.erase(CN.begin());
            tempCN -> letGoOffNeighbours();
            delete tempCN;
        }
        VN.clear();
        CN.clear();
        plr[index] = (double) lostPacketsCount / (double) sentPacketsCount;
        if(saveDelays){
        delays.at(index) = dec.getDelays();
        }
        streamsize normal_prec = cout.precision();
        cout<<setprecision(3)<<fixed;

        cout << g[index] << " \t \t "
        << setw((int)log10(numberOfPacketsToSimulateMax) + 1)
		<< sentPacketsCount << " \t \t"
		<< setw((int)log10(numberOfPacketLossesToSimulateMax) + 1)
		<< lostPacketsCount << " \t \t "<< scientific<<plr[index] << endl;

        cout << setprecision(normal_prec);
};

void evalLoadSc(int index)
	{
		assert(1 == sizeof(d)/sizeof(int));
		int rep = d[0];
		assert((n % rep) == 0);
		int nSubFrame = n / rep;
		n_rx = 5 * rep * nSubFrame; // makes the memory "sufficiently" large
	    int samp;
	    unsigned long int timeStep = 0;
	    Arrivals poiss = Arrivals(g[index]);
    	Encoder enc = Encoder(nSubFrame, n_rx, d, Lambda, q);
	    Decoder dec = Decoder(numDecodingIterations, slotsBetweenDecoding, nSubFrame, n_rx, maximumDelay);

	    vector<Node*> VN;
	    vector<Node*> VNForNextFrame;
	    vector<Node*> CN(rep * nSubFrame + n_rx);
	    int sentPacketsCount = 0;
	    int lostPacketsCount = 0;
	    Node* tempCN;
	    Node* tempVN;

	    // Initialize CNs
	    for (int i = 0; i < n_rx; i++)
	    {
	        tempCN = new Node();
	        CN[i] = tempCN;
	    }
	    for (int i = n_rx; i < n_rx + rep * nSubFrame; i++)
	    {
	        timeStep++;
	        tempCN = new Node(timeStep);
	        CN[i] = tempCN;
	    }

	    timeStep = 0;


	    while(sentPacketsCount < numberOfPacketsToSimulateMax && lostPacketsCount < numberOfPacketLossesToSimulateMax)    {
	        // Arrivals at this time step, saved in the VNs_for_next_frame.
	        samp = poiss.sample();
	        for (int i = 0; i < samp; i++)
	        {
	            tempVN = new Node(timeStep);
	            VNForNextFrame.push_back(tempVN);
	        }
	        timeStep++;

	        // If its time for a new frame, we let the activated VNs distribute their 'd' replicas...
	        if( (timeStep % nSubFrame) == 0 )
	        {
	            for (int i = 0; i < VNForNextFrame.size();i++)
	            {
	                enc.distributeRepsSC(VNForNextFrame[i], &CN);
	            }
	            VN.insert(VN.end(), VNForNextFrame.begin(), VNForNextFrame.end());
	            VNForNextFrame.clear();
	        }


	        // Shifts in a new node at the back of the memory and removes the  node at the front (the oldest one)
	        tempCN = new Node(timeStep + rep * nSubFrame);
	        CN.push_back(tempCN);
	        tempCN = CN[0];
	        tempCN -> letGoOffNeighbours();
	        CN.erase(CN.begin());
	        delete tempCN;
	        dec.decode(&CN, &VN, timeStep);

	        // Counting packets: different method depending on the time_initial variable
	        dec.countPacketsSC(&VN, timeStep, rep);
	        sentPacketsCount = dec.getSentPacketsCount();
	        lostPacketsCount = dec.getLostPacketsCount();

	    }

	    VN.clear();
	    CN.clear();

	    if (saveDelays)
	    {
	        delays.at(index) = dec.getDelays();
	    }
	    plr .at(index) = (double) lostPacketsCount / (double) sentPacketsCount;
	    streamsize normal_prec = cout.precision();
	    cout<<setprecision(3) << fixed;
	    cout<< g[index] << " \t \t "<< setw( (int) log10(numberOfPacketsToSimulateMax) + 1)
	    << sentPacketsCount << " \t \t"
		<< setw( (int) log10(numberOfPacketLossesToSimulateMax) + 1)
		<< lostPacketsCount << " \t \t "<< scientific << plr.at(index) << endl;
	    cout << setprecision(normal_prec);
	}

void writeToOutputFile(){
	///////// WRITES TO OUTPUT ////////////////////////
	// where to save the output and by which name:
	ofstream outputFile;

	outputFile.open(fileName);
	outputFile << "Lambda:  ";
	for (int i = 0; i < q ; i++) {
		outputFile << Lambda[i] << " ";
	}
	outputFile << endl;

	outputFile << "degrees:  ";
	for (int i = 0; i < q ; i++) {
		outputFile << d[i] << " ";
	}
	outputFile << endl;

	outputFile << "n:  " << n << endl;

	outputFile<< "n_rx:  " << n_rx << endl;

	outputFile<< "maximum delay:  "<< maximumDelay << endl;

	outputFile << "Type of simulation: ";
	switch(typeOfSimulation){
	case FA_FNB:
		outputFile << "FA_FNB" << endl;
		break;
	case FA_FB:
		outputFile << "FA_FB" << endl;
		break;
	case FA_UNB:
		outputFile << "FA_UNB" << endl;
		break;
	case FA_UB:
		outputFile << "FA_UB" << endl;
		break;
	case FS:
		outputFile << "FS" << endl;
		break;
	case SC:
		outputFile << "SC" << endl;
		break;
	}
	outputFile << "Max number of packets to simulate:  " << numberOfPacketsToSimulateMax << endl;
	outputFile << "Max number of losses to simulate:  " << numberOfPacketLossesToSimulateMax << endl;

	outputFile << endl;
	outputFile << endl;
	outputFile << endl;
	outputFile << endl;

	outputFile << "Load \t PLR" << endl;
	for (int i = gLength - 1; i >= 0; i--)
	{
		outputFile << g[i] << "\t" << plr [i] << "\\\\" << endl;
	}
	outputFile << endl;
	outputFile << endl;
	outputFile << "Delay PDF (one row per load, not normalized)" << endl;

	if (saveDelays)
	{
		for (int i=0; i<gLength; i++)
		{
			for (int ii=0; ii < maximumDelay; ii++)
			{
				outputFile<< delays[i][ii]<<" ";
			}
			outputFile <<"\\\\"<<endl;
		}
	}
	outputFile.close();
}
