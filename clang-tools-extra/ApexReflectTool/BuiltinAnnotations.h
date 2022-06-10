#pragma once

#include <string_view>

namespace Reflect {

	namespace Annotation {
	
		constexpr std::string_view CLASS = "reflect-class;";
		constexpr std::string_view ENUM = "reflect-enum;";
		constexpr std::string_view PROPERTY = "reflect-property;";
		constexpr std::string_view METHOD = "reflect-method;";
	}

}