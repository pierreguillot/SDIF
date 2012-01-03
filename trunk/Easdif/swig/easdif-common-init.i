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

// create signatures
%inline %{

unsigned int CreateSignature(char p1,char p2,char p3,char p4) { 
return (((((unsigned int)(p1))&0xff)<<24)|((((unsigned int)(p2))&0xff)<<16)|((((unsigned int)(p3))&0xff)<<8)|(((unsigned int)(p4))&0xff));
}
%}


// provide access to std::vector as eaSDIF.Vector and eaSDIF.Selection
namespace std {
   %template(Vector) vector<double>;
   %template(FVector) vector<float>;
   %template(IVector) vector<int>;
   %template(Selection) vector<unsigned int>;
};


%extend Easdif::SDIFEntity {
    void RestrictFrameSelection(const std::vector<unsigned int>& set) {
        Easdif::SelectionSet<SdifSignature> tmp;        
        tmp.insert(set.begin(),set.end());     
        $self->RestrictFrameSelection(tmp);      
    }       
    void RestrictStreamSelection(const std::vector<unsigned int>& set) {
        Easdif::SelectionSet<unsigned int> tmp;        
        tmp.insert(set.begin(),set.end());     
        $self->RestrictStreamSelection(tmp);      
    }       
    void RestrictMatrixSelection(const std::vector<unsigned int>& set) {
        Easdif::SelectionSet<SdifSignature> tmp;        
        tmp.insert(set.begin(),set.end());     
        $self->RestrictMatrixSelection(tmp);      
    }       
};
