/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scala.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sruff <sruff@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/08 15:21:17 by sruff             #+#    #+#             */
/*   Updated: 2026/05/10 17:06:49 by sruff            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ScalarConverter.hpp"
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <iomanip>
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

void ScalarConverter::convert(const std::string &str)
{
	char *end;
	if (!std::isdigit(str[0]) && str.length() == 1)
	{
		std::cout << static_cast<int>(str[0]) << std::endl;
		return ;
	}

	try
	{
		int64_t l = std::strtol(str.c_str(), &end, 10);
		if (*end == '\0')
		{
			if (l < std::numeric_limits<int>::min()
				|| l > std::numeric_limits<int>::max())
				throw std::out_of_range("int under/overflow");
			std::cout << static_cast<int>(l) << std::endl;
		}
	}
	catch (...)
	{
	}
}