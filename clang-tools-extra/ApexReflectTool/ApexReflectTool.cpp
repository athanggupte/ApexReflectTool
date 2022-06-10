#include <iostream>
#include <clang/Frontend/ASTConsumers.h>
#include <clang/Frontend/FrontendActions.h>
#include <clang/Driver/Options.h>
#include <clang/Tooling/Tooling.h>
#include <clang/Tooling/CommonOptionsParser.h>
#include <clang/ASTMatchers/ASTMatchers.h>
#include <clang/ASTMatchers/ASTMatchFinder.h>

#include <llvm/Support/CommandLine.h>

#include "ClassFinder.h"
#include "TypeInfoGenerator.h"

using namespace clang::driver;
using namespace clang::tooling;
using namespace llvm;

namespace {

	cl::OptionCategory option_category("ApexReflectTool");
	static const opt::OptTable &Options = getDriverOptTable();


}

int main(int argc, const char* argv[])
{
	auto expectedOptionsParser = CommonOptionsParser::create(argc, argv, option_category);
	if (!expectedOptionsParser) {
		llvm::Error error = expectedOptionsParser.takeError();
		std::cerr << "Error creating CommonOptionsParser : " << llvm::toString(std::move(error)) << '\n';
		return 1;
	}
	CommonOptionsParser& optionsParser = *expectedOptionsParser;

	ClangTool tool(optionsParser.getCompilations(), optionsParser.getSourcePathList());

	using namespace clang::ast_matchers;
	Reflect::MatchFinder matchFinder;
	Reflect::GeneratorBuilderImpl<Reflect::TypeInfoGenerator> genBuilder;
	Reflect::ClassFinder classFinder{ &genBuilder };
		
	DeclarationMatcher classMatcher = cxxRecordDecl(decl().bind("id"), hasAttr(clang::attr::Annotate), isExpansionInMainFile());
	DeclarationMatcher enumMatcher = enumDecl(decl().bind("id"), hasAttr(clang::attr::Annotate), isExpansionInMainFile());
	DeclarationMatcher fieldMatcher = fieldDecl(decl().bind("id"), hasAttr(clang::attr::Annotate), isExpansionInMainFile());
	DeclarationMatcher functionMatcher = functionDecl(decl().bind("id"), hasAttr(clang::attr::Annotate), isExpansionInMainFile());
	DeclarationMatcher dependencyMatcher = cxxRecordDecl(decl().bind("dep"),
											// ( has_annotation AND NOT in_main_file ) OR ( NOT is_definition AND in_main_file)
											anyOf(
												allOf(
													hasAttr(clang::attr::Annotate),
													unless(isExpansionInMainFile())
												),
												allOf(
													unless(isDefinition()),
													unless(isImplicit()),
													isExpansionInMainFile()
												)
											));

	matchFinder.addMatcher(classMatcher, &classFinder);
	matchFinder.addMatcher(enumMatcher, &classFinder);
	matchFinder.addMatcher(fieldMatcher, &classFinder);
	matchFinder.addMatcher(functionMatcher, &classFinder);
	matchFinder.addMatcher(dependencyMatcher, &classFinder);

	return tool.run(newFrontendActionFactory(&matchFinder).get());
}