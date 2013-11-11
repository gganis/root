# Tutorials #

In this section we dissect the turials available under $ROOTSYS/tutorials/proof .

## Simple ##
    Machine that

## Event ##
    MCHINE THAT

## Pythia8 ##

This tutorial shows how we can use PROOF to simulate physics events with the PYTHIA8 generator.

### Installation of Pythia8 ###
The first thing we need is to install Pythia8; this step can be skipped if Pythia8 is already
available on the machine(s). Keep anyhow track of the installation paths. For the installation,
make sure that you have HepMC installed (availabel in every major linux distribution) and then
download the gzipped tarball from the Pythia web site:


    $ mkdir -p local/pythia8/par  
    $ cd local/pythia8/par  
    $ wget http://home.thep.lu.se/~torbjorn/pythia8/pythia8180.tgz  
    $ cd ..  
    $ tar xzf tar/pythia8180.tgz  
    $ cd pythia8180  
    $ ./configure --installdir=/opt/pythia8 --enable-shared --enable-64bits  
    $ make  
    $ sudo make install  

This installes under `/opt/pythia8`.

MAchine that ...

