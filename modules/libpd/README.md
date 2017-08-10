## README

This module is intended to use libpd as audio engine within godot.

A copy of libpd (`libpd_wrapper`, `libpd_wrapper/util`, `pure-data/src`, `pure-data/extra`) can be found inside **thirdparty** folder.

The **SConstruct** file has been modified (a reference to builtin libpd has been added).

### TODO

* add puredata and libpd submodules to git repository;

* singleton issue: PDPlayer should be a singleton;

* How can a user load a patch from GUI editor?

* add extra references to SCsub in order to compile extra objects (`bob~`, `fiddle~`, `sigmund~`, ...);

* error handling, flow control, safely unload pd patch;

* add export template
