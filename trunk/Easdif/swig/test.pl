#!/usr/bin/perl

# $Id: test.pl,v 1.5 2003-04-17 11:15:01 schwarz Exp $
#
# $Log: not supported by cvs2svn $
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
use Data::Dumper;

$file = new SDIF::Entity;
print "created new SDIFEntity $file\n";


$frame = new SDIF::Frame;
print "created new SDIFFrame $frame\n";

#with shadow class: 
$res = $file->SDIF::Entity::OpenRead("../test/lic.sdif");
print "open...$res\n";

%count = ();

while (!$file->SDIF::Entity::eof())
{
    $res = $file->SDIF::Entity::ReadNextFrame($frame);
    $count{$frame->GetSignature}++;

    # print frame to stdout
    $frame->View();

    #print "reading frame $count: $res\n";
}

print $file->ViewTypes(), " types\n";

while (my ($ft, $c) = each %count)
{
    print "found $c frames of type ", SDIF::SdifSignatureToString($ft), "\n";
}
