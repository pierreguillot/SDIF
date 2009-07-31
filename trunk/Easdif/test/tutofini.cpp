/*
 * This file is part of the IRCAM EASDIF Library (http://www.ircam.fr/sdif)
 *
 * Copyright (C) 2002-2003 by IRCAM-Centre Georges Pompidou, Paris, France.
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * See file COPYING for further informations on licensing terms.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 *
 * The EASDIF library provides an Easy Api for IRCAMs SDIF library. 
 *
 */
#include "easdif/easdif.h"

using namespace Easdif;

int main(int argc, char** argv)
{  

    /* initialise the SDIF library and install exceptions*/
    EasdifInit();
    Easdif::SDIFEntity* p = new Easdif::SDIFEntity();
    if (!p->OpenRead(argv[1]) ) {
      std::cerr << "Could not open input file :"<<argv[1]<<std::endl;
      exit(1);
    }
    
    p->EnableFrameDir();
    int numNVTs =  p->GetNbNVT();
    
    int sumNVTEntries = 0;
    for(int ii=0;ii!=numNVTs;++ii){      
      sumNVTEntries += p->GetNVT(ii).size();
    }  

    Easdif::SDIFEntity::const_iterator it = p->begin();
    Easdif::SDIFEntity::const_iterator ite = p->end();
    // establish directory
    int ind = 0;
    while(it !=ite) {
      if(ind % 1000 == 0)
        fprintf(stderr, "ind %d  \n", ind);        
      ++it;
      ++ind;
    }
    p->Rewind();
    const Easdif::Directory & dir= p->GetDirectory();
    int dirsize = dir.size();
    fprintf(stderr, "directory size %d \n", dir.size() );
    it      = p->begin();
    for(int ii=0; ii != dirsize;  ++it,  ++ii) {
      if(ii % 1000 == 0)
        fprintf(stderr, "ii %d directory size %d \n", ii, dirsize );
        
    }
    
    delete p;
    /* deinitialise the SDIF library */
    //EasdifEnd();
    return 0;
}

