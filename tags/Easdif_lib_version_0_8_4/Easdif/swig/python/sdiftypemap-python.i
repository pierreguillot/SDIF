// $Id: sdiftypemap-python.i,v 1.2 2003-05-18 21:10:55 roebel Exp $
//
// sdiftypemap-python.i		30.04.2003		Patrice Tisserand
//
// typemaps for SWIG to map SdifSignature to strings and back
//
// $Log: not supported by cvs2svn $
// Revision 1.1  2003/04/30 11:45:48  tisseran
// Added swig python stuff
// Started autoconfiscation of swig
//

// include typemap for std::string from SWIG library
%include std_string.i


//
// typemaps for SWIG to map SdifSignature to strings and back
//
%typemap(python, in) SdifSignature 
{
    if (PyString_Check($input))
    {
	$1 = SdifStringToSignature(PyString_AsString($input));
    }
    else if (PyInt_Check($input))
    {
	$1 = PyInt_AS_LONG($input);
    }
    else  
    {
	PyErr_SetString(PyExc_TypeError, "not a String");
	return NULL;
    }
}

%typemap(python, out) SdifSignature 
{
    $result = PyString_FromString(SdifSignatureToString($1));
}


//
// typemaps for SWIG to map SdifStringT to strings and back
//

// using an SdifStringT as input argument: convert from python string
%typemap(python, in) SdifStringT *
{
    SdifStringT *str = SdifStringNew();		// memory leak...
    SdifStringAppend(str, PyString_AsString($input));
    $1 = str;
}

// using an SdifStringT as return value: convert to python string
%typemap(python, out) SdifStringT *
{ 
    $result = PyString_FromString($1->str);
}

%typemap (python, freearg) SdifStringT *
{
    SdifStringFree($1);
}
