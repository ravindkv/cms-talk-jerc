#ifndef GlobalFlag_H
#define GlobalFlag_H
        
#include <iostream>
#include <fstream>
#include <TString.h>

class GlobalFlag{
    public: 
        GlobalFlag(TString oName);
        ~GlobalFlag();
        
        bool isDebug;

        //Year 
        bool is22;
        bool is23;
        bool is24;

        //Eras 2022
        bool is22B;
        bool is22C;
        bool is22D;
        bool is22E;
        bool is22F;
        bool is22G;
        bool isPostEE;
        //Eras 2023
        bool is23A;
        bool is23B;
        bool is23C;
        bool is23Cv123;
        bool is23Cv4;
        bool is23D;
        bool is23E;
        bool is23F;
        bool is23G;
        bool is23Pre;
        bool is23Post;
        //Eras 2024
        bool is24B;
        bool is24C;
        bool is24D;
        bool is24E;
        bool is24F;
        bool is24G;

        //Data or MC
        bool isData;
        bool isMC;
        
        //Channels
        bool isGamJet;
        bool isZeeJet;
        bool isZmmJet;
        bool isMCTruth;
        bool isFlavour;
        bool isVetoMap;
        bool isDiJet;
        bool isIncJet;
        bool isMultiJet;
        bool isWqq;

        //Sample (e.g. QCD)
        bool isQCD;
        bool isMG;
        void printFlag();
};
#endif
