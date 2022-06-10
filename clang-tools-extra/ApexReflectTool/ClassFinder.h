#pragma once

#include <clang/ASTMatchers/ASTMatchers.h>
#include <clang/ASTMatchers/ASTMatchFinder.h>
#include "ReflectionData.h"
#include "Generator.h"

namespace Reflect {

	using clang::ast_matchers::MatchFinder;

	class ClassFinder : public MatchFinder::MatchCallback
	{
	public:
		ClassFinder(GeneratorBuilder* generator_builder) : m_GeneratorBuilder(generator_builder) {}

		void run(const MatchFinder::MatchResult& result) override;

		void onStartOfTranslationUnit() override;
		void onEndOfTranslationUnit() override;

		void FoundRecord(const clang::CXXRecordDecl* d);
		void FoundEnum(const clang::EnumDecl* d);
		void FoundField(const clang::FieldDecl* d);
		void FoundFunction(const clang::FunctionDecl* d);
		void FoundDependency(const clang::CXXRecordDecl* d);

	private:
		detail::ReflectionRegistry m_Registry;
		GeneratorBuilder* m_GeneratorBuilder;
		clang::ASTContext* m_Context;
		llvm::StringRef m_MainFileName;
	};
}
