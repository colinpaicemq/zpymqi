# Build instructions

This was built using a virtual environment.
It needs  build-wheel. Build-wheel can be installed in a virtual environment using pip3 install wheel         

I used sh cl.sh to invoke setup.py with the correct options specified.

The bind of the object to create a .so module, requires the object library to end in .OBJ.
The xlc compiler expects the oject library to have .obj at the end of the DSN.
I created a library MQM.V920.SCSQDEFS.OBJ and copied MQM.V920.SCSQDEFS(*) 
into it.

The output file is in /u/betacp/pymqi/ccode/build/lib.os390-27.00-2964-3.8/pymqi/zpymqe.so
You need to copy this into a file in PYTHONPATH.

