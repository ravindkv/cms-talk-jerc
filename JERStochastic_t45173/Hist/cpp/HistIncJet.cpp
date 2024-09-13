#include "HistIncJet.h"

#include "SkimTree.h"      
#include "EventPick.h"     
#include "ObjectPick.h"    
#include "ObjectScale.h"   


int HistIncJet::Run(SkimTree *tree, EventPick *eventP, ObjectPick *objP, ObjectScale *objS, TFile *fout){
   
  TDirectory *curdir = gDirectory;
  assert(fout && !fout->IsZombie());
  string dir = (isMC ? "MC" : "DATA");

  // Cutflow histograms 
  vector<string> cuts;
  cuts.push_back("eventsInSkim");
  cuts.push_back("passHLT");
  cuts.push_back("passGoodLumi");
  cuts.push_back("passMetFilter");
  cuts.push_back("passJetVetoMap");
  cuts.push_back("passAtleast1Jet");
  TH1D *hCutflow = new TH1D("hCutflow",";hCutflow",cuts.size(),0.5,cuts.size()+0.5);
  for (int i = 0; i != cuts.size(); ++i) {
    hCutflow->GetXaxis()->SetBinLabel(i+1, cuts[i].c_str());
  }
  TH1D *hDiff = new TH1D("hDiff",";cJER_Twiki - cJER_CMSSW",100,-1.0, 1.0);


  fout->mkdir("Incjet");
  fout->cd("Incjet");
 
  //------------------------------------
  // Event for loop 
  //------------------------------------
  TLorentzVector p4Jet;

  Long64_t nentries = tree->GetEntries(); 
  cout << "\nStarting loop over " << nentries << " entries" << endl;
 
  std::cout<<"---------------------------"<<std::endl;
  std::cout<<setw(10)<<"Progress"<<setw(10)<<"Time"<<std::endl;
  std::cout<<"---------------------------"<<std::endl;
  double totTime = 0.0;
	auto startClock = std::chrono::high_resolution_clock::now();
 
  for (Long64_t jentry=0; jentry<nentries;jentry++) {
    //if (jentry>100) break;
		if(nentries > 100  && jentry%(nentries/100) == 0){// print after every 1% of events
      totTime+= std::chrono::duration<double>(std::chrono::high_resolution_clock::now()-startClock).count();
      int sec = (int)(totTime)%60;
      int min = (int)(totTime)/60;
	    std::cout<<setw(10)<<100*jentry/nentries<<" %"<<setw(10)<<min<<"m "<<sec<<"s"<<std::endl;
			startClock = std::chrono::high_resolution_clock::now();
		}
   
    Long64_t ientry = tree->loadEntry(jentry);
    if (ientry < 0) break; 
    tree->fChain->GetTree()->GetEntry(ientry);
    //if(isDebug) cout<<"Loaded jentry = "<<jentry<<endl;

    // Weight
    double weight = (isMC ? tree->genWeight : 1);
    int countCut = 0;
    hCutflow->Fill(countCut++, weight);

    //------------------------------------
    // Basic event selections 
    //------------------------------------
    //if(!eventP->passHLT(tree)) continue; 
    if(isDebug) cout<<"passHLT"<<endl;
    hCutflow->Fill(countCut++, weight);

    bool passGoodLumi = true; 
    if(isData){
      passGoodLumi = objS->checkGoodLumi(tree->run, tree->luminosityBlock);
    }
    if(!passGoodLumi) continue; 
    if(isDebug) cout<<"passGoodLumi"<<endl;
    hCutflow->Fill(countCut++, weight);

    if(!eventP->passFilter(tree)) continue; 
    if(isDebug) cout<<"passMetFilter"<<endl;
    hCutflow->Fill(countCut++, weight);

    if(objS->checkJetVetoMap(tree)) continue; 
    if(isDebug) cout<<"passJetVetoMap"<<endl;
    hCutflow->Fill(countCut++, weight);

    if(tree->nJet < 1) continue; 
    if(isDebug) cout<<"passAtleast1Jet"<<endl;
    hCutflow->Fill(countCut++, weight);

		//------------------------------------------------
    // Jet loop
		//------------------------------------------------
    for (int i = 0; i != tree->nJet; ++i) {
      //Data+MC: undo the correction already applied in NanoAOD
      /*
      tree->Jet_pt[i]   = tree->Jet_pt[i] * (1.0 - tree->Jet_rawFactor[i]);
      tree->Jet_mass[i] = tree->Jet_mass[i] * (1.0 - tree->Jet_rawFactor[i]);

      //Data+MC: compute the L1RC correction and apply to the uncorrected jet;
      double corrL1FastJet = objS->getL1FastJetCorrection(tree->Jet_area[i], tree->Jet_eta[i], 
                                            tree->Jet_pt[i], tree->Rho);
      tree->Jet_pt[i]   = tree->Jet_pt[i] * corrL1FastJet;
      tree->Jet_mass[i] = tree->Jet_mass[i] * corrL1FastJet;

      //Data+MC: compute the MCTruth correction and apply to the L1RC corrected jet;
      double corrL2Relative = objS->getL2RelativeCorrection(tree->Jet_eta[i], tree->Jet_phi[i],tree->Jet_pt[i]);
      tree->Jet_pt[i]   = tree->Jet_pt[i] * corrL2Relative;
      tree->Jet_mass[i] = tree->Jet_mass[i] * corrL2Relative;

      //Only Data: compute the L2Residual+L3Residual correction and apply to the MCTruth corrected jet;
      if(isData){
        double corrL2L3Residual = objS->getL2L3ResidualCorrection(tree->Jet_eta[i], tree->Jet_pt[i]);
        tree->Jet_pt[i]   = tree->Jet_pt[i] * corrL2L3Residual;
        tree->Jet_mass[i] = tree->Jet_mass[i] * corrL2L3Residual;
      }
      
      */
      //JER Corrections
      if(tree->Jet_pt[i] < 50) continue;
      double corrJER = objS->getJERCorrection(tree, i, "nom");
      hDiff->Fill(corrJER);
      //p4Jet.SetPtEtaPhiM(tree->Jet_pt[i], tree->Jet_eta[i], tree->Jet_phi[i], tree->Jet_mass[i]);

    }// nJet

  }//event for loop

  fout->Write();
  cout<<"--------: Cutlow summary :-------"<<endl;
  eventP->printBins(hCutflow);
  eventP->printBins(hDiff);
  eventP->scanTFile(fout);
  cout<<"Output file: "<<fout->GetName()<<endl;
  fout->Close();
  return 0;
}
   
