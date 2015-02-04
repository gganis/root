# PROOF basics #

As mentioned in the introduction, there are two realizations of the PROOF concept in ROOT: the standard one addressing the case
of a multi-node cluster, and PROOF-Lite adapted to multi-core machines. The two realizations share large portions of code-base and
the same user interface. PROOF-Lite requires no additional configuration (wrt to ROOT) and it is a convenient way to familiarise with
the system. This the reason why we will start from there to explain the basics of PROOF. 

## PROOF-Lite session ##

To start a PROOF-Lite session from a ROOT shell just type

    root[] TProof::Open("lite://")

For an 8 cores machine you should get on the screen something like this:

     +++ Starting PROOF-Lite with 8 workers +++
    Opening connections to workers: OK (8 workers)
    Setting up worker servers: OK (8 workers)
    PROOF set to parallel mode (8 workers)
    (class TProof*)0x7f97b3811000

TProof::Open creates a TProof object, a pointer to which is stored into the global gProof

    root [] gProof
    (class TProof*)0x7f97b3811000

On a separate terminal you can check that opening a PROOF session has actually started 8 additional processes running 'proofserv',
the PROOF servers:

    $ ps ax | grep proofserv
    57283 s000  S+     0:00.19 /Users/ganis/local/root/v5-34-00-patches/bin/proofserv.exe proofslave lite 57277 0
    57285 s000  S+     0:00.18 /Users/ganis/local/root/v5-34-00-patches/bin/proofserv.exe proofslave lite 57277 0
    57287 s000  S+     0:00.18 /Users/ganis/local/root/v5-34-00-patches/bin/proofserv.exe proofslave lite 57277 0
    57289 s000  S+     0:00.19 /Users/ganis/local/root/v5-34-00-patches/bin/proofserv.exe proofslave lite 57277 0
    57291 s000  S+     0:00.19 /Users/ganis/local/root/v5-34-00-patches/bin/proofserv.exe proofslave lite 57277 0
    57293 s000  S+     0:00.18 /Users/ganis/local/root/v5-34-00-patches/bin/proofserv.exe proofslave lite 57277 0
    57295 s000  S+     0:00.18 /Users/ganis/local/root/v5-34-00-patches/bin/proofserv.exe proofslave lite 57277 0
    57297 s000  S+     0:00.19 /Users/ganis/local/root/v5-34-00-patches/bin/proofserv.exe proofslave lite 57277 0

These are the worker processes.


### TProof: the PROOF shell ###


### The PROOF sandbox ###

## First processing ##

## The dialog max ##

## The output list ##


## Feedback ##


## Example of TTree processing: the H1 example ##


## Datasets ##

### Datasets in PROOF ###

### A dataset for the H1 sample ###

## Loading additional code ##

### PAR files ###

### PAR example: event.par ###

