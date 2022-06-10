#include "Generator.h"

namespace Reflect {
	
	Generator::Generator(const detail::ReflectionRegistry& registry, const llvm::StringRef& source_filename)
	{
		m_Registry = &registry;
		m_SourceFilename = source_filename;
	}

	void Generator::Generate()
	{
		GeneratePreamble();
		for (const detail::Class& c : m_Registry->classes) {
			if (GenerateClass(c)) {
				for (const detail::Field& f : c.Fields()) {
					GenerateField(f);
				}
				for (const detail::Function& f : c.Functions()) {
					GenerateFunction(f);
				}
			}
			GenerateClassEpilogue(c);
		}
		for (const detail::Enum& e : m_Registry->enums) {
			GenerateEnum(e);
		}
		GenerateEpilogue();
	}

	void Generator::SetContext(clang::ASTContext *context)
	{
		m_Context = context;
	}

	bool Generator::GeneratePreamble() { return true;}
	bool Generator::GenerateClass(const detail::Class& c) { return true; }
	bool Generator::GenerateField(const detail::Field& c) { return true; }
	bool Generator::GenerateFunction(const detail::Function& c) { return true; }
	bool Generator::GenerateClassEpilogue(const detail::Class& c) { return true; }
	bool Generator::GenerateEnum(const detail::Enum& e) { return true; }
	bool Generator::GenerateEpilogue() { return true; }

}
