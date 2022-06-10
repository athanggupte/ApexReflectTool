# Apex-Reflect-Tool

Automated reflection data generator for [Apex Game Engine](https://github.com/xdevapps/ApexGameEngine) based on clang LibTooling.
The generated c++ files are intended to be used with Apex Game Engine's Reflection library.

* Currently tested only on windows, Visual Studio 19

## How to Build

First run the bootstrap script to download dependencies at the proper locations.
```
python bootstrap.py
```
This will download LLVM, Clang and fmt at ./llvm, ./clang and ./clang-tools-extra/ApexReflectTool/fmt-8.1.1 respectively.

To generate the Visual Studio projects, run the following commands -
```powershell
cd build
cmake_gen_projects.bat
```

The solution will be generated at `build/LLVM.sln`.
Build the project `ApexReflectTool` found under the *Clang executables* group.

## How to Use

### By providing compilation command in command line
```powershell
ApexReflectTool.exe path/to/header_file.h -- clang++ -xc++ -DAPEX_REFLECT_TOOL -Ipath/to/Reflect.h -c
```

### By providing compilation database in compile_commands.json


### Directory structure

- llvm/
  - llvm files
- clang/
  - clang files
- clang-tools-extra/
  - ApexReflectTool/
    - fmt-8.1.1/
    - ApexReflectTool files

Output files are present in build/{Debug|Release}/bin/ApexReflectTool.exe