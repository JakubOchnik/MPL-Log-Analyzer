# MPL Log Analyzer
A command-line C++ program which can be used for filtering MPL logs (present on ADVA FSP 3000/FSP 150 networking solutions).
## Working
- AND/OR concatenation mode
- Equals and contains conditions (with negations)
- Support for filtering strings, integers and POSIX time
## To do in the future:
- Grouping condition modes
- Error handling
- Support for other conditions, such as greater, less etc.
- Sorting algorithms
- Faster I/O (reading the file)
## How to use it?
Simply run the executable using input arguments in a following order:  
```
usage: mplLogAnalyzer [ initial arguments ] [ key ] [ condition ] ...
```
Avaliable initial arguments:
```
-i      Input file
-and    Set condition concatenation mode to AND
-or     Set condition concatenation mode to OR
-h      Display help message
```
Key-condition arguments:
```
-k      Key definition
-(n)e   Condition: (not)equals
-(n)c   Contition: (not)contains
-(n)g   Condition: (not)greater (NOT IMPLEMENTED)
-(n)l   Condition: (not)less (NOT IMPLEMENTED)
-(n)ge  Condition: (not)greater or equal (NOT IMPLEMENTED)
-(n)le  Condition: (not)less or equal (NOT IMPLEMENTED)
```
Available keys:  
```
time, source, type, thd, id, component, message, jsonStr
```
Example:
```
./mplLogAnalyzer -i mpl.log -and -k source -e mpl -k message -nc fp-1\n";
```
## Dependencies
- Boost 1.71
