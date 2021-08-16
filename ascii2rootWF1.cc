/*
Valerio, AXIAL 2018
-------------------
6 floating (XY * (2 telescopes + 1 beam chamber))
12 intergers (6 strip/clu + 6 clu nr.)
24 integers (CAEN V1730 digitizer: 8 baselines + 8 wavef max + 8 max times)
5 floating (goniometer)
3 integers (spill nr + scan step + id_event)
-------------------
TOT 50 columns

==> 5.08.2018
righe dispari come sempre, righe pari 512 valori waveform

*/
#include <string>
#include <sstream>
#include "Riostream.h"
#include "TClassTable.h"
#include "TFile.h"
#include "TTree.h"

void ascii2rootWF1(const int ndigi=1, const int nsili=6, string inFileName="dummy", string outFileName="out") {

    
//  string outFileName = "root_"+removeExtension(inFileName)+".root";
  std::cout<<"---> Converting file "<<inFileName<<std::endl;
  std::cout<<"--->       into file "<<outFileName<<std::endl;
  ifstream myfile;
  string line;
  Int_t lineNumber=0;
  myfile.open(inFileName.c_str()); 

  // OUTPUT TREE DEFINITION  
  TFile* outFile = new TFile(outFileName.c_str(),"RECREATE"); 
  TTree* t = new TTree("t","Axial tree from ascii file 2018");
  //gROOT->cd();
  // silicon trackers
  Float_t hitPos[nsili];
  Int_t hitStrip[nsili];
  Int_t nClu[nsili];
  t->Branch("hitPos",hitPos,Form("hitPos[%d]/F",nsili));
  t->Branch("hitStrip",hitStrip,Form("hitStrip[%d]/I",nsili));
  t->Branch("nClu",nClu,Form("nClu[%d]/I",nsili));
  // digitizer
  Float_t digiBase[8*ndigi]; 
  Float_t digiPh[8*ndigi]; 
  Float_t digiTime[8*ndigi];
  t->Branch("digiBase",digiBase,Form("digiBase[%d]/F",8*ndigi));
  t->Branch("digiPh",digiPh,Form("digiPh[%d]/F",8*ndigi));
  t->Branch("digiTime",digiTime,Form("digiTime[%d]/F",8*ndigi));
  // gonio
  Float_t gonio[5];
  t->Branch("gonio",gonio,"gonio[5]/F");
  Int_t spill, step, eventNumber;
  t->Branch("spill",&spill,"spill/I");
  t->Branch("step",&step,"step/I");
  t->Branch("eventNumber",&eventNumber,"eventNumber/I");


  // waveform
  Int_t wf[512];
  t->Branch("wf",wf,Form("wf[%d]/I",512));
  Float_t wfBase=0;
  t->Branch("wfBase",&wfBase,"wfBase/I");
  Int_t wfArea[3],wfIntegral=0;
  t->Branch("wfArea",wfArea,Form("wfArea[%d]/I",3));
  // end of OUTPUT TREE DEF
  
  if (myfile.is_open()) {
    
    //    while (!myfile.eof()) {
    while(getline(myfile,line)){
      std::stringstream ss(line);      
      // first lines
      
      if (lineNumber%2==0) {
	for (Int_t i=0;i<nsili;i++) ss >> hitPos[i];
	for (Int_t i=0;i<nsili;i++) ss >> hitStrip[i];
	for (Int_t i=0;i<nsili;i++) ss >> nClu[i];
	
	for (Int_t idigi=0;idigi<8*ndigi;idigi++) ss >> digiBase[idigi] ;
	for (Int_t idigi=0;idigi<8*ndigi;idigi++) ss >> digiPh[idigi] ;
	for (Int_t idigi=0;idigi<8*ndigi;idigi++) ss >> digiTime[idigi] ;
	
	for (Int_t ig=0;ig<5;ig++) ss >> gonio[ig] ;
	
	ss >> spill >> step >> eventNumber;
	
      //std::cout <<  lineNumber<< std::endl;

      }
      // second lines WAVEFORM
      if (lineNumber%2==1) {
	wfBase=0;wfIntegral=0;
	for (Int_t i=0;i<519;i++) {
	  if (i>3 && i<516)ss >> wf[i-4];
	}
	
	// waveform analysys
	for(Int_t i=0;i<512;i++) {
	  // baseline 50
	  if (i<50) wfBase++;
	  wfBase=wfBase/50;
	  
	  if (i>=265 && i<400) {
	    wfIntegral+=wfBase-wf[i]; // negative signal
	    // 3 different integrals
	    if (i==275) wfArea[0]=-wfIntegral;
	    if (i==290) wfArea[1]=-wfIntegral;
	    if (i==310) wfArea[2]=-wfIntegral;
	  }
	  
	}
	// wf analysis end

	if (abs(hitPos[0]+hitPos[1]+hitPos[2]+hitPos[3]+hitPos[4]+hitPos[5])<100) {
	  t->Fill();
	}
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
