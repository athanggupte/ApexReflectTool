#include "Utils.h"
#include <numeric>

namespace util {

	std::vector<std::string_view> Tokenize(const std::string& str, const std::string& del)
	{
		std::vector<std::string_view> tokens;
		size_t start = 0;
		size_t end = str.find(del);
		while (end != -1) {
			tokens.emplace_back(str.c_str() + start, end - start);
			start = end + del.size();
			end = str.find(del, start);
		}
		tokens.emplace_back(str.c_str() + start, str.size() - start);

		return tokens;
	}

	std::string TypeVarName(const std::string& type_name)
	{
		auto tokens = Tokenize(type_name, "::");
		return std::accumulate(tokens.begin(), tokens.end(), std::string(),
			[](std::string& ss, std::string_view& sv) {
				return ss.empty() ? std::string(sv) : ss + "_" + std::string(sv);
			});
	}

	std::string GetFullyQualifiedName(clang::QualType type, const clang::ASTContext& context)
	{
		clang::QualType qualType = clang::TypeName::getFullyQualifiedType(type, context);
		std::string typeStr = qualType.getAsString();

		const size_t len = std::string("class ").size();
		size_t offset = 0;
		size_t pos = typeStr.find("class ", offset);
		while (pos != std::string::npos) {
			typeStr.erase(pos, len);
			offset += pos;
			pos = typeStr.find("class ", offset);
		}

		return typeStr;
	}
}