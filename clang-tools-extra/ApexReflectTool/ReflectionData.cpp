#include "ReflectionData.h"
#include "BuiltinAnnotations.h"

#include <clang/AST/Attr.h>
#include <iostream>

using namespace clang;

namespace Reflect {
	namespace detail {
	
		Class& ReflectionRegistry::EmplaceClass(const CXXRecordDecl* d)
		{
			Class& c = classes.emplace_back(d, *this);
			c.field_beg = c.field_end = fields.size();
			c.func_beg = c.func_end = functions.size();

			if (auto it = dependencies.find(c); it != dependencies.end()) {
				dependencies.erase(it);
			}

			return c;
		}

		Field& ReflectionRegistry::EmplaceField(const clang::FieldDecl * d)
		{
			auto classIdx = classes.size() - 1;
			Class& c = classes[classIdx];

			Field& f = fields.emplace_back(d, *this);
			f.parent_itr = classIdx;
			f.field_idx = c.NumFields();
			c.field_end = fields.size();
			return f;
		}

		Function& ReflectionRegistry::EmplaceFunction(const clang::FunctionDecl * d)
		{
			auto classIdx = classes.size() - 1;
			Class& c = classes[classIdx];

			Function& f = functions.emplace_back(d, *this);
			f.parent_itr = classIdx;
			f.func_idx = c.NumFuncs();
			c.func_end = functions.size();
			return f;
		}

        Enum & ReflectionRegistry::EmplaceEnum(const clang::EnumDecl *d)
		{
			Enum& e = enums.emplace_back(d, *this);
			return e;
        }

		bool ReflectionRegistry::EmplaceDependency(const clang::CXXRecordDecl *d) {
			auto it = std::find_if(classes.begin(), classes.end(), [d](const Class& c){
				return c.clang_Decl->getID() == d->getID();
			});
			if (it != classes.end())
				return false;

			auto [_, success] = dependencies.emplace(d, *this);
			return success;
		}

		Iterable<std::vector<Field>> Class::Fields() const
		{
			return Iterable<std::vector<Field>>(parent_registry.fields.begin() + field_beg, parent_registry.fields.begin() + field_end);
		}

		Iterable<std::vector<Function>> Class::Functions() const
		{
			return Iterable<std::vector<Function>>(parent_registry.functions.begin() + func_beg, parent_registry.functions.begin() + func_end);
		}

		const Class& Field::ParentClass() const
		{
			return parent_registry.classes[parent_itr];
		}

		const Class& Function::ParentClass() const
		{
			return parent_registry.classes[parent_itr];
		}

	} // namespace detail
} // namespace Reflect

