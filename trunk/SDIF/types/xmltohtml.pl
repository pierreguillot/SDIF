#!/usr/bin/perl
#
# $Id: xmltohtml.pl,v 1.4 2000-08-09 14:43:50 schwarz Exp $
#
# xmltohtml.pl		6. July 2000		Diemo Schwarz
#
# Translate SDIF types description in XML to HTML.
#
# $Log: not supported by cvs2svn $
# Revision 1.3  2000/08/08  15:59:09  schwarz
# SDIF-TDL version 0.2 (no matrix:role attribute, types-version, types-revision
# Generation details in STYP 1NVT / HTML footer.
#
# revision 1.2 date: 2000/08/01 09:36:03;  author: schwarz;
# Coloured section headings.
#
# revision 1.1 date: 2000/07/27 13:32:03;  author: schwarz;
# First preliminary trial test version of SDIF types definition using XML:
# xmltostyp.pl generates library-types file sdiftypes.styp from sdiftypes.xml,
# xmltohtml.pl generates documentation file sdiftypes.html from sdiftypes.xml.

# TODO:
# - table for matrix, frames
# - use DOM parser instead of XML::Node
# - generate LaTeX->hevea->html???
# - if not above, then generate table of contents
# - status for matrices


use XML::Node;
use HTML::Stream qw(:funcs);
use English;


my $cvsrev     = '$Id: xmltohtml.pl,v 1.4 2000-08-09 14:43:50 schwarz Exp $ ';
my $tdlversion = '';
my $version    = 'unknown';
my $revision   = '';
my $framesig   = '';
my $framename  = '';
my $framestat  = '';
my $matrixsig  = '';
my $matrixname = '';
my $rownum     = '';
my $rowmin     = '';
my $rowmax     = '';
my $colname    = '';
my $colunit    = '';
my $refsig     = '';
my $refocc     = '';
my $text       = '';	# text for all descriptions and refs
my $lang       = '';
my @columns    = ();
my @components = ();

my %matrixnames = ();
#later my %matrixroles = ();


# init
$h   = new HTML::Stream \*STDOUT;
$xml = new XML::Node;


# html setup
my @copiedhtmltags = qw(i b emph code);
my %tagmap = (# section => 'H2', 
	      map { (uc $_, uc $_, lc $_, uc $_ ) } @copiedhtmltags);
# register XML->HTML tag mapping handlers
for (keys %tagmap)
{
    $xml->register ($_, char => \&tagmapper);
}

# header handler
$xml->register (">sdif-tdl", 		    start => \&header);
$xml->register (">sdif-tdl", 		    end   => \&footer);
$xml->register (">sdif-tdl:version",	    attr  => \$tdlversion);
$xml->register (">sdif-tdl>types-version",  char  => sub { 
    $h->I->t("SDIF Types Version: ")->_I->t($_[1])->P; });
$xml->register (">sdif-tdl>types-revision", char  => \$revision);
$xml->register (">sdif-tdl>types-revision", end   => sub { 
    $h->I->t("SDIF Types CVS Revision: ")->_I->CODE->t($revision)->_CODE->P;});

# row/column handlers
$xml->register ("rows:num",    attr => \$rownum);
$xml->register ("rows:min",    attr => \$rowmin);
$xml->register ("rows:max",    attr => \$rowmax);
$xml->register ("rows",        start=> \&row_start);
$xml->register ("rows",        char => \&out_char);
$xml->register ("rows",        end  => \&row_end);
$xml->register ("column",      start=> \&column_start);
$xml->register ("column",      char => \&out_char);
$xml->register ("column",      end  => \&column_end);
$xml->register ("column:name", attr => \$colname);
$xml->register ("column:unit", attr => \$colunit);

# matrix handlers, free or in frame
$xml->register ("matrix:signature", attr => \$matrixsig);
$xml->register ("matrix:name",      attr => \$matrixname);
$xml->register ("matrix",	    start=> \&matrix_start);
$xml->register ("matrix",	    end  => \&matrix_end);

# matrix in frame
$xml->register (">sdif-tdl>frame>matrixref:occurrence", attr  => \$refocc);
$xml->register (">sdif-tdl>frame>matrixref",            start => \&addmatrix);
$xml->register (">sdif-tdl>frame>matrixref",            char  => \&out_char);
# todo: heed roles

# frame handlers
$xml->register (">sdif-tdl>frame:signature", attr => \$framesig);
$xml->register (">sdif-tdl>frame:name",      attr => \$framename);
$xml->register (">sdif-tdl>frame:status",    attr => \$framestat);
$xml->register (">sdif-tdl>frame",	     start=> \&frame_start);
$xml->register (">sdif-tdl>frame",	     end  => \&frame_end);

# references in text
$xml->register ("matrixref:signature", attr  => \$refsig);
$xml->register ("matrixref",	       char  => \&matrixref_char);
		# no tags in ref text!

$xml->register ("description", start => \&description_start);
$xml->register ("description", char  => \&out_char);
$xml->register ("description", end   => \&description_end);
$xml->register ("description:language", attr => \$lang);
$xml->register ("section",     char  => \&section);


# process all input files
local $arg;
for $arg (@ARGV)
{
    $xml->parsefile ($arg);
}

# end of main




sub tagmapper
{
    my $tag = $tagmap{$xml->{CURRENT_TAG}};
    $h->tag ($tag)      if $tag;
    $h->text($_[1]);
    $h->tag ("_$tag")   if $tag; 
    $h->text("\n");
}


sub out_char
{
    $h->t($_[1]);
}

sub header
{
    $h->HTML->TITLE->t("Standard SDIF Types")->_TITLE;
    $h->BODY(bgcolor => "#ffffff")->H1->t("Standard SDIF Types")->_H1;
}

sub footer
{
    my @g = stat $PROGRAM_NAME;
    my @s = stat $arg;

    $h->BR->BR->BR->ADDRESS->tag('BASEFONT', size => 1)
      ->TABLE(cellpadding => 3)->TR
	  ->TD(colspan => 4, bgcolor => "gray")
	      ->FONT(size => +4)->B->t("About This Document")->_B->_FONT
      ->TR(align => 'left', valign => 'top')
	  ->TD(colspan => 4)
	      ->t("Document generated " . scalar localtime() . 
		  " by $PROGRAM_NAME from @ARGV")
      ->TR(align => 'left', valign => 'top')
	  ->TH->t("Generation")->TD->t($ENV{USER})
	  ->TD(nowrap)->t(scalar localtime())
	  ->TD(nowrap)->t("$ENV{HOST} $ENV{PWD}")->_TR
      ->TR(align => 'left', valign => 'top')
	  ->TH->t("Generator")->TD->t($PROGRAM_NAME)
	  ->TD(nowrap)->t(scalar localtime($g[9]))
	  ->TD(nowrap)->t($cvsrev)->_TR
      ->TR(align => 'left', valign => 'top')
	  ->TH->t("Source file")->TD->t("@ARGV")
	  ->TD(nowrap)->t(scalar localtime($s[9]))
	  ->TD(nowrap)->t($revision)->_TR
      ->_TABLE
      ->_ADDRESS;
    $h->BR->BR->BR->BR->BR->BR->BR->BR->BR->BR->BR->BR->BR->BR->BR->BR->BR;
    $h->_BODY->_HTML;
}


sub row_start
{
    $h->DL->DT->I->t("Rows:");
    $rownum  &&  $h->t(" number=$rownum");
    $h->_I->DD;

    $rownum = '';
};

sub row_end
{
    $h->_DD->_DL;

    push @columns, $colname;
    $colname = '';
    $colunit = '';
};

sub column_start
{
    !$colhead  && $h->DL->DT->I->t("Columns:")->_I->_DL->OL;
    $h->LI->B->t("$colname ")->_B;
    $colunit  &&  $h->I->t("[$colunit] ")->_I;
    $colhead = 1;
};

sub column_end
{
    push @columns, $colname;
    $colname = '';
    $colunit = '';
};

sub matrix_start
{
    $h->BR->A(name => "Matrix_$matrixsig")
      ->H3->t("Matrix $matrixsig $matrixname")->_H3->_A;
}

sub matrix_end
{
    $colhead     &&  $h->_OL;
    $text  &&  $h->DL->DT->I->t("Description:")->_I->DD->t($text)->_DD->_DL;
    $h->HR;
    # todo: text before columns

    $matrixnames{$matrixsig} = $matrixname;
    @columns    = ();
    $matrixsig  = ''; 
    $matrixname = ''; 
    $text = '';
    $colhead = 0;
}


sub matrixref
{
    $h->A(href => "#Matrix_$matrixref")->t("Matrix $matrixsig $matrixname")->_A;
    $matrixref = '';
}

sub frame_start
{
    $h->BR->A(name => "Frame_$framesig")->H3
      ->t("Frame $framesig $framename")->_H3->_A;
    $framestat  &&  $h->DL->DT->I->t("Status:")->_I->DD->t($framestat)->_DD->_DL;
    $framestat = '';
}

sub frame_end
{
    $mathead  &&  $h->_UL->_DL;
    $mathead = 0;
    $h->HR;

    $framesig   = '';
    $framename  = '';
    $text  = '';

}

sub addmatrix
{
    !$mathead  &&  $h->DL->DT->I->t("Matrices:")->_I->DD->UL;
    $h->LI->A(href => "#Matrix_$refsig")->t("$refsig $matrixnames{$refsig}")->_A;
    $h->I->t(" $refocc ")->_I;

    $mathead = 1;
    $refsig = '';
    $refocc = '';
    $text = '';
}

sub matrixref_char
{
    $h->A(HREF=>"#Matrix_$refsig")->t($_[1])->_A;    
    $refsig = '';
}

sub description_start
{
    $h->DL->DT->I->t("Description" . ($lang ? " (language = $lang)" : "") . 
		     ":")->_I->DD;
    $text = '';
    $lang = '';
}

sub description_end
{
    $h->_DD->_DL;
}


sub section
{
    chomp $_[1];
    #simple: $h->H2->t($_[1])->_H2;
    $h->BR->BR->TABLE(bgcolor => 'yellow', cellpadding => 3, width => '100%')->
	TR->TD->FONT(size => +5)->B->t($_[1])->_B->_FONT->_TD->_TR->_TABLE;
}
