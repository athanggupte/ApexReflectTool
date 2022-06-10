#pragma once
#include "ReflectionData.h"

namespace Reflect {

	class Generator
	{
	public:
		virtual void Generate();
		virtual ~Generator() = default;

		void SetContext(clang::ASTContext* context);

	protected:
		Generator(const detail::ReflectionRegistry& registry, const llvm::StringRef& source_filename);
		virtual bool GeneratePreamble();
		virtual bool GenerateClass(const detail::Class& c);
		virtual bool GenerateField(const detail::Field& c);
		virtual bool GenerateFunction(const detail::Function& c);
		virtual bool GenerateClassEpilogue(const detail::Class& c);
		virtual bool GenerateEnum(const detail::Enum& e);
		virtual bool GenerateEpilogue();

	protected:
		const detail::ReflectionRegistry* m_Registry;
		llvm::StringRef m_SourceFilename;
		clang::ASTContext* m_Context;
	};

	class GeneratorBuilder
	{
	public:
		virtual std::unique_ptr<Generator> Build(const detail::ReflectionRegistry& registry, const llvm::StringRef& source_filename) const = 0;
	};

	template <typename T, typename = std::enable_if_t<std::is_base_of_v<Generator, T>>>
	class GeneratorBuilderImpl : public GeneratorBuilder
	{
	public:
		std::unique_ptr<Generator> Build(const detail::ReflectionRegistry& registry, const llvm::StringRef& source_filename) const override
		{
			return std::make_unique<T>(registry, source_filename);
		}
	};
}
