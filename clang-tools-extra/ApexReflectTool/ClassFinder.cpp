#include "ClassFinder.h"
#include "ReflectionData.h"
#include "BuiltinAnnotations.h"
#include "Generator.h"
#include <iostream>

namespace Reflect {

	using namespace clang;
	using namespace clang::ast_matchers;

	static StringRef GetMainFileName(ASTContext& context)
	{
		FileID mainFileId = context.getSourceManager().getMainFileID();
		const FileEntry* mainFileEntry = context.getSourceManager().getFileEntryForID(mainFileId);
		return mainFileEntry->getName();
	}

	void ClassFinder::run(const MatchFinder::MatchResult &result)
	{
		//std::cout << "Running ClassFinder..." << '\n';
		m_Context = result.Context;
		m_MainFileName = GetMainFileName(*m_Context);

		// std::cout << "Dir:" << mainFileEntry->getDir()->getName().str() << '\n';
		// std::cout << "MainFileName : " << m_MainFileName.str() << '\n';

		const CXXRecordDecl* record = result.Nodes.getNodeAs<CXXRecordDecl>("id");
		if (record)
			return FoundRecord(record);

		const FieldDecl* field = result.Nodes.getNodeAs<FieldDecl>("id");
		if (field)
			return FoundField(field);

		const FunctionDecl* function = result.Nodes.getNodeAs<FunctionDecl>("id");
		if (function)
			return FoundFunction(function);

		const EnumDecl* enumeration = result.Nodes.getNodeAs<EnumDecl>("id");
		if (enumeration)
			return FoundEnum(enumeration);

		const CXXRecordDecl* dependency = result.Nodes.getNodeAs<CXXRecordDecl>("dep");
		if (dependency)
			return FoundDependency(dependency);
	}

	void ClassFinder::onStartOfTranslationUnit()
	{
		m_Registry = Reflect::detail::ReflectionRegistry{};
	}

	void ClassFinder::onEndOfTranslationUnit()
	{
		auto gen = m_GeneratorBuilder->Build(m_Registry, m_MainFileName);
		gen->SetContext(m_Context);
		gen->Generate();
	}

	void ClassFinder::FoundRecord(const CXXRecordDecl *d)
	{
		std::cout << "Record   : " << d->getNameAsString() << '\n';
		auto annotation = d->getAttr<AnnotateAttr>()->getAnnotation();
		if (annotation.consume_front(Reflect::Annotation::CLASS)) {
			std::cout << "         : " << "reflected" << '\n';
			detail::Class& c = m_Registry.EmplaceClass(d);
			const clang::SourceManager& sourceManager = m_Context->getSourceManager();
			llvm::StringRef& filename = m_Registry.filenames.emplace_back(sourceManager.getFilename(d->getLocation()));
			c.clang_SourceFile = &filename;
			/*for (const clang::CXXBaseSpecifier& base : d->bases()) {
				if (CXXRecordDecl* baseClass = base.getType().getTypePtr()->getAsCXXRecordDecl()) {
					m_Registry.EmplaceDependency(baseClass);
				}
			}*/
		}
	}

	void ClassFinder::FoundEnum(const clang::EnumDecl *d)
	{
		std::cout << "Enum     : " << d->getNameAsString() << '\n';
		auto annotation = d->getAttr<AnnotateAttr>()->getAnnotation();
		if (annotation.consume_front(Reflect::Annotation::ENUM)) {
			std::cout << "         : " << "reflected" << '\n';
			m_Registry.EmplaceEnum(d);
		}
	}

	void ClassFinder::FoundField(const clang::FieldDecl *d)
	{
		std::cout << "Field    : " << d->getNameAsString() << '\n';
		auto annotation = d->getAttr<AnnotateAttr>()->getAnnotation();
		if (annotation.consume_front(Reflect::Annotation::PROPERTY)) {
			const CXXRecordDecl* parentClass = llvm::dyn_cast<clang::CXXRecordDecl>(d->getParent());
			if (parentClass && !m_Registry.classes.empty() && m_Registry.classes.back().clang_Decl == parentClass) {
				std::cout << "         : " << "reflected" << '\n';
				m_Registry.EmplaceField(d);
				/*const Type* type = d->getType().getTypePtr();
				if (CXXRecordDecl* class_decl = type->getAsCXXRecordDecl()) {
					if (ClassTemplateDecl* template_decl = class_decl->getDescribedClassTemplate()) {
						for (auto template_param : *template_decl->getDescribedTemplateParams()) {
							template_param->
						}
					} else {
						m_Registry.EmplaceDependency(class_decl);
					}
				}*/
			}
		}
	}

	void ClassFinder::FoundFunction(const clang::FunctionDecl *d)
	{
		std::cout << "Function : " << d->getNameAsString() << '\n';
		auto annotation = d->getAttr<AnnotateAttr>()->getAnnotation();
		if (annotation.consume_front(Reflect::Annotation::METHOD)) {
			if (auto method = llvm::dyn_cast<CXXMethodDecl>(d)) {
				const CXXRecordDecl* parentClass = method->getParent();
				if (parentClass && !m_Registry.classes.empty() && m_Registry.classes.back().clang_Decl == parentClass) {
					std::cout << "         : " << "reflected" << '\n';
					m_Registry.EmplaceFunction(d);
				}
			}
		}
	}

	void ClassFinder::FoundDependency(const clang::CXXRecordDecl *d)
	{
		std::cout << "Dependency : " << d->getNameAsString() << '\n';
		m_Registry.EmplaceDependency(d);
	}

}