#!/usr/bin/perl

# $Id: test.pl,v 1.10 2003-04-29 16:28:11 schwarz Exp $
#
# $Log: not supported by cvs2svn $
# Revision 1.9  2003/04/18 15:41:18  schwarz
# Don't include all definitions from sdif.h, only the ones needed by
# easdif, defined in sdifdefine.i.  -> smaller wrapper size:
# .cxx	 880k -> 130k
# .o	1000k -> 210k
# .pm	 123k -> 8k
#
# Revision 1.8  2003/04/18 14:31:27  schwarz
# Added typemap for string to SdifSignature conversion (input arg):
# Setting/writing data works --> wrapper complete, but rudimentary.
#
# Revision 1.7  2003/04/17 14:45:28  schwarz
# No need to qualify methods.
# Now signatures are ONLY strings (i.e. MUST NOT use SdifSignatureToString!)
#
# Revision 1.6  2003/04/17 12:20:39  schwarz
# Output (return value) mapping for SdifSignature works!
#
# Revision 1.5  2003/04/17 11:15:01  schwarz
# Swigging sdif.h works (with some warnings), but that's not yet it...
#
# Revision 1.4  2003/04/17 11:03:52  schwarz
# Swigged all easdif classes!
# Now going on to map sdif basic types.
#
# Revision 1.3  2003/04/17 10:19:51  schwarz
# Nice class and method names using renaming.
#
# Revision 1.2  2003/04/17 10:05:48  schwarz
# The same with shadow classes (= proper memory management and methods).
#
# Revision 1.1  2003/04/17 09:51:10  schwarz
# First working swig generated perl extension!
# Classes re-declared in easdif.i, simple method without shadow classes.
# Doesn't do much except counting frames.


use eaSDIF;

$file = new eaSDIF::Entity;
print "created new SDIFEntity $file\n";


$frame = new eaSDIF::Frame;
print "created new SDIFFrame $frame\n";

#with shadow class: 
$res = $file->OpenRead("../test/lic.sdif")  ||  die;
print "open (mode $eaSDIF::eReadFile)...$res\n";

%count = ();

while (!$file->eof())
{
    $res  = $file->ReadNextFrame($frame);
    $fsig = $frame->GetSignature();
    $count{$fsig}++;

    # print frame to stdout
    #$frame->Print();

    $mat  = $frame->GetMatrix(0);
#    $nomat= $frame->GetMatrix(1);
    $msig = $mat->GetSignature();
    $nrow = $mat->GetNbRows();
    $ncol = $mat->GetNbCols();
    $val  = $mat->GetDouble(0, 0);

    $sig  = "$fsig/$msig";
    $valfsig{$sig}  = $fsig;
    $valmsig{$sig}  = $msig;
    $valcount{$sig}++;
    $valsum{$sig}  += $val;
    $valssum{$sig} += $val * $val;

    print "frame $fsig matrix $msig($nrow, $ncol) = $val\n";
}

print "\ntypes defined in file:\n";
$file->PrintTypes();

# close file
$file->Close();



$out = new eaSDIF::Entity();
$ofr = new eaSDIF::Frame();
$omt = new eaSDIF::Matrix();	# create matrix with same type

$out->OpenWrite("mean.sdif")  ||  die;
print "writing file 'mean.sdif'...\n";

while (my ($sig, $c) = each %valcount)
{
    my $mu    = $valsum{$sig} / $c;
    my $sigma = sqrt($valssum{$sig} / $c - $mu * $mu);
    printf "found %d %s matrices, mean = %g, sigma = %g\n",
	   $c, $sig, $mu, $sigma;

    # write one matrix with mean at time sigma per type found
    $omt->CreateMatrixData($valmsig{$sig}, 1, 1, $eaSDIF::eFloat4);
    $omt->Set(0, 0, $mu);

    $ofr->SetHeader($valfsig{$sig}, $stream++, $sigma);
    $ofr->AddMatrix($omt);
    $bytes += $ofr->Write($out);
}

$out->Close();
print "...wrote $bytes bytes\n";
