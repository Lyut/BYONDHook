#pragma once

namespace sigscan
{
	bool DataCompare(const char* base, const char* pattern)
	{
		for (; *pattern; ++base, ++pattern)
			if (*pattern != '?' && *base != *pattern)
				return 0;

		return *pattern == 0;
	}
	unsigned long FindPatternBase(unsigned long address, unsigned long size, const char* pattern)
	{
		for (unsigned long i = 0; i < size; i++)
			if (DataCompare((const char*)(address + i), pattern))
				return address + i;

		return NULL;
	}
	unsigned long FindPattern(const char* dll, const char* pattern)
	{
		return FindPatternBase((unsigned long)GetModuleHandleA(dll), (unsigned long)-1, pattern);
	}
};