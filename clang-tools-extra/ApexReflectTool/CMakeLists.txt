add_subdirectory(fmt-8.1.1)

set( LLVM_LINK_COMPONENTS
  ${LLVM_TARGETS_TO_BUILD}
  Option
  Support
  )

add_clang_executable(ApexReflectTool
  ApexReflectTool.cpp
  ClassFinder.cpp
  Generator.cpp
  ReflectionData.cpp
  TypeInfoGenerator.cpp
  Utils.cpp
  )

clang_target_link_libraries(ApexReflectTool
  PRIVATE
  clangAST
  clangBasic
  clangDriver
  clangFrontend
  clangRewriteFrontend
  clangSerialization
  clangStaticAnalyzerFrontend
  clangTooling
  clangToolingSyntax
  clangLex
  )

target_compile_features(ApexReflectTool PRIVATE cxx_std_17)

target_link_libraries(ApexReflectTool
 PRIVATE
 fmt::fmt
 )