#pragma once

#include <clang/AST/AST.h>
#include "Iterable.hpp"
#include <set>

namespace Reflect {
	namespace detail {
		
		struct Class;
		struct Field;
		struct Function;
		struct Enum;
	}
}

template <>
struct std::less<Reflect::detail::Class>
{
	bool operator() (const Reflect::detail::Class& lhs, const Reflect::detail::Class& rhs) const;
};

namespace Reflect {
	namespace detail {

		struct ReflectionRegistry
		{
			Class& EmplaceClass(const clang::CXXRecordDecl* d);
			Field& EmplaceField(const clang::FieldDecl* d);
			Function& EmplaceFunction(const clang::FunctionDecl* d);
			Enum& EmplaceEnum(const clang::EnumDecl* d);

			bool EmplaceDependency(const clang::CXXRecordDecl* d);

			std::vector<Class> classes;
			std::set<Class> dependencies;
			std::vector<Field> fields;
			std::vector<Function> functions;
			std::vector<Enum> enums;
			std::vector<llvm::StringRef> filenames;
		};

		struct Class
		{
			Iterable<std::vector<Field>> Fields() const;
			Iterable<std::vector<Function>> Functions() const;
			size_t NumFields() const { return field_end - field_beg; }
			size_t NumFuncs() const { return func_end - func_beg; }

			const clang::CXXRecordDecl* clang_Decl = nullptr;
			const llvm::StringRef* clang_SourceFile = nullptr;

			//Class() = default;
			Class(const clang::CXXRecordDecl* d, ReflectionRegistry& registry)
				: clang_Decl(d), parent_registry(registry) {}

			friend struct ReflectionRegistry;

		private:
			uint32_t field_beg, field_end;
			uint32_t func_beg, func_end;
			ReflectionRegistry& parent_registry;
		};

		struct Enum
		{
			const clang::EnumDecl* clang_Decl = nullptr;
			const llvm::StringRef* clang_SourceFile = nullptr;

			Enum(const clang::EnumDecl* d, ReflectionRegistry& registry)
				: clang_Decl(d), parent_registry(registry) {}

			friend struct ReflectionRegistry;

		private:
			ReflectionRegistry& parent_registry;
		};

		struct Field
		{
			const Class& ParentClass() const;
			uint32_t Index() const { return field_idx; } // index of field in parent class
			const clang::FieldDecl* clang_Decl = nullptr;

			//Field() = default;
			Field(const clang::FieldDecl* d, ReflectionRegistry& registry)
				: clang_Decl(d), parent_registry(registry) {}

			friend struct ReflectionRegistry;

		private:
			uint32_t parent_itr;
			uint32_t field_idx;
			ReflectionRegistry& parent_registry;
		};
	
		struct Function
		{
			const Class& ParentClass() const;
			uint32_t Index() const { return func_idx; } // index of function in parent class
			const clang::FunctionDecl* clang_Decl = nullptr;
		
			//Function() = default;
			Function(const clang::FunctionDecl* d, ReflectionRegistry& registry)
				: clang_Decl(d), parent_registry(registry) {}

			friend struct ReflectionRegistry;

		private:
			uint32_t parent_itr;
			uint32_t func_idx;
			ReflectionRegistry& parent_registry;
		};

	} // namespace detail
} // namespace Reflect

inline bool std::less<Reflect::detail::Class>::operator()(const Reflect::detail::Class &lhs, const Reflect::detail::Class &rhs) const
{
	return lhs.clang_Decl->getQualifiedNameAsString() < rhs.clang_Decl->getQualifiedNameAsString();
}
