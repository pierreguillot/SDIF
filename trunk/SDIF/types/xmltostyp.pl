#!/usr/bin/perl
#
# $Id: xmltostyp.pl,v 1.4 2000-09-27 10:12:47 schwarz Exp $
#
# xmltostyp.pl		6. July 2000		Diemo Schwarz
#
# Translate SDIF types description in XML to STYP format for the SDIF-library.
#
# $Log: not supported by cvs2svn $
# Revision 1.3  2000/08/09  14:43:50  schwarz
# Put all SDIF types into XML format.  Lots of descriptions still missing.
# Full "about this document" info.
#
# Revision 1.2  2000/08/08  15:59:10  schwarz
# SDIF-TDL version 0.2 (no matrix:role attr., types-version, types-revision)
# Generation details in STYP 1NVT / HTML footer
#
# revision 1.1 date: 2000/07/27 13:32:03;  author: schwarz;
# First preliminary trial test version of SDIF types definition using XML:
# xmltostyp.pl generates library-types file sdiftypes.styp from sdiftypes.xml,
# xmltohtml.pl generates documentation file sdiftypes.html from sdiftypes.xml.
#
# TODO:
# - use DOM parser instead of XML::Node


use English;
use XML::Node;


my $cvsrev     = '$Id: xmltostyp.pl,v 1.4 2000-09-27 10:12:47 schwarz Exp $ ';
my $tdlversion = '';
my $version    = '';
my $revision   = '';
my $framesig   = '';
my $matrixsig  = '';
my $matrixname = '';
my @columns    = ();
my @components = ();

my %matrixnames = ();
#later my %matrixroles = ();


# init xml
$xml = new XML::Node;


# header handler
$xml->register (">sdif-tdl:version",	    attr  => \$tdlversion);
$xml->register (">sdif-tdl",		    end   => sub { print "}\n"; } );
$xml->register (">sdif-tdl>types-version",  char  => \$version);
$xml->register (">sdif-tdl>types-revision", char  => \$revision);

# column handlers
$xml->register ("column:name", attr => sub { push @columns, $_[1] });

# matrix handlers, free or in frame
$xml->register ("matrix",	    start => \&header);
$xml->register ("matrix:signature", attr  => \$matrixsig);
$xml->register ("matrix:name",      attr  => \$matrixname);
$xml->register ("matrix",	    end   => \&matrix);

# matrix in frame
$xml->register (">sdif-tdl>frame>matrix:signature",    attr  => \&addmatrix);
$xml->register (">sdif-tdl>frame>matrixref:signature", attr  => \&addmatrix);
# todo: heed roles

# frame handlers
$xml->register (">sdif-tdl>frame",	     start => \&header);
$xml->register (">sdif-tdl>frame:signature", attr  => \$framesig);
$xml->register (">sdif-tdl>frame",	     end   => \&frame);

# a little grouping
$xml->register (">sdif-tdl>section",	     start => sub { print "\n"; } );



# process all input files
local $arg;
for $arg (@ARGV)
{
    $xml->parsefile ($arg);
}

# end of main



sub header
{   # write header only once!
    if (defined $version)
    {
	my @g = stat $PROGRAM_NAME;
	my @s = stat $arg;
	my %nvt = (SdifTypesVersion   => $version,
		   GenerationDate     => scalar localtime,
		   GenerationUser     => $ENV{USER},
		   GenerationHost     => $ENV{HOST},
		   GenerationDir      => $ENV{PWD},
		   Generator	      => $PROGRAM_NAME,
		   GeneratorFiledate  => scalar localtime($g[9]),
		   GeneratorRevision  => $cvsrev,
		   SourceFile	      => $arg,
		   SourceFiledate     => scalar localtime($s[9]),
		   SourceFileRevision => $revision);

	print "SDIF\n\n1NVT\n{\n";
	for my $name (sort keys %nvt)
	{
	    # replace reserved chars
	    (my $value = $nvt{$name}) =~ tr(.:;, \n\t)(_); 
	    print "  $name\t$value;\n";
	}
	print "}\n\n1TYP\n{\n";
	undef $version;
    }
}


sub addmatrix
{
    push (@components, $_[1]);
}

sub matrix
{
    print "1MTD $matrixsig { ", join (', ', @columns), " }\n";
    $matrixnames{$matrixsig} = $matrixname;
    @columns    = ();
    $matrixsig  = ''; 
    $matrixname = ''; 
}

sub frame 
{
    print "1FTD $framesig { ", map ("$_ $matrixnames{$_}; ", @components), " }\n";
    @components = ();
    $framesig   = '';
}
