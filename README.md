# KMCGUI
## How to run it? (debug version)
  1. Install Qt on your computer (https://tiny.pl/wtn5g) 
  2. Install Qt Visual Studio Tools from Extension Manager in your IDE  (more here https://www.qt.io/blog/qt-vs-tools-for-visual-studio-2022) 
  3. Then open "Extensions" bar -> Qt VS Tools -> Qt Options and define installed Qt version and path to msvc2019_64 folder (You can find it in place where you installed   Qt) 
  4. Clone this repository 
  5. Add "kmc_runner.h" to dependencies in your project properties 
  - In Properties -> C/C++ -> General -> Additional Include Directories type path to Release folder of KMC library (with .lib and .h file)
  - In Properties -> Linker -> General -> Additional Library Directories type path to Release folder of KMC library (with .lib and .h file)
  6. Try to run application 
