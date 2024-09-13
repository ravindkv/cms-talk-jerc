#include "HistIncJet.h"
#include "SkimTree.h"
#include "EventPick.h"
#include "ObjectScale.h"
#include "ObjectPick.h"
#include "GlobalFlag.h"

#include <sys/stat.h>
#include <sys/types.h>
#include <nlohmann/json.hpp>
#include <boost/algorithm/string.hpp>

using namespace std;

int main(int argc, char* argv[]){
    string fileDefault = "input/json/FilesSkim_2024_IncJet.json";//default file
    ifstream fileDefault_(fileDefault.c_str());
    nlohmann::json js; 
    try{
        js = nlohmann::json::parse(fileDefault_);
    } catch (const exception& e) {
        cout<<"\nEXCEPTION: Check the input json fileDefault: "<<fileDefault<<endl;
        cout<<e.what()<<endl;
        abort();
    }
    //--------------------------------
    // Parse command-line options
    //--------------------------------
    int opt;
    string outName; //output Name
    outName = js.begin().key()+"_Hist_1of100.root"; //defualt value
    while ((opt = getopt(argc, argv, "o:h")) != -1) {
        switch (opt) {
            case 'o':
                outName = optarg;
                break;
            case 'h':
                cout<<"Default input json: "<<fileDefault<<endl;
                cout << "Usage: ./makeHist -o sKey_Hist_1of100.root\n" << endl;
                cout<<"Choose sKey from the following:"<<endl;
                for (auto& element : js.items()) {
                    cout << element.key() << endl;
                }
                return 0;
            default:
                cerr << "Invalid option" << endl;
                return 1;
        }
    }
    cout<<"\n./makeHist -o " <<outName<<endl;
    TString oName = outName;

    cout<<"\n--------------------------------------"<<endl;
    cout<<" Set GlobalFlag.cpp"<<endl;
    cout<<"--------------------------------------"<<endl;
    GlobalFlag *globF =  new GlobalFlag(oName);
    globF->printFlag();

    cout<<"\n--------------------------------------"<<endl;
    cout<<" Set and load SkimTree.cpp"<<endl;
    cout<<"--------------------------------------"<<endl;
    SkimTree *skimT = new SkimTree(oName);
    skimT->setInput(outName);
    skimT->loadInput();
    cout<<endl;
    skimT->setInputJsonPath("input/json/");
    skimT->loadInputJson();
    cout<<endl;
    skimT->loadJobFileNames();
    cout<<endl;
    skimT->loadTree();
    cout<<endl;

    cout<<"\n--------------------------------------"<<endl;
    cout<<" Set and load EventPick.cpp"<<endl;
    cout<<"--------------------------------------"<<endl;
    EventPick *eventP = new EventPick(oName);

    cout<<"\n--------------------------------------"<<endl;
    cout<<" Set and load ObjectPick.cpp"<<endl;
    cout<<"--------------------------------------"<<endl;
    ObjectPick *objP = new ObjectPick(oName);
    objP->setTree(skimT);

    cout<<"\n--------------------------------------"<<endl;
    cout<<" Set and load ObjectScale.cpp"<<endl;
    cout<<"--------------------------------------"<<endl;
    ObjectScale *objS = new ObjectScale(oName);
    //Jet veto 
    objS->setJetVetoKey(oName); 
    objS->setJetVetoName(oName); 
    objS->setJetVetoJsonPath(oName); 
    objS->loadJetVetoRef(); 

    //Jet L1FastJet 
    objS->setJetL1FastJetName(oName); 
    objS->setJetL1FastJetJsonPath(oName); 
    objS->loadJetL1FastJetRef(); 
    cout<<endl;

    //Jet L2Relative 
    objS->setJetL2RelativeName(oName); 
    objS->setJetL2RelativeJsonPath(oName); 
    objS->loadJetL2RelativeRef(); 
    cout<<endl;

    //Jet L2L3Residual 
    objS->setJetL2L3ResidualName(oName); 
    objS->setJetL2L3ResidualJsonPath(oName); 
    objS->loadJetL2L3ResidualRef(); 
    cout<<endl;

    //Jet L2L3 
    objS->setJetL2L3Names(oName); 
    objS->setJetL2L3JsonPath(oName); 
    objS->loadJetL2L3Refs(); 
    cout<<endl;

    // JER Resolution 
    objS->setJERResoName(oName); 
    objS->setJERResoJsonPath(oName); 
    objS->loadJERResoRef(); 
    cout<<endl;

    // JER SF 
    objS->setJERSFName(oName); 
    objS->setJERSFJsonPath(oName); 
    objS->loadJERSFRef(); 
    cout<<endl;

    if(globF->isGamJet){//Scale and Smearing
        //Photon SS 
        objS->setPhoSsName(oName); 
        objS->setPhoSsJsonPath(oName); 
        objS->loadPhoSsRef(); 
        cout<<endl;
        
        //Electron SS 
        objS->setEleSsName(oName); 
        objS->setEleSsJsonPath(oName); 
        objS->loadEleSsRef(); 
        cout<<endl;
    }

    //Lumi
    if (globF->isData){
        objS->setLumiJsonPath(oName); 
        objS->loadLumiJson(); 
        cout<<endl;
    }

    string outDir = "output";
    mkdir(outDir.c_str(), S_IRWXU);
    TFile *fout = new TFile(outDir+"/"+oName, "RECREATE");

    cout<<"\n--------------------------------------"<<endl;
    cout<<" Loop over events and fill Histos"<<endl;
    cout<<"--------------------------------------"<<endl;
    if(globF->isIncJet){
      cout<<"==> Running IncJet"<<endl;
      HistIncJet *incJet = new HistIncJet(oName);
      incJet->Run(skimT, eventP, objP, objS, fout);  
    }
  return 0;
}
