#--------------------------
#MC
#--------------------------
# Sample key naming convention = MC_Year_Channel_SampleName
# SampleName should not contain minus (-)
def getMC_IncJet():
    samps = {}
    samps['MC_2022_IncJet_TT']  = '/TT_TuneCP5_13p6TeV_powheg-pythia8/Run3Summer22NanoAODv12-130X_mcRun3_2022_realistic_v5-v2/NANOAODSIM'
    samps['MC_2022EE_IncJet_TT']  = '/TT_TuneCP5_13p6TeV_powheg-pythia8/Run3Summer22EENanoAODv12-Poisson70KeepRAW_130X_mcRun3_2022_realistic_postEE_v6_ext1-v2/NANOAODSIM'
    return samps

def getData_IncJet():
    samps = {}
    samps['Data_2022B_IncJet_EGamma']       = '/EGamma/Run2022B-22Sep2023-v2/NANOAOD' 
    samps['Data_2022C_IncJet_EGamma']       = '/EGamma/Run2022C-22Sep2023-v1/NANOAOD' 
    samps['Data_2022D_IncJet_EGamma']       = '/EGamma/Run2022D-22Sep2023-v1/NANOAOD'
    samps['Data_2022E_IncJet_EGamma']       = '/EGamma/Run2022E-22Sep2023-v1/NANOAOD'
    samps['Data_2022F_IncJet_EGamma']       = '/EGamma/Run2022F-22Sep2023-v1/NANOAOD'
    samps['Data_2022G_IncJet_EGamma']       = '/EGamma/Run2022G-22Sep2023-v2/NANOAOD'
    return samps

def sampleDict():
    allSamples = {}
    allSamples.update(getMC_IncJet())
    allSamples.update(getData_IncJet())
    return allSamples

if __name__=="__main__":
    samp = sampleDict()
    print(samp)

