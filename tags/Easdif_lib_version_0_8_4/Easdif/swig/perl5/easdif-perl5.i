// generate perl module named 'eaSDIF'
%module eaSDIF


// module initialisation code
%init 
%{
    EasdifInit();
%}

// include typemap
%include sdiftypemap-perl5.i

// define only the needed basic SDIF stuff from sdif.h
%include ../easdif-defines.i

// include common module directive
%include ../easdif-common.i
