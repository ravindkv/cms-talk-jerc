#include "HistIncJet.h"

#include "SkimTree.h"      
#include "EventPick.h"     
#include "ObjectPick.h"    
#include "ObjectScale.h"   

class VetoMapHistos
{
public:
  // Basic information about the trigger
  string trg;
  int trgpt;
  double ptMin, ptMax, absEtaMin, absEtaMax;
  TH1D *hNJet;
};

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

  TH1D *hNJet_Skim = new TH1D("hNJet_Skim", "hNJet_Skim", 10, 0.5, 10.5);
  TH1D *hNJet_Trig = new TH1D("hNJet_Trig", "hNJet_Trig", 10, 0.5, 10.5);
  TH1D *hNJet_Lumi = new TH1D("hNJet_Lumi", "hNJet_Lumi", 10, 0.5, 10.5);
  TH1D *hNJet_Filter = new TH1D("hNJet_Filter", "hNJet_Filter", 10, 0.5, 10.5);
  TH1D *hNJet_VetoMap = new TH1D("hNJet_VetoMap", "hNJet_VetoMap", 10, 0.5, 10.5);
  TH1D *hNJet_NoTrig = new TH1D("hNJet_NoTrig", "hNJet_NoTrig", 10, 0.5, 10.5);
 
  bool isZB = false;
  double fwdeta = 3.139;  // was 2.853. 80% (100%) on negative (positive) side
  double fwdeta0 = 2.964; // 2.853; // 40 and 260 up
  map<string, pair<const Bool_t*, struct TrigRange>> trigsName_ValRange;
  if (!isZB) {
  trigsName_ValRange["HLT_PFJet40"           ]  = make_pair(&tree->HLT_PFJet40            ,TrigRange{40, 40, 85, 0, 5.2   });
  trigsName_ValRange["HLT_PFJet60"           ]  = make_pair(&tree->HLT_PFJet60            ,TrigRange{60, 85, 100, 0, 5.2  });
  trigsName_ValRange["HLT_PFJet80"           ]  = make_pair(&tree->HLT_PFJet80            ,TrigRange{80, 100, 155, 0, 5.2 });
  trigsName_ValRange["HLT_PFJet140"          ]  = make_pair(&tree->HLT_PFJet140           ,TrigRange{140, 155, 210, 0, 5.2 });
  trigsName_ValRange["HLT_PFJet200"          ]  = make_pair(&tree->HLT_PFJet200           ,TrigRange{200, 210, 300, 0, 5.2 });
  trigsName_ValRange["HLT_PFJet260"          ]  = make_pair(&tree->HLT_PFJet260           ,TrigRange{260, 300, 400, 0, 5.2 });
  trigsName_ValRange["HLT_PFJet320"          ]  = make_pair(&tree->HLT_PFJet320           ,TrigRange{320, 400, 500, 0, 5.2 });
  trigsName_ValRange["HLT_PFJet400"          ]  = make_pair(&tree->HLT_PFJet400           ,TrigRange{400, 500, 600, 0, 5.2 });
  trigsName_ValRange["HLT_PFJet450"          ]  = make_pair(&tree->HLT_PFJet450           ,TrigRange{450, 500, 600, 0, 5.2 });
  trigsName_ValRange["HLT_PFJet500"          ]  = make_pair(&tree->HLT_PFJet500           ,TrigRange{500, 600, 6500, 0, 5.2});
  trigsName_ValRange["HLT_DiPFJetAve40"      ]  = make_pair(&tree->HLT_DiPFJetAve40       ,TrigRange{40, 40, 85, 0, 5.2   });
  trigsName_ValRange["HLT_DiPFJetAve60"      ]  = make_pair(&tree->HLT_DiPFJetAve60       ,TrigRange{60, 85, 100, 0, 5.2  });
  trigsName_ValRange["HLT_DiPFJetAve80"      ]  = make_pair(&tree->HLT_DiPFJetAve80       ,TrigRange{80, 100, 155, 0, 5.2 });
  trigsName_ValRange["HLT_DiPFJetAve140"     ]  = make_pair(&tree->HLT_DiPFJetAve140      ,TrigRange{140, 155, 210, 0, 5.2 });
  trigsName_ValRange["HLT_DiPFJetAve200"     ]  = make_pair(&tree->HLT_DiPFJetAve200      ,TrigRange{200, 210, 300, 0, 5.2 });
  trigsName_ValRange["HLT_DiPFJetAve260"     ]  = make_pair(&tree->HLT_DiPFJetAve260      ,TrigRange{260, 300, 400, 0, 5.2 });
  trigsName_ValRange["HLT_DiPFJetAve320"     ]  = make_pair(&tree->HLT_DiPFJetAve320      ,TrigRange{320, 400, 500, 0, 5.2 });
  trigsName_ValRange["HLT_DiPFJetAve400"     ]  = make_pair(&tree->HLT_DiPFJetAve400      ,TrigRange{400, 500, 600, 0, 5.2 });
  trigsName_ValRange["HLT_DiPFJetAve500"     ]  = make_pair(&tree->HLT_DiPFJetAve500      ,TrigRange{500, 600, 6500, 0, 5.2});
  trigsName_ValRange["HLT_DiPFJetAve60_HFJEC"]  = make_pair(&tree->HLT_DiPFJetAve60_HFJEC ,TrigRange{60, 85, 100, fwdeta, 5.2   });
  trigsName_ValRange["HLT_DiPFJetAve80_HFJEC"]  = make_pair(&tree->HLT_DiPFJetAve80_HFJEC ,TrigRange{80, 100, 125, fwdeta, 5.2  });
  trigsName_ValRange["HLT_DiPFJetAve100_HFJEC"] = make_pair(&tree->HLT_DiPFJetAve100_HFJEC,TrigRange{100, 125, 180, fwdeta, 5.2  });
  trigsName_ValRange["HLT_DiPFJetAve160_HFJEC"] = make_pair(&tree->HLT_DiPFJetAve160_HFJEC,TrigRange{160, 180, 250, fwdeta, 5.2  });
  trigsName_ValRange["HLT_DiPFJetAve220_HFJEC"] = make_pair(&tree->HLT_DiPFJetAve220_HFJEC,TrigRange{220, 250, 350, fwdeta0, 5.2 });
  trigsName_ValRange["HLT_DiPFJetAve300_HFJEC"] = make_pair(&tree->HLT_DiPFJetAve300_HFJEC,TrigRange{300, 350, 6500, fwdeta0, 5.2});
  }

  map<string, VetoMapHistos *> mapStrHistVetoMap;
  for (const auto& trigName_ValRange : trigsName_ValRange) {
    string trigName = trigName_ValRange.first;
    
    fout->mkdir(trigName.c_str());
    fout->cd(trigName.c_str());
    TDirectory *dout = gDirectory;

    const struct TrigRange &r = trigName_ValRange.second.second;
    VetoMapHistos *h = new VetoMapHistos();
    mapStrHistVetoMap[trigName] = h;
    h->trg = trigName;
    h->trgpt = r.ptThresh;

    h->ptMin = r.ptMin;
    h->ptMax = r.ptMax;
    h->absEtaMin = r.absEtaMin;
    h->absEtaMax = r.absEtaMax;
    h->hNJet = new TH1D("hNJet", "hNJet", 15, 0.5, 15.5);
  } // for itrg


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
    hNJet_Skim->Fill(tree->nJet);

    // Reco objects
    objP->clearObjects();
    objP->pickMuons();
    objP->pickElectrons();
    if(objP->pickedMuons.size() !=1) continue;
    if(objP->pickedElectrons.size() !=1) continue;
    vector<int>jets;
    for (int i = 0; i != tree->nJet; ++i) {
      if(tree->Jet_pt[i] > 30 && abs(tree->Jet_eta[i]) <2.5 &&  tree->Jet_jetId[i] & (1 << 1)){
			  double eta = tree->Jet_eta[i];
        double phi = tree->Jet_phi[i];
     	//loop over selected electrons
				bool passDR_lep_jet = true;
        for(std::vector<int>::const_iterator eleInd = objP->pickedElectrons.begin(); eleInd !=  objP->pickedElectrons.end(); eleInd++) {
            double dR_JetEle = objP->dR(eta, phi, tree->eleEta_[*eleInd], tree->elePhi_[*eleInd]);
	        if (dR_JetEle < 0.4) passDR_lep_jet = false;
        }
        //loop over selected muons
        for(std::vector<int>::const_iterator muInd = objP->pickedMuons.begin(); muInd != objP->pickedMuons.end(); muInd++) {
            double dR_JetMu = objP->dR(eta, phi, tree->muEta_[*muInd], tree->muPhi_[*muInd]);
	        if (dR_JetMu < 0.4) passDR_lep_jet = false;
        }
				if(passDR_lep_jet) jets.push_back(i);
      }
    }

    //------------------------------------
    // Basic event selections 
    //------------------------------------
    //if(!eventP->passHLT(tree)) continue; 
    if(isDebug) cout<<"passHLT"<<endl;
    hCutflow->Fill(countCut++, weight);
    hNJet_Trig->Fill(jets.size());

    bool passGoodLumi = true; 
    if(isData){
      passGoodLumi = objS->checkGoodLumi(tree->run, tree->luminosityBlock);
    }
    if(!passGoodLumi) continue; 
    if(isDebug) cout<<"passGoodLumi"<<endl;
    hCutflow->Fill(countCut++, weight);
    hNJet_Lumi->Fill(jets.size());

    if(!eventP->passFilter(tree)) continue; 
    if(isDebug) cout<<"passMetFilter"<<endl;
    hCutflow->Fill(countCut++, weight);
    hNJet_Filter->Fill(jets.size());

    if(objS->checkJetVetoMap(tree)) continue; 
    if(isDebug) cout<<"passJetVetoMap"<<endl;
    hCutflow->Fill(countCut++, weight);
    hNJet_VetoMap->Fill(jets.size());
    
    hNJet_NoTrig->Fill(jets.size());
    for (const auto& trigName_ValRange : trigsName_ValRange) {
      if (!(*trigName_ValRange.second.first)) continue;
      VetoMapHistos *h = mapStrHistVetoMap[trigName_ValRange.first];
      h->hNJet->Fill(jets.size());
    } // itrg
    
		//------------------------------------------------
    // Jet loop
		//------------------------------------------------
      /*
    for (int i = 0; i != tree->nJet; ++i) {
      //Data+MC: undo the correction already applied in NanoAOD
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
      
      //JER Corrections
      if(tree->Jet_pt[i] < 50) continue;
      double corrJER = objS->getJERCorrection(tree, i, "nom");
      //hDiff->Fill(corrJER);
      //p4Jet.SetPtEtaPhiM(tree->Jet_pt[i], tree->Jet_eta[i], tree->Jet_phi[i], tree->Jet_mass[i]);

    }// nJet
      */

  }//event for loop

  fout->Write();
  cout<<"--------: Cutlow summary :-------"<<endl;
  eventP->printBins(hCutflow);
  //eventP->printBins(hDiff);
  eventP->scanTFile(fout);
  cout<<"Output file: "<<fout->GetName()<<endl;
  fout->Close();
  return 0;
}
   
