#!/usr/bin/perl


use XML::Node;

my $version    = '';
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
$xml->register (">sdif-tdl", start => \&header);
$xml->register (">sdif-tdl:types-version", attr => \$version);
$xml->register (">sdif-tdl", end   => sub { print "}\n"; } );

# column handlers
$xml->register ("column:name", attr => sub { push @columns, $_[1] });

# matrix handlers, free or in frame
$xml->register ("matrix:signature", attr => \$matrixsig);
$xml->register ("matrix:name",      attr => \$matrixname);
$xml->register ("matrix",	    end  => \&matrix);

# matrix in frame
$xml->register (">sdif-tdl>frame>matrix:signature",    attr  => \&addmatrix);
$xml->register (">sdif-tdl>frame>matrixref:signature", attr  => \&addmatrix);
# todo: heed roles

# frame handlers
$xml->register (">sdif-tdl>frame:signature", attr => \$framesig);
$xml->register (">sdif-tdl>frame",	     end  => \&frame);


# process all input files
for (@ARGV)
{
    $xml->parsefile ($_);
}

# end of main



sub header
{
    print "SDIF\n\n";
    $version  &&  print "1NVT\n{\n  SdifTypesVersion $version;\n}\n\n";
    print "1TYP\n{\n";
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
