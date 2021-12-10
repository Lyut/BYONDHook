#pragma once
#include <Windows.h>
#include <stdio.h>
#include <iostream>
#include "PEFile.h"

char strLast[318] = { 0x43, 0x3A, 0x5C, 0x42, 0x59, 0x4F, 0x4E, 0x44, 0x5C, 0x42, 0x59, 0x4F, 0x4E, 0x44, 0x48, 0x6F, 0x6F, 0x6B,
   0x2E, 0x64, 0x6C, 0x6C, 0x00, 0x00, 0x00, 0x4D, 0x61, 0x69, 0x6E, 0x00, 0x17, 0xF0, 0x0B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08,
   0xC9, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x64, 0x08, 0x00, 0x2C, 0x4A, 0x06, 0x00, 0xE0, 0xC2,
   0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x90, 0x64, 0x08, 0x00, 0x04, 0x44, 0x06, 0x00, 0xF8, 0xC2, 0x07,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE6, 0x9C, 0x08, 0x00, 0x1C, 0x44, 0x06, 0x00, 0xFC, 0xBF, 0x07, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xBA, 0xA0, 0x08, 0x00, 0x20, 0x41, 0x06, 0x00, 0x38, 0xD6, 0x07, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x98, 0xA1, 0x08, 0x00, 0x5C, 0x57, 0x06, 0x00, 0x0C, 0xBF, 0x07, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0xA6, 0x08, 0x00, 0x30, 0x40, 0x06, 0x00, 0xA8, 0xC1, 0x07, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x0E, 0xAB, 0x08, 0x00, 0xCC, 0x42, 0x06, 0x00, 0xE4, 0xBE, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0xC0, 0xAB, 0x08, 0x00, 0x08, 0x40, 0x06, 0x00, 0x90, 0xC1, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x28, 0xAC, 0x08, 0x00, 0xB4, 0x42, 0x06, 0x00, 0xDC, 0xBE, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x48, 0xAC, 0x08, 0x00, 0x00, 0x40, 0x06, 0x00, 0xF0, 0xC2, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x56, 0xAC, 0x08, 0x00, 0x14, 0x44, 0x06, 0x00, 0x84, 0xC1, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x90, 0xAC, 0x08, 0x00, 0xA8, 0x42, 0x06, 0x00, 0x1E, 0xF0, 0x0B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0xF0, 0x0B, 0x00, 0x1E, 0xF0, 0x0B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

char strFirst[784] = { 0x4D, 0x5A, 0x90, 0x00, 0x03, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xB8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x01, 0x00, 0x00, 0x0E, 0x1F, 0xBA, 0x0E, 0x00, 0xB4, 0x09, 0xCD, 0x21, 0xB8, 0x01,
  0x4C, 0xCD, 0x21, 0x54, 0x68, 0x69, 0x73, 0x20, 0x70, 0x72, 0x6F, 0x67, 0x72, 0x61, 0x6D, 0x20, 0x63, 0x61, 0x6E, 0x6E, 0x6F, 0x74, 0x20, 0x62, 0x65, 0x20,
  0x72, 0x75, 0x6E, 0x20, 0x69, 0x6E, 0x20, 0x44, 0x4F, 0x53, 0x20, 0x6D, 0x6F, 0x64, 0x65, 0x2E, 0x0D, 0x0D, 0x0A, 0x24, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x7D, 0x55, 0x23, 0x9E, 0x39, 0x34, 0x4D, 0xCD, 0x39, 0x34, 0x4D, 0xCD, 0x39, 0x34, 0x4D, 0xCD, 0x7F, 0x65, 0xAC, 0xCD, 0x3F, 0x34, 0x4D, 0xCD, 0x7F,
  0x65, 0x90, 0xCD, 0x3B, 0x34, 0x4D, 0xCD, 0xA7, 0x94, 0x8A, 0xCD, 0x31, 0x34, 0x4D, 0xCD, 0x34, 0x66, 0x90, 0xCD, 0x3B, 0x34, 0x4D, 0xCD, 0x34, 0x66, 0x92,
  0xCD, 0x3E, 0x34, 0x4D, 0xCD, 0x34, 0x66, 0xAD, 0xCD, 0x2A, 0x34, 0x4D, 0xCD, 0x30, 0x4C, 0xDE, 0xCD, 0x2B, 0x34, 0x4D, 0xCD, 0x34, 0x66, 0xAC, 0xCD, 0x31,
  0x34, 0x4D, 0xCD, 0x8C, 0xAA, 0x90, 0xCD, 0x3C, 0x34, 0x4D, 0xCD, 0x39, 0x34, 0x4C, 0xCD, 0x2C, 0x33, 0x4D, 0xCD, 0x8C, 0xAA, 0xAD, 0xCD, 0x38, 0x34, 0x4D,
  0xCD, 0x8C, 0xAA, 0xAC, 0xCD, 0x2E, 0x34, 0x4D, 0xCD, 0x34, 0x66, 0x96, 0xCD, 0x38, 0x34, 0x4D, 0xCD, 0x39, 0x34, 0xDA, 0xCD, 0x38, 0x34, 0x4D, 0xCD, 0x8C,
  0xAA, 0x93, 0xCD, 0x38, 0x34, 0x4D, 0xCD, 0x52, 0x69, 0x63, 0x68, 0x39, 0x34, 0x4D, 0xCD, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x50, 0x45, 0x00,
  0x00, 0x4C, 0x01, 0x06, 0x00, 0x6B, 0x2B, 0x9D, 0x61, 0x5B, 0x4C, 0x6F, 0x72, 0x64, 0x50, 0x45, 0x5D, 0xE0, 0x00, 0x22, 0x01, 0x0B, 0x01, 0x0C, 0x00, 0x00,
  0x2C, 0x06, 0x00, 0x00, 0x5E, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x25, 0xA2, 0x05, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x40, 0x06, 0x00, 0x00, 0x00, 0x40,
  0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x05, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x0C, 0x00, 0x00, 0x04, 0x00, 0x00, 0x16, 0x08, 0x0C, 0x00, 0x02, 0x00, 0x40, 0x81, 0x00, 0x00, 0x20, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x10,
  0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x26, 0xF0, 0x0B, 0x00, 0x04,
  0x01, 0x00, 0x00, 0x00, 0xB0, 0x09, 0x00, 0xBC, 0x60, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x20, 0x0B, 0x00, 0x98, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x12, 0x07, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xDC, 0xB2, 0x07, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2E, 0x74, 0x65, 0x78, 0x74, 0x00, 0x00, 0x00, 0x4E, 0x2A, 0x06, 0x00, 0x00, 0x10, 0x00,
  0x00, 0x00, 0x2C, 0x06, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x60, 0x2E,
  0x72, 0x64, 0x61, 0x74, 0x61, 0x00, 0x00, 0xF0, 0x6C, 0x02, 0x00, 0x00, 0x40, 0x06, 0x00, 0x00, 0x6E, 0x02, 0x00, 0x00, 0x30, 0x06, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0xC0, 0x2E, 0x64, 0x61, 0x74, 0x61, 0x00, 0x00, 0x00, 0x30, 0xFF, 0x00, 0x00, 0x00,
  0xB0, 0x08, 0x00, 0x00, 0xCC, 0x00, 0x00, 0x00, 0x9E, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00,
  0xC0, 0x2E, 0x72, 0x73, 0x72, 0x63, 0x00, 0x00, 0x00, 0xBC, 0x60, 0x01, 0x00, 0x00, 0xB0, 0x09, 0x00, 0x00, 0x62, 0x01, 0x00, 0x00, 0x6A, 0x09, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x40, 0x2E, 0x72, 0x65, 0x6C, 0x6F, 0x63, 0x00, 0x00, 0x98, 0xC0, 0x00,
  0x00, 0x00, 0x20, 0x0B, 0x00, 0x00, 0xC2, 0x00, 0x00, 0x00, 0xCC, 0x0A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40,
  0x00, 0x00, 0x42, 0x2E, 0x64, 0x73, 0x70, 0x61, 0x74, 0x63, 0x68, 0x00, 0x10, 0x00, 0x00, 0x00, 0xF0, 0x0B, 0x00, 0x3E, 0x01, 0x00, 0x00, 0x00, 0x8E, 0x0B,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };