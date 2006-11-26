#!/usr/bin/env python
import math
import string
import eaSDIF
import sys 

if len(sys.argv) < 2 :
    sys.stderr.write( "please provide input and output file name!")
    sys.exit()

file = eaSDIF.Entity()
fileout = eaSDIF.Entity()
frame = eaSDIF.Frame();
infile = sys.argv[1]
outfile = sys.argv[2]

#with shadow class: 
res = file.OpenRead(infile)
sys.stderr.write( " file.OpenReads(\"%s\")... %d\n" % (infile, res))

#with shadow class: 
res = fileout.OpenWrite(outfile)
sys.stderr.write( " file.OpenWrite(\"%s\") ... %d\n" % (outfile ,res))


frameout = eaSDIF.Frame();
matrixout = eaSDIF.Matrix();

# write one matrix with mean at time sigma per type found
matrixout.Init("1TRC", 1, 4, eaSDIF.eFloat4)
frameout.SetHeader("1TRC", 0, 0)



count = {}

rowind = 0
bytes = 0
row=eaSDIF.vectord();
rowtrc=eaSDIF.vectord();

while not file.eof():
    res  = file.ReadNextFrame(frame);
    fsig = frame.GetSignature();
    # need typemap SdifSignature -> string 
    #    count[fsig] += 1;

    # print frame to stdout
    #$frame->View();

    mat  = frame.GetMatrix(0)
    mat.GetRow(row,rowind)
    sys.stderr.write( "fsig %s row %d -> %f \n" % (fsig, rowind , row[0]))
    rowtrc = [0,row[0],row[0],0 ]
    matrixout.SetRow(rowtrc,0)
    frameout.AddMatrix(matrixout)
    frameout.SetTime(frame.GetTime())
    bytes += frameout.Write(fileout)
    frameout.Resize(0)
    

# close file
file.Close();
fileout.Close();

sys.stderr.write( "...wrote %d bytes \n" % bytes);
