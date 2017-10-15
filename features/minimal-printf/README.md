# Minimal printf

Prints directly to stdio/UART without using malloc. 
Doesn't support any modifiers. 

Supports: 
* %d: signed long int
* %u: unsigned long int
* %p: void* (e.g. 0x00123456)
* %s: string
* %X: unsigned char printed as hexadecimal number (e.g., FF)
