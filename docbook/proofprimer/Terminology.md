## Terminology ##

**Client**  
The *client* is the machine running the ROOT session opening the connection to the master
or to the workers (in case of PROOF-Lite). We call *client* also the client ROOT sessions itself.

**Master**  
The *master* is the machine running a ROOT application coordinating the work between the workers
and merging the results. We call *master* also the master ROOT application itself.

**Worker or Slave**  
The *worker* (or *slave*) is any machine running a ROOT application doing the actual work.
We call *worker* (or *slave*) also the worker ROOT application itself.

**PROOF session**  
A *PROOF session* is a set of {client, master, workers} started successfully by a call to `TProof::Open()`.

**Query**  
A *query* is a process request submitted by the client to the master. It consists of a selector 
and possibly by a dataset (chain).

**Package, PAR file**  
A *package* or *PAR file* is additional code needed by the selector, not available on the PROOF cluster nodes,
loaded as a separate library. It is given in the form of a gzipped tarball containing all what needed
to build and enable the package.

**Selector**  
A *selector* is a class derivign from `TSelector` and providing the code to be executed.

**Dataset**  
A *dataset* is the meta-information for the files containing the `TTree` to be processed. It can be
a `TChain`, a `TFileCollection`, a `TDset`. It can also be the name of a `TFileCollection` stored
on the master.

* * * *



Now *let's dive into PROOF!*

