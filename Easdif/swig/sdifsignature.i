// $Id: sdifsignature.i,v 1.2 2003-04-18 14:30:10 schwarz Exp $ -*-c-*-
//
// sdifsignature.i		17.04.2003		Diemo Schwarz
//
// typemaps for SWIG to map SdifSignature to strings and back
//
// $Log: not supported by cvs2svn $
// Revision 1.1  2003/04/17 12:19:56  schwarz
// Perl output mapping for SdifSignature works
// (by comparison with _wrap_SdifSignatureToString).


%typemap(perl5, in) SdifSignature 
{
    // I have no idea what PL_na is, 
    // but it works like in _wrap_SdifStringToSignature
    $1 = SdifStringToSignature(SvPV($input, PL_na));
}

%typemap(perl5, out) SdifSignature 
{
    $result = sv_newmortal();
    sv_setpvn($result, SdifSignatureToString($1), 4);

    // tell swig there's an output parameter
    argvi++;
}
