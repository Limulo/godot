## README

This module is intended to use libpd as audio engine within godot.

A copy of libpd (`libpd_wrapper`, `libpd_wrapper/util`, `pure-data/src`, `pure-data/extra`) can be found inside **thirdparty** folder.

The **SConstruct** file has been modified (a reference to builtin libpd has been added).

### TODO

* add puredata and libpd submodules to git repository;
* maybe it is necessary to have multiple PDPlayer in order to load multiple patches. This we will be able to load individual synth for each particular sound.
* Maybe it will be necessary to make a 3DPDPlayer (which inherits from an emitter) in order for the sound coming from the patch to be subject to Doppler, occlusion and other spatialisation effects.
* singleton issue: PDPlayer should be a singleton(?);
* How can a user load a patch from GUI editor? Build an import plugin: patches as resources;
* add extra references to SCsub in order to compile extra objects (`bob~`, `fiddle~`, `sigmund~`, ...);
* error handling, flow control, safely unload pd patch;
* add export template
