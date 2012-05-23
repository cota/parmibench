ParMiBench
==========

Code taken from the original at:

  http://code.google.com/p/multiprocessor-benchmark/

I have only changed the makefiles to support relatively painless
cross-compilation.

Known Issues (patches wanted!)
--------------------------------------
+ Lots of style issues (e.g. trailing white space)
+ bitcount is broken: it segfaults.
+ Inconsistent file formats: they should all be in unix format (LF line breaks)
+ Code raises warnings in modern gcc's
