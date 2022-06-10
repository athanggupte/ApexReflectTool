#pragma once

#include "Generator.h"
#include <fmt/os.h>

namespace Reflect {

	class TypeInfoGenerator : public Generator
	{
	public:
		TypeInfoGenerator(const detail::ReflectionRegistry& registry, const llvm::StringRef& source_filename);

		~TypeInfoGenerator() override;

		bool GeneratePreamble() override;
		bool GenerateClass(const detail::Class& c) override;
		bool GenerateField(const detail::Field& c) override;
		bool GenerateFunction(const detail::Function& c) override;
		bool GenerateClassEpilogue(const detail::Class& c) override;
		bool GenerateEnum(const detail::Enum& e) override;

	private:
		std::string gen_file;
		fmt::ostream os;
	};

}