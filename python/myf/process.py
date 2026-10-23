"""Process: the python description of an analysis path. It only collects configuration;
the event loop runs in C++ (myf._myf.Looper)."""
import json

from . import _myf


class Process:
    def __init__(self, name):
        self.name = name
        self.modules = []  # in execution order

    def add(self, type, label, **params):
        if type not in _myf.available_modules():
            raise ValueError(f"[myf] unknown module type '{type}', see myf.available_modules()")
        if any(m["label"] == label for m in self.modules):
            raise ValueError(f"[myf] module label '{label}' used twice")
        self.modules.append({"type": type, "label": label, "params": params})
        return self

    def to_json(self):
        return json.dumps({"process": self.name, "modules": self.modules}, indent=1)

    def run(self, sample, output, max_events=-1):
        looper = _myf.Looper(sample, output)
        for m in self.modules:
            looper.add(m["type"], m["label"], m["params"])
        looper.set_config(self.to_json())
        looper.run(max_events)
