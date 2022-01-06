This is a text file that has instruction of how to run the code.

First, the simulation can be run in 2 ways:

	./filtro [Int number_of_SiPM]

					or

	./filtro [FILE macro] [Int number_of_SiPM]

If no macro is provided, the simulation will open the GUI.
If no sipm number is provided, the simulation assumes it to be = 1.

The X-ARAPUCA dimensions can be changed in the Filtro_DetectorConstruction.cc. It is important that the number of SiPMs is not bigger than arapuca_z / sipm_z. 

Other important part is the bash .sh file. It can iterate the simulation over any set of parameters.

IMPORTANT! When altering the X-ARAPUCA size, it is important to change it in the primary genarator aswell! 