/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scala.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sruff <sruff@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/08 15:21:17 by sruff             #+#    #+#             */
/*   Updated: 2026/05/11 18:49:09 by sruff            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ScalarConverter.hpp"
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <iomanip>
#include <ios>
#include <iostream>
#include <limits>
#include <ostream>
#include <stdexcept>
#include <string>

ScalarConverter::ScalarConverter()
{
}
ScalarConverter::ScalarConverter(const ScalarConverter &)
{
}
ScalarConverter &ScalarConverter::operator=(const ScalarConverter &)
{
	return (*this);
}
ScalarConverter::~ScalarConverter()
{
}

static void	format_and_print(char c, int32_t i, float f, double d,
		char typeInfo)
{
	// char
	std::cout << "char: ";
	if (std::isinf(f) || std::isnan(f)) //
		std::cout << "impossible" << std::endl;
	else if (i > 127 || i < 0)
		std::cout << "impossible" << std::endl;
	else if (c > 126 || c < 32)
		std::cout << "none displayable" << std::endl;
	else
		std::cout << c << std::endl;
	// int
	std::cout << "int: ";
	if (i < std::numeric_limits<int>::min()
		|| i > std::numeric_limits<int>::max())
		std::cout << "impossibel" << std::endl;
	else
		std::cout << i << std::endl;
	// float
	if (typeInfo == 'f')
	{
		std::cout << "float: " << std::fixed << std::setprecision(1) << f << "f\n";
	}
	else if (typeInfo == 'd')
		std::cout << "double: " << std::fixed << std::setprecision(1) << std::endl;
	// double
}

void ScalarConverter::convert(const std::string &str)
{
	char *end;
	float f;
	double d; // stdint equivalent only in c++23
	if (!std::isdigit(str[0]) && str.length() == 1)
	{
		std::cout << static_cast<int>(str[0]) << std::endl;
		return ;
	}

	try // int
	{
		int64_t l = std::strtol(str.c_str(), &end, 10);
		if (*end == '\0')
		{
			if (l < std::numeric_limits<int>::min()
				|| l > std::numeric_limits<int>::max())
				throw std::out_of_range("int under/overflow");
			std::cout << static_cast<int>(l) << std::endl;
			return ;
		}
	}
	catch (...)
	{
	}

	try // float
	{
		d = std::strtod(str.c_str(), &end);
		if (*end == 'f' && *(end + 1) == '\0')
		{
			if (d > std::numeric_limits<float>::max()
				|| d < std::numeric_limits<float>::min())
				throw std::out_of_range("float over/underflow");
			f = static_cast<float>(d);
			std::cout << f << std::endl;
			// format_and_print(char c, int32_t i, float f, double d,
				char typeInfo)
			return ;
		}
		throw std::invalid_argument("not a float");
	}
	catch (...)
	{
	}
	try // double
	{
		d = std::strtod(str.c_str(), &end);
		if (*end == '\0')
		{
			std::cout << d << std::endl;
			return ;
		}
		throw std::invalid_argument("not a double");
	}
	catch (...)
	{
	}

	std::cout << "char: impossible\n";
	std::cout << "int: impossible\n";
	std::cout << "float: impossible\n";
	std::cout << "double: impossible\n";
}