% Write a SDIF file.
%
% To open a SDIF file in Write Mode:
% Usage: writesdif(filename)
%        writesdif(filename,sdif types definition file)
%        writesdif(filename,file of types definition, number of parameters)
%
% The number of parameters is need for use with jmax sdifplayer. It is
%        written in Information Value Table
%
% To write value in SDIF file:
% Usage: writesdif(time,streamId,FrameSignature, MatrixSignature, MatrixData)
%        time and streamId are float.
%        Frame and Matrix signature are string.
%
% To close SDIF file:
% Usage: writesdif('close')
%

%
% $Id: writesdif.m,v 1.3 2000-05-15 13:11:59 tisseran Exp $
%
% writesdif.m	12. May 2000	Patrice Tisserand
%
% $Log: not supported by cvs2svn $
% Revision 1.2  2000/05/15  13:07:49  tisseran
% Added test for input arguments:
%    Is Matrix Signature a char?
% Removed debugging message (mexPrintf("Matrix Loop") ...)
% Removed unused variable
% Change comment style for writesdif.m
%
% Revision 1.1  2000/05/12  16:14:15  tisseran
% Mexfile to write sdif files in matlab.
% TODO: add possibility to use several file at same time.
%       add test on arguments
%
%
% Matlab mex extension and support functions to write SDIF files.
