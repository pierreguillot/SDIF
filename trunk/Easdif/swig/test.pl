#!/usr/bin/perl

# $Id: test.pl,v 1.8 2003-04-18 14:31:27 schwarz Exp $
#
# $Log: not supported by cvs2svn $
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


use SDIF;

$file = new SDIF::Entity;
print "created new SDIFEntity $file\n";


$frame = new SDIF::Frame;
print "created new SDIFFrame $frame\n";

#with shadow class: 
$res = $file->OpenRead("../test/lic.sdif");
print "open...$res\n";

%count = ();

while (!$file->eof())
{
    $res  = $file->ReadNextFrame($frame);
    $fsig = $frame->GetSignature();
    $count{$fsig}++;

    # print frame to stdout
    #$frame->View();

    $mat  = $frame->GetMatrix(0);
    $nomat= $frame->GetMatrix(1);
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
$file->ViewTypes();

# close file
$file->Close();



$out = new SDIF::Entity();
$ofr = new SDIF::Frame();
$omt = new SDIF::Matrix();	# create matrix with same type

$out->OpenWrite("mean.sdif");

while (my ($sig, $c) = each %valcount)
{
    my $mu    = $valsum{$sig} / $c;
    my $sigma = sqrt($valssum{$sig} / $c - $mu * $mu);
    print "found $c $sig matrices, mean = $mu, sigma = $sigma\n";

    $omt->CreateMatrixData($valmsig{$sig}, 1, 1, 0x0008);
    $omt->Set(0, 0, $mu);

    $ofr->SetInfo($valfsig{$sig}, $stream++, $sigma);
    $ofr->AddMatrix($omt);
    $ofr->Write($out);
}

$out->Close();
