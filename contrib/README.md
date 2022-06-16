Wallet Tools
---------------------

### [BitRPC](/contrib/bitrpc) ###
Allows for sending of all standard Bitcoin commands via RPC rather than as command line args.

Repository Tools
---------------------

### [Developer tools](/contrib/devtools) ###
Specific tools for developers working on this repository.
Contains the script `github-merge.sh` for merging github pull requests securely and signing them using GPG.

### [Linearize](/contrib/linearize) ###
Construct a linear, no-fork, best version of the blockchain.

### [Qos](/contrib/qos) ###

A Linux bash script that will set up traffic control (tc) to limit the outgoing bandwidth for connections to the PCOIN network. This means one can have an always-on pcoind instance running, and another local pcoind/pcoin-qt instance which connects to this node and receives blocks from it.

### [Seeds](/contrib/seeds) ###
Utility to generate the pnSeed[] array that is compiled into the client.

Build Tools and Keys
---------------------

### [Gitian-descriptors](/contrib/gitian-descriptors) ###
Gavin's notes on getting gitian builds up and running using KVM.

### [Gitian-downloader](/contrib/gitian-downloader)
Various PGP files of core developers. 

### [MacDeploy](/contrib/macdeploy) ###
Scripts and notes for Mac builds. 

Test and Verify Tools 
---------------------

### [TestGen](/contrib/testgen) ###
Utilities to generate test vectors for the data-driven Bitcoin tests.
