/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scala.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sruff <sruff@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/08 15:21:17 by sruff             #+#    #+#             */
/*   Updated: 2026/05/12 17:49:11 by sruff            ###   ########.fr       */
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
	(void)typeInfo;
	// char
	std::cout << "char: ";
	if (std::isinf(f) || std::isnan(f)) //
		std::cout << "impossible" << std::endl;
	else if (i > 127 || i < 0)
		std::cout << "impossible" << std::endl;
	else if (c > 126 || c < 32)
		std::cout << "Non displayable" << std::endl;
	else
		std::cout << "'" << c << "'" << std::endl;
	// int
	std::cout << "int: ";
	if (std::isnan(f) || std::isinf(f))
		std::cout << "impossible" << std::endl;
	else if (d < std::numeric_limits<int>::min()
		|| d > std::numeric_limits<int>::max())
		std::cout << "impossible" << std::endl;
	else
		std::cout << i << std::endl;
	// float
	std::cout << "float: ";
	if (std::isinf(f) && f > 0)
		std::cout << "+inff\n";
	else
		std::cout << std::fixed << std::setprecision(1) << f << "f\n";
	// double
	std::cout << "double: ";
	if (std::isinf(d) && d > 0)
		std::cout << "+inf\n";
	else
		std::cout << std::fixed << std::setprecision(1) << d << std::endl;
}

void ScalarConverter::convert(const std::string &str)
{
	char *end;
	// char	c;
	int64_t l;
	float f;
	double d; // stdint equivalent only in c++23

	if (str.empty())
	{
		std::cout << "char: impossible\n";
		std::cout << "int: impossible\n";
		std::cout << "float: impossible\n";
		std::cout << "double: impossible\n";
		return ;
	}

	if (!std::isdigit(str[0]) && str.length() == 1)
	{
		// std::cout << static_cast<int>(str[0]) << std::endl;
		format_and_print(str[0], static_cast<int>(str[0]),
			static_cast<float>(str[0]), static_cast<double>(str[0]), 't');
		return ;
	}

	try // int
	{
		l = std::strtol(str.c_str(), &end, 10);
		if (*end == '\0')
		{
			if (l < std::numeric_limits<int>::min()
				|| l > std::numeric_limits<int>::max())
				throw std::out_of_range("int under/overflow");
			// std::cout << static_cast<int>(l) << std::endl;
			format_and_print(static_cast<int>(l), static_cast<int>(l),
				static_cast<float>(l), static_cast<double>(l), 't');
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
			if (!std::isinf(d) && !std::isnan(d)
				&& (d > std::numeric_limits<float>::max()
					|| d < std::numeric_limits<float>::lowest()))
				// use lowest instead of min()
				throw std::out_of_range("float over/underflow");
			f = static_cast<float>(d);
			// std::cout << f << std::endl;
			format_and_print(static_cast<char>(f), static_cast<int>(f), f,
				static_cast<double>(d), 't');
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
			// std::cout << d << std::endl;
			format_and_print(static_cast<char>(d), static_cast<int>(d),
				static_cast<float>(d), d, 't');
			return ;
		}
		throw std::invalid_argument("not a double");
	}
	catch (...)
	{
	}
	// if no check applies
	std::cout << "char: impossible\n";
	std::cout << "int: impossible\n";
	std::cout << "float: impossible\n";
	std::cout << "double: impossible\n";
}