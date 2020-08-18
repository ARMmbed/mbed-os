This folder contains selected FNET services and nanostack adaptation layer.

Selected FNET services are ported to work in nanostack environment, for example mDNS responder. 
FNET TCP/IP stack is not copied here.

FNET code is located under fnet directory. When FNET file was modified then ARM copyright has been added. The fnet 
directory does not contain all FNET files, only needed files are copied from the FNET source repository.

Files in the top level are glue logic that are needed to run FNET services in nanostack environment. FNET services 
will be used by using API's specified in this directory. Usage of FNET services directly from nanostack is not allowed.

NOTE!
Please do not edit these files locally here, a master copy of these files can be found from https://github.com/ARMmbed/nanomesh-fnet-private.
A master copy is needed to make upgrades easier in the future.


