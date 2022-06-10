#pragma once
#include <vector>
#include <string_view>
#include <clang/AST/QualTypeNames.h>

namespace util {
	std::vector<std::string_view> Tokenize(const std::string& str, const std::string& del);
	std::string TypeVarName(const std::string& type_name);
	std::string GetFullyQualifiedName(clang::QualType type, const clang::ASTContext& context);
}