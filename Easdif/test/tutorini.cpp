

#include "easdif/easdif.h"


using namespace Easdif;


int main(int argc, char** argv)
{  
/***************************************************************************/

    /* initialise the SDIF library and install exceptions*/
    EasdifInit();

    SDIFEntity readentity;
    SDIFEntity entity;

    SDIFFrame frame;
    SDIFFrame frameTowrite;

    int i = 0;
    int y = 0;


 
    /******** NAME VALUE TABLE ********/

    /* for making NameValueTables to the EntityToWrite */
    SDIFNameValueTable NameValueTable;
    NameValueTable.AddNameValue("name", "value");
    NameValueTable.AddNameValue("the_name","the_value");
    NameValueTable.AddNameValue("Eas","dif");

    SDIFNameValueTable NameValueTable2;
    NameValueTable2.AddNameValue("an", "other");
    NameValueTable2.AddNameValue("name","value");

    /*for adding the Name Value Tables to the EntityToWrite*/
    entity.AddNVT(NameValueTable);
    /*for adding with a StreamID number 3 for example */
    entity.AddNVT(NameValueTable2, 3);

    /*for adding the Name Value Tables of the EntityRead*/
    readentity.OpenRead(argv[1]); //the file of EntityRead must be Open
    y = readentity.GetNbNVT();
    if( y != 0)
    {
	for (i = 0 ; i < y ; i++)
	{
	    entity.AddNVT(readentity.GetNVT(i));
	}	
    }

    /******** DESCRIPTION TYPE ********/

    /*to copy the descriptions type of EntityRead to EntityToWrite*/
    entity.SetTypeString(readentity.GetTypeString());

    /*for adding descriptions types : the description must be done before
     *the opening of the EntityToWrite (they are writing when opening)*/
    /* Add a matrix type must be done before to add a frame type*/
    entity.AddMatrixType("1FQ0", "ColumnToAdd1, ColumnToAdd2");
    entity.AddMatrixType("1NEW", "col1, col2, col3, col_i, col_n");
   
    entity.AddFrameType("1NEW", "1NEW NewMatrix; 1FQ0 New1FQ0");

    /* to open a file for writing*/
    entity.OpenWrite("FileToWrite.sdif");


    /******** SELECTION ********/

    /* if you want to change the selection of the EntityRead that the same 
     * mecanism that the for selecting on file */
    //readentity.ChangeSelection(":1FOB/1FOF");


    /******** READ AND WRITE  ********/
    while (!readentity.eof())
	{
	  readentity.ReadNextFrame(frame);
	  /* reading the next frame of the EntityRead, return the number of
	   * bytes read or 0 if the frame is not selected*/
	  //  i = readentity.ReadNextFrame(frame);
	  
	  /* the next part of the loop is for writing just the selected 
	   * frames. If the current frame is not selected we go to the
	   * next frame */
	  //if (i == 0)
	  //continue;
	  
	  /* for adding new matrix in EntityToWrite */
	  for (unsigned int i=0 ; i < frame.GetNbMatrix() ; i++)
	    {
	      /*take the matrix number "i" and put it in tmpMatrix */
	      SDIFMatrix tmpMatrix = frame.GetMatrix(i);
	      
	      /*add a matrix in the matrix vector of the frame*/
	      //frameTowrite.AddMatrixSelected(readentity.GetFile(), tmpMatrix);
	      frameTowrite.AddMatrix( tmpMatrix);
	      
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
	  
	  /* clean the frames */
	  frameTowrite.ClearData();
	  frame.ClearData();
	}
	//   }
    /* to catch an exception */
/*
    catch(SDIFEof& e)
    {
	std::cerr << " Catch EOF " << std::endl;
	e.ErrorMessage();
    }
*/
    /* deinitialise the SDIF library */
    EasdifEnd();
    return 0;
}
   
