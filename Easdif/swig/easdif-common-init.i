%include typemaps.i

// generate perl module named 'eaSDIF'
%module eaSDIF

// module initialisation code
%init 
%{
    EasdifInit();
%}

// include typemap for std::string from SWIG library
%include std_string.i

// include typemap for std::string from SWIG library
%include std_vector.i

// provide access to std::vector as eaSDIF.vector
namespace std {
   %template(Vector) vector<double>;
};
