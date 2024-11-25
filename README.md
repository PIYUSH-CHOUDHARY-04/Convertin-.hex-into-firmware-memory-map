# Converting-.hex-into-firmware-memory-map
Convert Intel .hex file into .bin file 

Program written in this repository can convert a .hex file which is generally used to program the micro-controllers to a .bin file which is exactly the memory map of the 
firmware which is laid down into the mirco-controllers flash/bootable memory.
The application of this program lies in self-programming/in-application-programming where the micro-controller can itself update its own firmware.


This is a Command-line tool which accepts multiple .hex files via command line by their paths.
Example usage : hex2bin file1.hex file2.hex .... fileN.hex
