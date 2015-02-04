# Tutorials #

In this section we dissect the tutorials available under $ROOTSYS/tutorials/proof .

## Simple ##
    Machine that

## Event ##
    MCHINE THAT

## Pythia8 ##

This tutorial shows how we can use PROOF to simulate physics events with the PYTHIA8 generator
based on main03.cc example in Pythia 8.1. To run this analysis, Pythia8 must be installed on
the client and workers machine and  ROOT must be configured with pythia8 enabled.

### Installation of Pythia8 ###
The first thing we need is to install Pythia8; this step can be skipped if Pythia8 is already
available on the machine(s). Keep anyhow track of the installation paths. For the installation,
make sure that you have HepMC installed (available for installation in every major linux
distribution) and then download the gzipped tarball from the Pythia web site:


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

### Enabling Pythia8 in ROOT ###
For the tutorial we need the ROOT Pythia8 plug-in, `libEGPythia8.so`. To build this plug-in we need
to enable it at configuration

    $ cd $ROOTSYS  
    $ ./configure *other options* --enable-pythia8
    $ make

For non *standard* include and libraries paths for the Pythia8 installation, we need to specify them
with the `--with-pythia8-incdir` and `--with-pythia8-libdir` switches.

### Setting the right environment ###
To run the tutorial we need to set the environment variable `PYTHIA8` to point to the Pythia8
installation directory, i.e.

    $ export PYTHIA8=/opt/pythia8

for the example above. If the file `Index.xml` is not under the default directory `$PYTHIA8/xmldoc`
then the variable PYTHIA8DATA must point to directory containing `Index.xml`.

The tutorial assumes that the location of Pythia8 and of Index.xml on the worker machines is exactly
the same.

### Running the tutorial
To run the tutorial in, for example, PROOF-Lite, just type

    root[] .L tutorials/proof/runProof.C+
    root[] runProof("pythia8", "lite://")

You should get the output shown in Figure [4.1](#f41)

[f41]: figures/pythia8.png "f41"
<a name="f41"></a>

![Graphical output of the pythia8 tutorial.\label{f41}][f41]
