// $Id: sdifsignature.i,v 1.1 2003-04-17 12:19:56 schwarz Exp $ -*-c-*-
//
// sdifsignature.i		17.04.2003		Diemo Schwarz
//
// typemaps for SWIG to map SdifSignature to strings and back
//
// $Log: not supported by cvs2svn $


%typemap(perl5, in) SdifSignature 
{
    
}

%typemap(perl5, out) SdifSignature 
{
    $result = sv_newmortal();
    sv_setpvn($result, SdifSignatureToString($1), 4);

    // tell swig there's an output parameter
    argvi++;
}
