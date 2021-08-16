/*
Valerio, KLEVER 2018
-------------------
     <row 1>
     1        [1]  n event
     2        [1]  unix time
     3--6     [4]  tele x,y,x,y
     7--11    [5]  gonio parameters
     12--13   [2]  step, info
     14--37   [24] 1 digi = 8 * (BASELINE, MAX, MAX TIME)
     38--45   [8]  nclu on beam chambers and barca
     <row 2>

     <row 3>
     ...
     ....
     <row 9>
     1--10    [10] 5 cluster position + 5 cluster ph
     11       [1]  total charge on the silicon plane

*/

#include <string>
#include <sstream>
#include "Riostream.h"
#include "TClassTable.h"
#include "TFile.h"
#include "TTree.h"

void ascii2root(const int ndigi=1, string inFileName="dummy", string outFileName="out") {

    
//  string outFileName = "root_"+removeExtension(inFileName)+".root";
  std::cout<<"---> Converting file "<<inFileName<<std::endl;
  std::cout<<"--->       into file "<<outFileName<<std::endl;
  ifstream myfile;
  string line;
  Int_t lineNumber=0;
  myfile.open(inFileName.c_str()); 

  // OUTPUT TREE DEFINITION  
  TFile* outFile = new TFile(outFileName.c_str(),"RECREATE"); 
  TTree* t = new TTree("t","Klever tree from ascii file 2018");
  
  // silicon trackers
  Float_t telePos[4];
  t->Branch("telePos",telePos,"telePos[4]/F");
  Int_t bcClu[8];
  t->Branch("bcClu",bcClu,"bcClu[8]/I");

  Float_t bc4Pos[5],bc4Charge[5],bc4TotCharge;
  t->Branch("bc4Pos",bc4Pos,"bc4Pos[5]/F");  
  t->Branch("bc4Charge",bc4Charge,"bc4Charge[5]/F"); 
  t->Branch("bc4TotCharge",&bc4TotCharge,"bc4TotCharge/I");
  Float_t bc5Pos[5],bc5Charge[5],bc5TotCharge;
  t->Branch("bc5Pos",bc5Pos,"bc5Pos[5]/F");  
  t->Branch("bc5Charge",bc5Charge,"bc5Charge[5]/F"); 
  t->Branch("bc5TotCharge",&bc5TotCharge,"bc5TotCharge/I");
  Float_t bc6Pos[5],bc6Charge[5],bc6TotCharge;
  t->Branch("bc6Pos",bc6Pos,"bc6Pos[5]/F");  
  t->Branch("bc6Charge",bc6Charge,"bc6Charge[5]/F"); 
  t->Branch("bc6TotCharge",&bc6TotCharge,"bc6TotCharge/I");
  Float_t bc7Pos[5],bc7Charge[5],bc7TotCharge;
  t->Branch("bc7Pos",bc7Pos,"bc7Pos[5]/F");  
  t->Branch("bc7Charge",bc7Charge,"bc7Charge[5]/F"); 
  t->Branch("bc7TotCharge",&bc7TotCharge,"bc7TotCharge/I");
  Float_t bc8Pos[5],bc8Charge[5],bc8TotCharge;
  t->Branch("bc8Pos",bc8Pos,"bc8Pos[5]/F");  
  t->Branch("bc8Charge",bc8Charge,"bc8Charge[5]/F"); 
  t->Branch("bc8TotCharge",&bc8TotCharge,"bc8TotCharge/I");
  Float_t bc9Pos[5],bc9Charge[5],bc9TotCharge;
  t->Branch("bc9Pos",bc9Pos,"bc9Pos[5]/F");  
  t->Branch("bc9Charge",bc9Charge,"bc9Charge[5]/F"); 
  t->Branch("bc9TotCharge",&bc9TotCharge,"bc9TotCharge/I");
  Float_t bc10Pos[5],bc10Charge[5],bc10TotCharge;
  t->Branch("bc10Pos",bc10Pos,"bc10Pos[5]/F");  
  t->Branch("bc10Charge",bc10Charge,"bc10Charge[5]/F"); 
  t->Branch("bc10TotCharge",&bc10TotCharge,"bc10TotCharge/I");
  Float_t bc11Pos[5],bc11Charge[5],bc11TotCharge;
  t->Branch("bc11Pos",bc11Pos,"bc11Pos[5]/F");  
  t->Branch("bc11Charge",bc11Charge,"bc11Charge[5]/F"); 
  t->Branch("bc11TotCharge",&bc11TotCharge,"bc11TotCharge/I");
  

  // digitizer
  Int_t digiBase[8*ndigi]; 
  Int_t digiPh[8*ndigi]; 
  Int_t digiTime[8*ndigi];
  t->Branch("digiBase",digiBase,Form("digiBase[%d]/I",8*ndigi));
  t->Branch("digiPh",digiPh,Form("digiPh[%d]/I",8*ndigi));
  t->Branch("digiTime",digiTime,Form("digiTime[%d]/I",8*ndigi));
  // gonio
  Float_t gonio[5];
  t->Branch("gonio",gonio,"gonio[5]/F");
  Int_t spill, step, eventNumber, unixTime;
  Float_t info;
  t->Branch("spill",&spill,"spill/I");
  t->Branch("step",&step,"step/I");
  t->Branch("eventNumber",&eventNumber,"eventNumber/I");
  t->Branch("unixTime",&unixTime,"unixTime/I");
  t->Branch("info",&info,"info/F");


  // end of OUTPUT TREE DEF
  
  if (myfile.is_open()) {
    
    while(getline(myfile,line)) {
      std::stringstream ss(line);      
      // first lines
      
      if (lineNumber%9==0) {
	if (lineNumber>0) {
	  if (abs(telePos[0]+telePos[1]+telePos[2]+telePos[3])<100) {
	    t->Fill();
	  }
	}
	ss >> unixTime >> eventNumber ;
	for (Int_t i=0;i<4;i++) ss >> telePos[i];
	for (Int_t i=0;i<5;i++) ss >> gonio[i];
	ss >> spill >> step ;
	for (Int_t idigi=0;idigi<8*ndigi;idigi++) ss >> digiBase[idigi] ;
	for (Int_t idigi=0;idigi<8*ndigi;idigi++) ss >> digiPh[idigi] ;
	for (Int_t idigi=0;idigi<8*ndigi;idigi++) ss >> digiTime[idigi] ;
	for (Int_t i=0;i<8;i++) ss >> bcClu[i];


      }
      // OPPURE 
      if (lineNumber%9==1) {
	for (Int_t i=0;i<5;i++) ss >> bc4Pos[i];
	for (Int_t i=0;i<5;i++) ss >> bc4Charge[i];
	ss >> bc4TotCharge;
      }
      if (lineNumber%9==2) {
	for (Int_t i=0;i<5;i++) ss >> bc5Pos[i];
	for (Int_t i=0;i<5;i++) ss >> bc5Charge[i];
	ss >> bc5TotCharge;
      }
      if (lineNumber%9==3) {
	for (Int_t i=0;i<5;i++) ss >> bc6Pos[i];
	for (Int_t i=0;i<5;i++) ss >> bc6Charge[i];
	ss >> bc6TotCharge;
      }
      if (lineNumber%9==4) {
	for (Int_t i=0;i<5;i++) ss >> bc7Pos[i];
	for (Int_t i=0;i<5;i++) ss >> bc7Charge[i];
	ss >> bc7TotCharge;
      }
      if (lineNumber%9==5) {
	for (Int_t i=0;i<5;i++) ss >> bc8Pos[i];
	for (Int_t i=0;i<5;i++) ss >> bc8Charge[i];
	ss >> bc8TotCharge;
      }
      if (lineNumber%9==6) {
	for (Int_t i=0;i<5;i++) ss >> bc9Pos[i];
	for (Int_t i=0;i<5;i++) ss >> bc9Charge[i];
	ss >> bc9TotCharge;
      }
      if (lineNumber%9==7) {
	for (Int_t i=0;i<5;i++) ss >> bc10Pos[i];
	for (Int_t i=0;i<5;i++) ss >> bc10Charge[i];
	ss >> bc10TotCharge;
      }
      if (lineNumber%9==8) {
	for (Int_t i=0;i<5;i++) ss >> bc11Pos[i];
	for (Int_t i=0;i<5;i++) ss >> bc11Charge[i];
	ss >> bc11TotCharge;
      }
      
    
      lineNumber++;
    }
      
    

    myfile.close();

  }

  else {
    std::cout<<"Can't open file!!"<<std::endl;
  }
  
  outFile->cd();
  t->Write();
  
  outFile->Close();
    
  return;
}
