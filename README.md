# Assembler
Final Project for CMPS 361: Software Design.
End of Fall 2016 Semester

## PASS I  ::   12/08/2016
Able to parse through a text file to determine whether a line contains a label, machine language or assembly language code, operand, and/or a comment.

## PASS II :: 12/12/2016
Translates the text lines into location, and operation code and operand location. Upon success it will begin the emulation process, PassII is responsible for handling any errors in the code itself.

### Disclaimer :: Pass II
Error-checking code has not been extensively tested, therefore not all bugs have been found / are known.

## Emulator :: 12/13/2013
Working on PassII branch for emulation.

### Disclaimer :: Emulator
* In order for the emulation to work, it assumes there is an ORG instruction as the first memory location, to find the starting location of where to begin reading from the VC 3600 memory.
* Accepts negative integer as input *without* spacing between the hyphen and the numerical value : -59 _not_ - 59
* Assumptions :
⋅⋅* Any value following a DC/DS Assembly Lanugae operation, is assumed to be a positive value. There is checking for digit values though.
⋅⋅* NO labels accepted on HALT commands.
