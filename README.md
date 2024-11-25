# Converting-.hex-into-firmware-memory-map
Convert Intel .hex file into .bin file 

Program written in this repository can convert a .hex file which is generally used to program the micro-controllers to a .bin file which is exactly the memory map of the 
firmware which is laid down into the mirco-controllers flash/bootable memory.
The application of this program lies in self-programming/in-application-programming where the micro-controller can itself update its own firmware.


This is a Command-line tool which accepts multiple .hex files via command line by their paths.
Example usage : hex2bin file1.hex file2.hex .... fileN.hex

Same program can be used to convert the .hex file for 8/16/32 bit microcontrollers, this functionality is defined into the corresponding header file where user can choose architecture specific macro.
Note that single execution of this program can only perform conversion on same type of files i.e. all files from file1.hex to filen.hex must be .hex files for same architecture.
Functionalities can be extended definitley which is left over to the end user.

So, the end user only need to modify the header file and just compile.
