#include "easdif/easdif.h"

using namespace Easdif;

int main(int argc, char** argv)
{  

    /* initialise the SDIF library and install exceptions*/
    EasdifInit();

    SDIFEntity readentity;
    SDIFEntity entity;
   
    SDIFFrame frame;
    SDIFFrame frameTowrite;


    if(argc <3){
      std::cerr << "usage: rmNVT filin fileout \n";
      exit(1);
    }

    /*for adding the Name Value Tables of the EntityRead, the file of
     *EntityToRead must be Open*/
    readentity.OpenRead(argv[1]);

    SdifStringT *type = readentity.GetTypeString();
    entity.SetTypeString(type);
    /* to open a file for writing */
    entity.OpenWrite(argv[2]);


    /******** READ AND WRITE  ********/
    /* to show the exception : we break the loop with an exception */
    try{
      while (1)	{

	/* reading the next frame of the EntityRead, return the number of
	 * bytes read, return 0 if the frame is not selected and -1 if this is 
	 * the last frame of the file */
	if(!readentity.ReadNextFrame(frame))	    
	  continue;
	
	/* for adding new matrix in EntityToWrite */
	for (unsigned int i=0 ; i < frame.GetNbMatrix() ; i++)
	  {
	    /*take the matrix number "i" and put it in tmpMatrix */
	    SDIFMatrix tmpMatrix = frame.GetMatrix(i);
	    
	    /*add a matrix in the matrix vector of the frame*/
	    frameTowrite.AddMatrix(tmpMatrix);
	    /*an other method to add just the frame selected in file:
	     * frameTowrite.AddMatrixSelected(readentity.GetFile(),
	     * tmpMatrix);*/
	    
	    /* if you want to access to the data : an example, if we want 
	     * to multiply with 2 the last column of a matrix : */
	    double dou;
	    int ncols = tmpMatrix.GetNbCols();
	    for(int i = 0 ; i < tmpMatrix.GetNbRows() ; i++)
	      {
		//dou = tmpMatrix.GetDouble(i, ncols-1);
		/* to get the value in the double dou*/
		tmpMatrix.Get(i, ncols-1,dou);
		/* an other method can be :
		   dou = tmpMatrix.GetDouble(i, ncols-1);
		*/
		tmpMatrix.Set(i, ncols-1,2.*dou);
	      }
	  }
	
	/* to set the header of the new frame with the one of a frame :*/
	frameTowrite.SetInfo(frame.GetSignature(),
			     frame.GetStreamID(),
			     frame.GetTime() );
	
	/* an other method can be :
	   frameTowrite.SetStreamID(frame.GetStreamID());
	   frameTowrite.SetTime(frame.GetTime());
	   frameTowrite.SetSignature(frame.GetSignature());
	*/
	
	/* to write the EntityToWrite */
	frameTowrite.Write(entity);
	
	/* for reading a frame of the EntityToWrite, this make an 
	 * exception because the file is at end*/
	//frame.Read(entity);
	
	/* clean the frames */
	frameTowrite.ClearData();
	frame.ClearData();
      }
    }
    
    /* to catch an exception */
    catch(SDIFEof& e)
      {
	/* if we want an access to the file */
	SdifFileT *sf = e.sdifFile();

	std::cerr << " Catch EOF for file " <<sf->Name << " -- ending program " << std::endl;
	/* to have the error message */
      }
    
    catch(SDIFUnDefined& e)
      {
	std::cerr << " Catch Undefined " << std::endl;	
	e.ErrorMessage();
      }
    
    catch(Easdif::SDIFException&e)
      {
	std::cerr << " Catch other SDIFException " << std::endl;
	e.ErrorMessage();
      }
    catch(std::exception &e)
      {
	std::cerr << " Catch other Exception: " << e.what() <<std::endl;

      }



    /* to open a file for writing */
    entity.Close();
    readentity.Close();
    
    /* deinitialise the SDIF library */
    EasdifEnd();
    return 0;
}

