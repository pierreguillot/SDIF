#!/usr/bin/env python
import math
import string
import eaSDIF

file = eaSDIF.Entity()
print "created new SDIFEntity ", file


frame = eaSDIF.Frame();
print "created new SDIFFrame ", frame

#with shadow class: 
res = file.OpenRead("../../test/lic.sdif")
print "open (mode ", eaSDIF.eReadFile , ")...",res

count = {}
valfsig = {}
valmsig = {}
valcount ={}
valsum ={}
valssum ={}
while not file.eof():
    res  = file.ReadNextFrame(frame);
    fsig = frame.GetSignature();
    # need typemap SdifSignature -> string 
    #    count[fsig] += 1;

    # print frame to stdout
    #$frame->View();

    mat  = frame.GetMatrix(0);
    #    $nomat= $frame->GetMatrix(1);
    msig = mat.GetSignature();
    nrow = mat.GetNbRows();
    ncol = mat.GetNbCols();
    val  = mat.GetDouble(0, 0);
    sig  = fsig + "/" + msig;

    valfsig[sig]  = fsig
    valmsig[sig]  = msig;
    if valcount.has_key(sig):
         valcount[sig] += 1
         valsum[sig]   += val
         valssum[sig]  += val * val
    else:
         valcount[sig] = 1
         valsum[sig]   = val
         valssum[sig]  = val * val
         
    print "frame " + fsig + " matrix " + msig + "(" + str(nrow) + ", " + str(ncol) + ") = " + str(val);


print "\ntypes defined in file:\n";
file.PrintTypes();

# close file
file.Close();

out = eaSDIF.Entity();
ofr = eaSDIF.Frame();
omt = eaSDIF.Matrix();	# create matrix with same type
stream=0
bytes = 0
out.OpenWrite("mean.sdif")
print "writing file 'mean.sdif'...\n";


for sig in valcount.keys():
     mu    = valsum[sig] / valcount[sig];
     sigma = math.sqrt(valssum[sig] / valcount[sig] - mu * mu)
     print "found %d %s matrices, mean = %g, sigma = %g\n" % (valcount[sig] , sig, mu, sigma)

     # write one matrix with mean at time sigma per type found
     omt.Init(valmsig[sig], 1, 1, eaSDIF.eFloat4)
     omt.Set(0, 0, mu)

     ofr.SetHeader(valfsig[sig], stream, sigma)
     ofr.AddMatrix(omt)
     bytes += ofr.Write(out)
     ofr.ClearData()

out.Close();
print "...wrote ", bytes," bytes\n";
