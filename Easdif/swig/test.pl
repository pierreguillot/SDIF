#!/usr/bin/perl

# $Id: test.pl,v 1.3 2003-04-17 10:19:51 schwarz Exp $
#
# $Log: not supported by cvs2svn $
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
$res = $file->SDIF::Entity::OpenRead("../test/lic.sdif");
print "open...$res\n";

while (!$file->SDIF::Entity::eof())
{
    $file->SDIF::Entity::ReadNextFrame($frame);
    $count++;
}

print "found $count frames\n";
