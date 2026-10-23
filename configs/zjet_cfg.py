# Z(->mumu)+jet selection. Run with:  myf-run configs/zjet_cfg.py --sample DYJetsToLL_M50_amcatnlo
import myf

process = myf.Process("zjet")

process.add("TriggerFilter", "trigger", paths=["HLT_IsoMu24", "HLT_IsoTkMu24"])
process.add("MuonSelector", "goodMuons", ptMin=20, etaMax=2.4, id="tight", isoMax=0.15)
process.add("JetSelector", "goodJets", ptMin=30, etaMax=2.4, muons="goodMuons", dRMin=0.4)
process.add("ZBuilder", "zCandidate", muons="goodMuons", leadPtMin=26, massMin=60, massMax=120)
process.add("ZJetHistos", "zHistos", z="zCandidate", jets="goodJets")        # inclusive Z
process.add("CountFilter", "atLeastOneJet", jets="goodJets", min=1)
process.add("ZJetHistos", "zjetHistos", z="zCandidate", jets="goodJets")     # Z + >= 1 jet
