/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ScalarConverter.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sruff <sruff@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/08 15:20:10 by sruff             #+#    #+#             */
/*   Updated: 2026/05/10 17:06:52 by sruff            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//#include "ScalarConverter.hpp"
#include <cmath>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <limits>
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

static void	printResults(char c, int i, float f, double d, int typeState)
{
	// typeState: 0=char, 1=int, 2=float, 3=double
	std::cout << "char: ";
	if (typeState == 4 || std::isnan(f) || std::isinf(f))
		std::cout << "impossible\n";
	else if (i < 0 || i > 127)
		std::cout << "impossible\n";
	else if (c < 32 || c > 126)
		std::cout << "Non displayable\n";
	else
		std::cout << "'" << c << "'\n";
	std::cout << "int: ";
	if (typeState == 4 || std::isnan(f) || std::isinf(f)
		|| d > static_cast<double>(std::numeric_limits<int>::max())
		|| d < static_cast<double>(std::numeric_limits<int>::min()))
	{
		std::cout << "impossible\n";
	}
	else
	{
		std::cout << i << "\n";
	}
	std::cout << "float: " << std::fixed << std::setprecision(1) << f << "f\n";
	std::cout << "double: " << std::fixed << std::setprecision(1) << d << "\n";
}

void ScalarConverter::convert(const std::string &str)
{
	char	c;
	char	*endptr;
	long	l;
	int		i;
	double	d;
	float	f;

	if (str.length() == 1 && !std::isdigit(str[0]))
	{
		c = str[0];
		printResults(c, static_cast<int>(c), static_cast<float>(c),
			static_cast<double>(c), 0);
		return ;
	}
	// Try int
	try
	{
		l = std::strtol(str.c_str(), &endptr, 10);
		if (*endptr == '\0')
		{
			if (l < std::numeric_limits<int>::min()
				|| l > std::numeric_limits<int>::max())
				throw std::out_of_range("int overflow");
			i = static_cast<int>(l);
			printResults(static_cast<char>(i), i, static_cast<float>(i),
				static_cast<double>(i), 1);
			return ;
		}
		throw std::invalid_argument("not an int");
	}
	catch (...)
	{
	}
	// Try float
	try
	{
		d = std::strtod(str.c_str(), &endptr);
		if (*endptr == 'f' && *(endptr + 1) == '\0')
		{
			f = static_cast<float>(d);
			printResults(static_cast<char>(f), static_cast<int>(f), f,
				static_cast<double>(f), 2);
			return ;
		}
		throw std::invalid_argument("not a float");
	}
	catch (...)
	{
	}
	// Try double
	try
	{
		d = std::strtod(str.c_str(), &endptr);
		if (*endptr == '\0')
		{
			printResults(static_cast<char>(d), static_cast<int>(d),
				static_cast<float>(d), d, 3);
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
