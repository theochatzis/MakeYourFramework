#!/usr/bin/env python3
"""Data vs MC comparison of the histograms written by myf-run.
Example: plot_zmass.py --data output/SingleMuon_2016H.root --mc output/DYJetsToLL_M50_amcatnlo.root
MC is normalised to xsec * lumi / sum(genWeight). Without --lumi (in /pb) it is scaled to the data yield."""
import argparse
import json
import os

import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt
import mplhep as hep
import numpy as np
import uproot

parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
parser.add_argument("--data", required=True)
parser.add_argument("--mc", required=True)
parser.add_argument("--lumi", type=float, default=None, help="integrated luminosity of the data in /pb")
parser.add_argument("--histos", nargs="+", default=["zHistos/mass", "zjetHistos/mass", "zjetHistos/zPt",
                                                      "zjetHistos/jet1Pt", "zjetHistos/nJets", "zjetHistos/dPhiZJet"])
parser.add_argument("--outdir", default="plots")
args = parser.parse_args()

XLABELS = {"mass": r"$m_{\mu\mu}$ [GeV]", "zPt": r"$p_{T}^{Z}$ [GeV]", "mu1Pt": r"leading muon $p_{T}$ [GeV]",
           "nJets": "number of jets", "jet1Pt": r"leading jet $p_{T}$ [GeV]", "dPhiZJet": r"$|\Delta\phi(Z, j_1)|$"}


def counts(f):
    h = f["myf/counts"]
    return dict(zip(h.axis().labels(), h.values()))


data, mc = uproot.open(args.data), uproot.open(args.mc)
mc_sample = json.loads(mc["myf/sample"].member("fTitle"))
mc_counts = counts(mc)
print(f"[myf] MC: {mc_counts['eventsRead']:.0f} events read, sum of weights {mc_counts['sumGenWeight']:.4g}")

os.makedirs(args.outdir, exist_ok=True)
hep.style.use("CMS")
for name in args.histos:
    d, m = data[name], mc[name]
    edges = d.axis().edges()
    yd = d.values()
    ym, vm = m.values(), m.variances()

    if args.lumi:
        scale = mc_sample["xsec"] * args.lumi / mc_counts["sumGenWeight"]
        label = f"{args.lumi / 1000:.3g} fb$^{{-1}}$ (13 TeV)"
    else:
        scale = yd.sum() / ym.sum()
        label = "MC scaled to data (13 TeV)"
    ym, vm = ym * scale, vm * scale**2

    fig, (ax, rax) = plt.subplots(2, 1, figsize=(10, 10), gridspec_kw={"height_ratios": (3, 1)}, sharex=True)
    hep.histplot(ym, edges, ax=ax, histtype="fill", color="#5790fc", label=r"Z/$\gamma^*$ $\to \ell\ell$ (MC)")
    ax.stairs(ym + np.sqrt(vm), edges, baseline=ym - np.sqrt(vm), fill=True, hatch="///", alpha=0.3,
              color="gray", label="MC stat. unc.")
    hep.histplot(yd, edges, ax=ax, histtype="errorbar", yerr=np.sqrt(yd), color="black", label="Data")
    hep.cms.label("Open Data", data=True, rlabel=label, ax=ax)
    ax.set_ylabel("Events")
    ax.legend()

    with np.errstate(divide="ignore", invalid="ignore"):
        ratio, err = yd / ym, np.sqrt(yd) / ym
    bad = ym <= 0  # empty bins, or negative ones from negative NLO weights
    ratio[bad], err[bad] = np.nan, 0
    hep.histplot(ratio, edges, yerr=err, ax=rax, histtype="errorbar", color="black")
    rax.axhline(1, color="gray", ls="--")
    rax.set_ylim(0.5, 1.5)
    rax.set_ylabel("Data / MC")
    rax.set_xlabel(XLABELS.get(name.split("/")[-1], name))

    out = os.path.join(args.outdir, name.replace("/", "_") + ".png")
    fig.savefig(out, bbox_inches="tight")
    plt.close(fig)
    print(f"[myf] {out}")
