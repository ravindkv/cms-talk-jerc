
## https://cms-talk.web.cern.ch/t/XXX
* Replace XXX with the numbers mentioned in the name of each subdirectory after underscore
* https://cms-talk.web.cern.ch/t/45173

## Basic setups for EL9
* cmsrel CMSSW_13_3_0
* cd CMSSW_13_3_0/src
* cmsenv
* git clone git@github.com:ravindkv/cms-talk-jerc.git
* voms-proxy-init --voms cms --valid 24:00



For Debugging (or catching the segmentation violation)

* Compile the code with flag -g in the Makefile (GCC=g++ -g -std=c++17)
* Run the code with gdp (gdb --args ./makeHist -o Data_2024Cv1_DiJet_JetMET1_Hist_1of100.root)
* Once inside the gdb, type run
* Type quit to exit 
