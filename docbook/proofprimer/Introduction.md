# Motivation and Introduction #

The primary goal of PROOF is to speed-up tree processing by doing it in parallel.
The data of many HEP experiemnts are stored as entries in a TTree structure.
These entries are independent and the order of processing is irrelevant for the final
result, so the paradigm of embarrassing parallelism applies and we can just split the
input sample in many parts, have them analysized by parallel processes and merge the
results of each single process.

There is nothing much new so far: splitting and merge has been the way to speed-up
analysis, for example on LSF, already at LEP times. What is different in the PROOF 
approach is the splitting technique, not based anymore on a push architecture (split
a priori in usually equal parts), but on a pull architecture, where the splitting is
based on the effective performance of the actors. This requires the introduction of a
new component, the master, in interactive control of the workers.

PROOF realizes the splitting and merge paradigm using a 3-tier architecture: client,
master and workers (see Figure [1.1](#f11)). 

[f11]: figures/arch_proof.png "f11"
<a name="f11"></a>

![PROOF architecture.\label{f11}][f11]

The second tier, the master, can be split in additional tiers to connect geographically
separated PROOF clusters or to reduce the control work on a single master for large number
of workers, improving overall scalability.

The architecture shown in Figure [1.1](#f11) reflects the fact that a PROOF-enabled facility
is usually distributed over many physically separated nodes, which can be physically distant
from the client or user machine. The PROOF processes are then network connected and a network
service provided a dedicated daemon - called _xproofd_ - insures that the relevant connections
are established.

While this architecture can be adapted to all cases, for single multi-core machines, e.g.
desktops or laptops, network-connecting the processes is not strictly necessary. Processes
can communicate efficiently using, for example, _unix_ sockets. Furthermore, the client tasks
can be done directly by the client, removing some a layer of data transfers. The resulting
2-tier architecture is shown in Figure [1.2](#f12)

[f12]: figures/arch_lite.png "f12"
<a name="f12"></a>

![PROOF-Lite architecture.\label{f12}][f12]

and is implemented in the version of PROOF optimized for multi-core desktops or laptops referred
to as PROOF-Lite.

## Welcome to PROOF ##

The purpose of
this document is to serve as a beginners guide and provides extendable
examples for your own use cases.
This guide will hopefully lay the ground for more complex
applications in your future scientific work building on a modern,
state-of the art tool for data analysis.

This guide in form of a tutorial is intended to introduce you to the
ROOT package in a few handfuls of pages. This goal will be accomplished using
concrete examples, according to the "learning by doing" principle.
Being PROOF a ROOT application, many basic concepts are the same as in ROOT.
The ROOT Users Guide [@ROOT_Users_Guide] and navigate through the Class
Reference [@ROOT_Class_Reference] are certainly good refernces to have at hand
when doing this tutorial.

PROOF, like ROOT, is written in and heavily relys
on the programming language `C++`, and therefore some knowledge about
`C` and `C++` is required. Eventually, just profit from the immense
available literature about `C++` if you do not have any idea of what
object oriented programming could be.

PROOF is available for Unix platforms (Linux, Mac OS X, ...), but
in this guide we will implicitly assume that you are using Linux.
PROOF is part of ROOT, so the first thing you need to do with ROOT is install
it, don't you?
Obtaining the latest ROOT version is straightforward. Just seek the "Pro" version
on this webpage <http://root.cern.ch/drupal/content/downloading-root>.
You will find precompiled versions for the different architectures, or
the ROOT source code to compile yourself. Just pick up the flavour you
need and follow the installation instructions.

**Let's dive into PROOF!**
