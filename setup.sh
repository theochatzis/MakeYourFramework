# source setup.sh  -- environment for MakeYourFramework
# On lxplus (or any machine with /cvmfs) it uses an LCG view; elsewhere it uses what you installed
# (e.g. `conda env create -f environment.yml && conda activate myf`).
MYF_LCG_VIEW=/cvmfs/sft.cern.ch/lcg/views/LCG_108/x86_64-el9-gcc13-opt
if [ -z "$CONDA_PREFIX" ] && [ -f "$MYF_LCG_VIEW/setup.sh" ]; then
  source "$MYF_LCG_VIEW/setup.sh"
fi
export MYF_BASE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
export PYTHONPATH="$MYF_BASE/python:$PYTHONPATH"
export PATH="$MYF_BASE/scripts:$PATH"
if ! command -v root-config > /dev/null; then
  echo "[myf] ROOT not found: use lxplus, or install the conda environment (environment.yml)"
else
  echo "[myf] ready. base=$MYF_BASE root=$(root-config --version)"
fi
