/*
Valerio, KLEVER 2021

-------------------
     <row 1, EVENT N, 104 fields>
     0--7     [8]   xRaw
     8--15    [8]   nStripHit
     16--23   [8]   nHit

     24--31   [8]   digitizer 1, baselines
     32--39   [8]   digitizer 2, baselines 
     40--47   [8]   digitizer 3, baselines

     48--49   [8]   digitizer 1, pulse heights
     56--63   [8]   digitizer 2, pulse heights
     64--71   [8]   digitizer 3, pulse heights 

     72--79   [8]   digitizer 1, time of max
     80--87   [8]   digitizer 2, time of max
     88--95   [8]   digitizer 3, time of max

     96--100  [5]   goniometer parameters

     101      [1]   number of spill
     102      [1]   number of step
     103      [1]   event number

    
    <row 2>
    0--263    [263] waveform nr 1
    
    <row 3>
    0--263    [263] waveform nr 2
    
    <row 4>
    0--263    [263] waveform nr 3

    <row5, EVENT N+1, 104 fields>
    ...
    ...
*/

#include <string>
#include <sstream>
#include "Riostream.h"
#include "TClassTable.h"
#include "TFile.h"
#include "TTree.h"

void ascii2root(string inFileName="dummy", string outFileName="out") {

    
//  string outFileName = "root_"+removeExtension(inFileName)+".root";
  std::cout<<"---> Converting file "<<inFileName<<std::endl;
  std::cout<<"--->       into file "<<outFileName<<std::endl;
  ifstream myfile;
  string line;
  Int_t lineNumber=0;
  myfile.open(inFileName.c_str()); 

  // OUTPUT TREE DEFINITION  
  TFile* outFile = new TFile(outFileName.c_str(),"RECREATE"); 
  TTree* t = new TTree("t","Klever tree from ascii file 2021");
  
  // silicon trackers
  Float_t xRaw[8];        t->Branch("xRaw",xRaw,"xRaw[8]/F");
  UShort_t nStripHit[8];     t->Branch("nStripHit",nStripHit,"nStripHit[8]/s");
  UShort_t nHit[8];          t->Branch("nHit",nHit,"nHit[8]/s");


  // Digitizer baselines, pulse heights and time of max
  const int ndigi = 3 ;
  UShort_t digiBase[8*ndigi];    t->Branch("digiBase",digiBase,Form("digiBase[%d]/s",8*ndigi));
  UShort_t digiPh[8*ndigi];      t->Branch("digiPh",digiPh,Form("digiPh[%d]/s",8*ndigi));
  UShort_t digiTime[8*ndigi];    t->Branch("digiTime",digiTime,Form("digiTime[%d]/s",8*ndigi));

  // Goniometer
  Float_t gonio[5];           t->Branch("gonio",gonio,"gonio[5]/F");

  // Spill, step and event... numbers.
  Int_t spill, step, eventNumber;
  t->Branch("spill",&spill,"spill/I");
  t->Branch("step",&step,"step/I");
  t->Branch("eventNumber",&eventNumber,"eventNumber/I");

  // Waveforms

  UShort_t wave0[256], wave1[256], wave2[256];
  t->Branch("wave0",wave0,"wave0[256]/s");
  t->Branch("wave1",wave1,"wave1[256]/s");
  t->Branch("wave2",wave2,"wave2[256]/s");

  // end of OUTPUT TREE DEF
  Int_t dummy=0;

  if (myfile.is_open()) {
    
    while(getline(myfile,line)) {
      std::stringstream ss(line);      

      // HEADER LINES
      if (lineNumber%4==0) {

        	if (lineNumber>0) t->Fill();

          for (Int_t i=0;i<8;i++) ss >> xRaw[i];
          for (Int_t i=0;i<8;i++) ss >> nStripHit[i];
          for (Int_t i=0;i<8;i++) ss >> nHit[i];

          for (Int_t idigi=0;idigi<8*ndigi;idigi++) ss >> digiBase[idigi] ;
          for (Int_t idigi=0;idigi<8*ndigi;idigi++) ss >> digiPh[idigi] ;
          for (Int_t idigi=0;idigi<8*ndigi;idigi++) ss >> digiTime[idigi] ;
          
          for (Int_t i=0;i<5;i++) ss >> gonio[i];

          ss >> spill >> step >> eventNumber ;
      }

      // OTHER LINES (Waveforms) 
      if (lineNumber%4==1) {
        for (Int_t i=0;i<5;i++)   ss >> dummy ;
  	    for (Int_t i=0;i<256;i++) ss >> wave0[i];
        for (Int_t i=0;i<2;i++)   ss >> dummy ;
      }
      if (lineNumber%4==2) {
       for (Int_t i=0;i<5;i++)   ss >> dummy ;
        for (Int_t i=0;i<256;i++) ss >> wave1[i];
        for (Int_t i=0;i<2;i++)   ss >> dummy ;
      }
      if (lineNumber%4==3) {
        for (Int_t i=0;i<5;i++)   ss >> dummy ;
        for (Int_t i=0;i<256;i++) ss >> wave2[i];
        for (Int_t i=0;i<2;i++)   ss >> dummy ;
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
