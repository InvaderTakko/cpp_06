/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sruff <sruff@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/12 18:23:11 by sruff             #+#    #+#             */
/*   Updated: 2026/05/13 18:28:49 by sruff            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Data.hpp"
#include "Serializer.hpp"
#include <iomanip>
#include <iostream>

int	main(void)
{
	Data		data;
	uintptr_t	raw;
	Data		*deserializedData;
	Data		*nullData;
	uintptr_t	rawNull;
	Data		*deserializedNull;

	data.id = 42;
	data.name = "test data";
	data.value = 3.14f;
	std::cout << "--- Valid Pointer Serialization ---" << std::endl;
	std::cout << "OG address: " << &data << std::endl;
	std::cout << "OG values: id=" << data.id << ", name=" << data.name << ", value=" << data.value << std::endl;
	raw = Serializer::serialize(&data);
	std::cout << "serialized value (uintptr_t): " << raw  << std::endl;
	std::cout << "serialized loopback check (hex): 0x" << std::hex << raw << std::dec << std::endl;
	deserializedData = Serializer::deserialize(raw);
	std::cout << "deserialized data address: " << deserializedData << std::endl;
	if (deserializedData == &data)
	{
		std::cout << "Huuuuuge success: deserialized pointer matches the original pointer" << std::endl;
		std::cout << "Deserialized data values: id=" << deserializedData->id << ", name=" << deserializedData->name << ", value=" << deserializedData->value << std::endl;
	}
	else
	{
		std::cout << "Error: Deserialized pointer does NOT match the original pointer!" << std::endl;
	}
	std::cout << "\n--- Testing NULL Pointer ---" << std::endl;
	nullData = NULL;
	rawNull = Serializer::serialize(nullData);
	std::cout << "Serialized NULL pointer: " << rawNull << std::endl;
	deserializedNull = Serializer::deserialize(rawNull);
	if (deserializedNull == NULL)
		std::cout << "Yuuuuuuge success: NULL pointer didnt blow up the programm and deserialized to NULL." << std::endl;
	else
		std::cout << "Error: NULL pointer was not deserialized properly" << std::endl;
	return (0);
}
