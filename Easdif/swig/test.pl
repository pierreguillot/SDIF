#!/usr/bin/perl

# $Id: test.pl,v 1.7 2003-04-17 14:45:28 schwarz Exp $
#
# $Log: not supported by cvs2svn $
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
    $msig = $mat->GetSignature();
    $nrow = $mat->GetNbRows();
    $ncol = $mat->GetNbCols();
    $val  = $mat->GetDouble(0, 0);
    print "frame $fsig matrix $msig($nrow, $ncol) = $val\n";
}

print "\ntypes defined in file:\n";
$file->ViewTypes();

while (my ($ft, $c) = each %count)
{
    print "found $c frames of type $ft\n";
}
