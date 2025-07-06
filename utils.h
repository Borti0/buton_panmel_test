#pragma once
#include <iostream>
#include <vector>
#include <stdio.h>

std::vector<std::string> StringSplit(std::string Original, std::string Delimiter);
std::string TrimEndString(std::string Original);

#ifdef __UTILS_IMPL__
std::vector<std::string> StringSplit(std::string Original, std::string Delimiter)
{
	std::vector<std::string> NewLine;
	size_t NewLineLenght = Original.length();
	while (NewLineLenght != 0)
	{
		size_t DelimiterPosition = Original.find(Delimiter);
		if (DelimiterPosition == std::string::npos)
		{
			NewLine.push_back(Original);
			return NewLine;
		}
		
		if (DelimiterPosition == 0) DelimiterPosition++;
		std::string TMP = Original.substr(0, DelimiterPosition);
		if (TMP.length() > 1) NewLine.push_back(Original.substr(0, DelimiterPosition));
		Original = Original.substr(DelimiterPosition, NewLineLenght - DelimiterPosition);
		NewLineLenght = Original.length();
	}
	return NewLine;
}

std::string TrimEndString(std::string Original)
{
	std::string NewLine = Original;
	size_t StrLenght = Original.length() - 1;
	size_t StartTrim = StrLenght;
	for (size_t CharIndex = StrLenght; CharIndex >= 0; CharIndex--)
	{
		if (NewLine[CharIndex] == ' ' or NewLine[CharIndex] == '\t')
			StartTrim -= 1;
		else
			break;
	}
	if (StartTrim != StrLenght)
		NewLine.erase(StartTrim+1, StrLenght);
	return NewLine;
}

#endif