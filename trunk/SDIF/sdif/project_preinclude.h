/* $Id: project_preinclude.h,v 3.1 1999-09-28 13:09:18 schwarz Exp $

   SDIF/sdif/project_preinclude.h	28. Sep 1999	Diemo Schwarz

   This file "project_preinclude.h" is #included by preincluded.h into
   every source file of your project.  You should add here any
   project-specific information, macro definitions and conditionals.
	
   Generic stuff (valid for all projects and all architectures) should
   be added to preincluded.h.  Platform-specific stuff should be added
   to host_architecture.h.
  
   You will usually have one project_preinclude.h file per project,
   and you should therefore make sure that the access paths/include
   search paths are such that the project_preinclude.h file is unique.
  
   In order to write platform-independent code, you should insert any
   #defines in this file rather than adding them to the command line
   in the Makefile. This is because CodeWarrior for Mac OS does not
   use Makefiles and such declarations would therefore get lost.
  
   $Log: not supported by cvs2svn $
   
   Alberto Ricci, 19990212 <- I'm Y2K!
*/


#ifndef __H_PROJECT_PREINCLUDE__
#define __H_PROJECT_PREINCLUDE__


#if HOST_OS_MAC

#define bzero(p,s)	memset(p,0,s)
#define bcopy(p1,p2,s)	memcpy(p2,p1,s)

#endif /* HOST_OS_MAC */


#endif /* __H_PROJECT_PREINCLUDE__ */
