#!/usr/bin/perl

# $Id: test.pl,v 1.1 2003-04-17 09:51:10 schwarz Exp $
#
# $Log: not supported by cvs2svn $

use SDIF;

$file = SDIF::new_SDIFEntity;
print "created new SDIFEntity $file\n";

$frame = SDIF::new_SDIFFrame;
print "created new SDIFFrame $frame\n";

#with shadow class: 
#$res = $file->SDIF::OpenRead("x");
$res = SDIF::SDIFEntity_OpenRead($file, "../test/lic.sdif");
print "open...$res\n";

while (!SDIF::SDIFEntity_eof($file))
{
    SDIF::SDIFEntity_ReadNextFrame($file, $frame);
      $count++;
}

print "found $count frames\n";
