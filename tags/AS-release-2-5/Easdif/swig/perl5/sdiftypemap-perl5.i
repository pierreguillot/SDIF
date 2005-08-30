// $Id: sdiftypemap-perl5.i,v 1.1 2003-04-30 11:31:35 tisseran Exp $ -*-c-*-
//
// sdiftypemap.i		17.04.2003		Diemo Schwarz
//
// typemaps for SWIG to map SdifSignature to strings and back
//
// $Log: not supported by cvs2svn $
// Revision 1.1  2003/04/18 17:45:23  schwarz
// Renamed sdifsignature.i to sdiftypemap.i, because all types are in there.
// Include typemap for std::string from SWIG library
// Added typemap freearg SdifStringT *
//
// Revision 1.2  2003/04/18 14:30:10  schwarz
// Added typemap for string to SdifSignature conversion (input arg).
//
// Revision 1.1  2003/04/17 12:19:56  schwarz
// Perl output mapping for SdifSignature works
// (by comparison with _wrap_SdifSignatureToString).


// include typemap for std::string from SWIG library
%include std_string.i


//
// typemaps for SWIG to map SdifSignature to strings and back
//

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


//
// typemaps for SWIG to map SdifStringT to strings and back
//

// using an SdifStringT as input argument: convert from perl string
%typemap(perl5, in) SdifStringT *
{
    SdifStringT *str = SdifStringNew();		// memory leak...
    SdifStringAppend(str, SvPV($input, PL_na));
    $1 = str;
}

// using an SdifStringT as return value: convert to perl string
%typemap(perl5, out) SdifStringT *
{ 
    SdifStringT *str = $1;
    $result = sv_newmortal();
    sv_setpv($result, str->str);

    // tell swig there's an output parameter
    argvi++;
}

%typemap (perl5, freearg) SdifStringT *
{
    SdifStringFree($1);
}
