#pragma once

#include <Windows.h>
#include <string>
#include <functional>
#include <type_traits>

template<class Type>
using TypeRPMFunc = std::function<bool(Type Address, void* Buffer, size_t Size)>;

class PatternScan {
private:
	constexpr static unsigned MAX_MASK_SIZE = 0x100;

	static bool DataCompare(uint8_t* pData, uint8_t* bMask, uint8_t* vMask) {
		for (; *bMask; ++bMask, ++pData, ++vMask)
			if (*bMask == 'x' && *pData != *vMask)
				return false;
		return true;
	}

	static uintptr_t FindPattern(uintptr_t Address, size_t Len, uint8_t* bMask, uint8_t* vMask) {
		for (size_t i = 0; i < Len; i++)
			if (DataCompare((uint8_t*)(Address + i), bMask, vMask))
				return Address + i;
		return 0;
	}

	static bool ConvertStringToMask(std::string strPattern, uint8_t* bMask, uint8_t* vMask, size_t MaxSize) {
		memset(bMask, 0, MaxSize);
		memset(vMask, 0, MaxSize);
		uintptr_t i = 0, j = 0, k = 0;
		while (strPattern[i]) {
			if (strPattern[i] == ' ') {
				k = 0;
				j++;
				if (j >= MaxSize)
					return false;
			}
			else if (strPattern[i] >= '0' && strPattern[i] <= '9') {
				bMask[j] = 'x';
				if (k == 1) vMask[j] += strPattern[i] - '0';
				else if (k == 0) vMask[j] += (strPattern[i] - '0') * 0x10;
				else return false;
				k++;
			}
			else if (strPattern[i] >= 'a' && strPattern[i] <= 'f') {
				bMask[j] = 'x';
				if (k == 1) vMask[j] += strPattern[i] - 'a' + 0xA;
				else if (k == 0) vMask[j] += (strPattern[i] - 'a' + 0xA) * 0x10;
				else return false;
				k++;
			}
			else if (strPattern[i] >= 'A' && strPattern[i] <= 'F') {
				bMask[j] = 'x';
				if (k == 1) vMask[j] += strPattern[i] - 'A' + 0xA;
				else if (k == 0) vMask[j] += (strPattern[i] - 'A' + 0xA) * 0x10;
				else return false;
				k++;
			}
			else if (strPattern[i] == '?') {
				bMask[j] = '?';
				vMask[j] = false;
			}
			else return false;
			i++;
		}
		return true;
	}

	template <class Type>
	static bool GetModuleInfo(Type ModuleBase, DWORD& BaseOfCode, DWORD& SizeOfCode, TypeRPMFunc<Type> RPM) {
		static_assert(std::is_same<Type, uint32_t>::value | std::is_same<Type, uint64_t>::value, "Type must be 32bit or 64bit.");

		IMAGE_DOS_HEADER DosHeader;
		if (!RPM(ModuleBase, &DosHeader, sizeof(DosHeader)))
			return false;

		if (DosHeader.e_magic != IMAGE_DOS_SIGNATURE)
			return false;

		using NtHeadersType = IMAGE_NT_HEADERS64;
		if (std::is_same<Type, uint32_t>::value)
			using NtHeadersType = IMAGE_NT_HEADERS32;

		NtHeadersType NtHeaders;
		if (!RPM(ModuleBase + DosHeader.e_lfanew, &NtHeaders, sizeof(NtHeaders)))
			return false;

		if (NtHeaders.Signature != IMAGE_NT_SIGNATURE)
			return false;

		BaseOfCode = NtHeaders.OptionalHeader.BaseOfCode;
		SizeOfCode = NtHeaders.OptionalHeader.SizeOfCode;
		return true;
	}

public:
	template <class Type>
	static Type Range(Type Address, size_t Len, uint8_t* bMask, uint8_t* vMask, TypeRPMFunc<Type> RPM) {
		static_assert(std::is_same<Type, uint32_t>::value | std::is_same<Type, uint64_t>::value, "Type must be 32bit or 64bit.");

		Type Start = Address & ~0xFFF;
		Type End = (Address + Len + 0x1000) & ~0xFFF;

		for (Type i = Start; i < End; i += 0x1000) {
			uint8_t Buf[0x2000];

			if (!RPM(i, Buf, 0x1000))
				continue;

			RPM(i + 0x1000, Buf + 0x1000, 0x1000);

			Type AddressTemp = (Type)Buf;
			while (1) {
				Type LocalResult = FindPattern(AddressTemp, 0x1000, bMask, vMask);
				if (LocalResult == 0)
					break;
				else {
					Type Result = LocalResult - (Type)Buf + i;
					if (Result >= Address && Result < Address + Len && Result != (Type)vMask)
						return Result;

					AddressTemp = LocalResult + 1;
				}
			}
		}
		return 0;
	}

	template <class Type>
	static Type Range(Type Address, size_t Len, std::string strPattern, TypeRPMFunc<Type> RPM) {
		static_assert(std::is_same<Type, uint32_t>::value | std::is_same<Type, uint64_t>::value, "Type must be 32bit or 64bit.");

		uint8_t bMask[MAX_MASK_SIZE] = { 0 };
		uint8_t vMask[MAX_MASK_SIZE] = { 0 };
		if (!ConvertStringToMask(strPattern, bMask, vMask, MAX_MASK_SIZE))
			return 0;

		return Range(Address, Len, bMask, vMask, RPM);
	}

	template <class Type>
	static Type Module(Type ModuleBase, uint8_t* bMask, uint8_t* vMask, TypeRPMFunc<Type> RPM) {
		static_assert(std::is_same<Type, uint32_t>::value | std::is_same<Type, uint64_t>::value, "Type must be 32bit or 64bit.");

		DWORD BaseOfCode, SizeOfCode;
		if (!GetModuleInfo<Type>(ModuleBase, BaseOfCode, SizeOfCode, RPM))
			return 0;

		return Range(ModuleBase + BaseOfCode, SizeOfCode, bMask, vMask, RPM);
	}

	template <class Type>
	static Type Module(Type ModuleBase, std::string strPattern, TypeRPMFunc<Type> RPM) {
		static_assert(std::is_same<Type, uint32_t>::value | std::is_same<Type, uint64_t>::value, "Type must be 32bit or 64bit.");

		DWORD BaseOfCode, SizeOfCode;
		if (!GetModuleInfo<Type>(ModuleBase, BaseOfCode, SizeOfCode, RPM))
			return 0;

		return Range(ModuleBase + BaseOfCode, SizeOfCode, strPattern, RPM);
	}

	static uintptr_t Range(uintptr_t Address, size_t Len, uint8_t* bMask, uint8_t* vMask, TypeRPMFunc<uintptr_t> RPM) {
		return Range<uintptr_t>(Address, Len, bMask, vMask, RPM);
	}

	static uintptr_t Range(uintptr_t Address, size_t Len, std::string strPattern, TypeRPMFunc<uintptr_t> RPM) {
		return Range<uintptr_t>(Address, Len, strPattern, RPM);
	}

	static uintptr_t Module(uintptr_t ModuleBase, uint8_t* bMask, uint8_t* vMask, TypeRPMFunc<uintptr_t> RPM) {
		return Module<uintptr_t>(ModuleBase, bMask, vMask, RPM);
	}

	static uintptr_t Module(uintptr_t ModuleBase, std::string strPattern, TypeRPMFunc<uintptr_t> RPM) {
		return Module<uintptr_t>(ModuleBase, strPattern, RPM);
	}
};