# MakeYourFramework (myf)

A small, modular C++ analysis framework steered from python, built step by step in the tutorial
"Build Your Own HEP Analysis Framework". Target: Z(->mumu)+jet on CMS 2016 NanoAOD open data, ending in a
data/MC comparison of the dimuon mass.

```
source setup.sh
mkdir -p build && cd build && cmake .. && make -j8 && cd ..
myf-run configs/zjet_cfg.py --sample SingleMuon_2016H
myf-run configs/zjet_cfg.py --sample DYJetsToLL_M50_amcatnlo --max-events 300000
plot_zmass.py --data output/SingleMuon_2016H.root --mc output/DYJetsToLL_M50_amcatnlo.root
```

| Directory | Content |
|---|---|
| `DataFormats/` | the data model: `Particle`, `Lepton`, `Muon`, `Jet`, `ZCandidate` (header only) |
| `Core/` | `Event`, `Module`, `ParameterSet`, `ModuleFactory`, `NanoReader`, `Bookkeeper`, `Looper` |
| `Modules/` | the analysis steps, one file each, registered with `MYF_REGISTER_MODULE` |
| `bindings/` | the pybind11 module `myf._myf` |
| `python/myf/` | python steering: `Process`, `load_samples` |
| `configs/`, `samples/` | the Z+jet configuration and the sample list |
| `scripts/` | `myf-run`, `plot_zmass.py` |
| `examples/` | `first_loop.C`, the no-framework starting point |
