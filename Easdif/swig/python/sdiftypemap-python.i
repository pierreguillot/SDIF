// $Id: sdiftypemap-python.i,v 1.4 2006-11-26 16:39:30 roebel Exp $
//
// sdiftypemap-python.i		30.04.2003		Patrice Tisserand
//
// typemaps for SWIG to map SdifSignature to strings and back
//
// $Log: not supported by cvs2svn $
// Revision 1.3  2006/11/26 16:15:08  roebel
// Improved platform support by means of using automake more consistently.
// Added new example script.
// Reorganized to work with swig 1.3.31!
//
// Revision 1.2  2003/05/18 21:10:55  roebel
// Added proper handling of int to typemap.
//
// Revision 1.1  2003/04/30 11:45:48  tisseran
// Added swig python stuff
// Started autoconfiscation of swig
//

// include typemap for std::string from SWIG library
%include std_string.i

// include typemap for std::string from SWIG library
%include std_vector.i

// provide access to std::vector as eaSDIF.vector
namespace std {
   %template(Vector) vector<double>;
};

//
// typemaps for SWIG to map SdifSignature to strings and back
//
%typemap(in) SdifSignature 
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

%typemap(out) SdifSignature 
{
    $result = PyString_FromString(SdifSignatureToString($1));
}


//
// typemaps for SWIG to map SdifStringT to strings and back
//

// using an SdifStringT as input argument: convert from python string
%typemap(in) SdifStringT *
{
    SdifStringT *str = SdifStringNew();		// memory leak...
    SdifStringAppend(str, PyString_AsString($input));
    $1 = str;
}

// using an SdifStringT as return value: convert to python string
%typemap(out) SdifStringT *
{ 
    $result = PyString_FromString($1->str);
}

%typemap (freearg) SdifStringT *
{
    SdifStringFree($1);
}
