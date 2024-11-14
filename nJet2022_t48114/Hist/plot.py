import ROOT

# List of directories
dirs = [
    "HLT_PFJet40", "HLT_PFJet60", "HLT_PFJet80", "HLT_PFJet140", "HLT_PFJet200",
    "HLT_PFJet260", "HLT_PFJet320", "HLT_PFJet400", "HLT_PFJet450", "HLT_PFJet500", 
    "HLT_DiPFJetAve40", "HLT_DiPFJetAve60", "HLT_DiPFJetAve80", "HLT_DiPFJetAve140", "HLT_DiPFJetAve200",
    "HLT_DiPFJetAve260", "HLT_DiPFJetAve320", "HLT_DiPFJetAve400", "HLT_DiPFJetAve500", "HLT_DiPFJetAve60_HFJEC",
    "HLT_DiPFJetAve80_HFJEC", "HLT_DiPFJetAve100_HFJEC", "HLT_DiPFJetAve160_HFJEC", "HLT_DiPFJetAve220_HFJEC",
    "HLT_DiPFJetAve300_HFJEC"
]

# Names of the two root files
fileTT_22   = "output/MC_2022_IncJet_TT_Hist_1of100.root"
fileTT_22EE = "output/MC_2022EE_IncJet_TT_Hist_1of100.root"

# Open the root files
fTT_22 = ROOT.TFile.Open(fileTT_22)
fTT_22EE = ROOT.TFile.Open(fileTT_22EE)

# Set the histogram name
hName = "hNJet"

# Loop over directories
for dirName in dirs:
    # Get histograms from each directory in the two files
    hTT_22 = fTT_22.Get(f"{dirName}/{hName}")
    hTT_22EE = fTT_22EE.Get(f"{dirName}/{hName}")
    hTT_22.SetTitle(dirName)
    hTT_22.GetXaxis().SetTitle("nJet")
    hTT_22.GetYaxis().SetTitle("Events normalised to 1")
    hTT_22.GetYaxis().CenterTitle()
    hTT_22.GetXaxis().SetTitleOffset(1.0)
    hTT_22.GetYaxis().SetTitleOffset(1.0)
    hTT_22.GetXaxis().SetTitleSize(0.05)
    hTT_22.GetYaxis().SetTitleSize(0.05)
    hTT_22.GetXaxis().SetTitleSize(0.05)
    hTT_22.GetYaxis().SetTitleSize(0.05)

    if hTT_22.Integral()!=0.0 and hTT_22EE.Integral()!=0.0:
        hTT_22.Scale(1/hTT_22.Integral())
        hTT_22EE.Scale(1/hTT_22EE.Integral())

    # Check if histograms exist
    if not hTT_22 or not hTT_22EE:
        print(f"Histogram {hName} not found in directory {dirName} for one of the files.")
        continue
    
    # Create a canvas
    canvas = ROOT.TCanvas("canvas", "Overlay", 800, 600)
    
    # Draw the histograms
    hTT_22.SetLineColor(ROOT.kRed)
    hTT_22EE.SetLineColor(ROOT.kBlue)
    
    # Create a legend
    legend = ROOT.TLegend(0.7, 0.6, 0.9, 0.7)
    legend.AddEntry(hTT_22EE, "fileTT_22EE", "l")
    legend.AddEntry(hTT_22, "fileTT_22", "l")
    legend.Draw()
    
    # Create the ratio plot
    canvas.cd()
    pad1 = ROOT.TPad("pad1", "Top pad", 0, 0.3, 1, 1)
    pad1.SetBottomMargin(0.02)
    pad1.Draw()
    pad1.cd()
    
    hTT_22.Draw("HIST")
    hTT_22EE.Draw("HIST SAME")
    legend.Draw()
    
    canvas.cd()
    pad2 = ROOT.TPad("pad2", "Bottom pad", 0, 0, 1, 0.3)
    pad2.SetTopMargin(0.02)
    pad2.SetBottomMargin(0.3)
    pad2.Draw()
    pad2.cd()
    
    # Create ratio plot
    ratio = hTT_22EE.Clone("ratio")
    ratio.SetLineColor(ROOT.kBlack)
    ratio.SetTitle("")
    ratio.SetStats(0)
    ratio.Divide(hTT_22)
    
    ratio.GetYaxis().SetTitle("Ratio")
    ratio.GetYaxis().SetNdivisions(505)
    ratio.GetYaxis().SetTitleSize(20)
    ratio.GetYaxis().SetTitleFont(43)
    ratio.GetYaxis().SetTitleOffset(1.55)
    ratio.GetYaxis().SetLabelFont(43)
    ratio.GetYaxis().SetLabelSize(15)
    
    ratio.GetXaxis().SetTitleSize(20)
    ratio.GetXaxis().SetTitleFont(43)
    ratio.GetXaxis().SetTitleOffset(4.0)
    ratio.GetXaxis().SetLabelFont(43)
    ratio.GetXaxis().SetLabelSize(15)
    ratio.GetYaxis().SetRangeUser(0.0, 3.0)
    
    ratio.Draw("ep")
    
    # Save the plot as a PDF
    canvas.SaveAs(f"plot_{dirName}.pdf")

# Close the root files
fTT_22.Close()
fTT_22EE.Close()

