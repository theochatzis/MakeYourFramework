"""Samples are data, not code: they live in a JSON file and are turned into myf Samples here."""
import json

from . import _myf


def load_samples(path):
    with open(path) as f:
        entries = json.load(f)
    samples = {}
    for name, entry in entries.items():
        sample = _myf.Sample()
        sample.name = name
        sample.isData = entry["isData"]
        sample.xsec = entry.get("xsec_pb", 0.0)
        sample.files = entry["files"]
        samples[name] = sample
    return samples
