#pragma once

#include <vector>
#include <cstdint>

namespace spymarine {

const std::vector<uint8_t> raw_device_count_response = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x02, 0x85, 0xde, 0xc3,
    0x46, 0x00, 0x11, 0xff, 0x01, 0x01, 0x00, 0x00, 0x00, 0x1f,
    0xff, 0x02, 0x01, 0x00, 0x00, 0x00, 0x29, 0xff, 0x13, 0x14};

const std::vector<std::vector<uint8_t>> raw_device_info_response = {
    {0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x41, 0x85, 0xde, 0xc3, 0x46, 0x01,
     0x61, 0xff, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0xff, 0x01, 0x03, 0x00,
     0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x0a, 0xff, 0x02, 0x01, 0x00,
     0x00, 0x00, 0x00, 0xff, 0x03, 0x01, 0x85, 0xde, 0xc3, 0x46, 0xff, 0x04,
     0x03, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x01, 0xff, 0x05,
     0x03, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x01, 0xff, 0x06,
     0x03, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x01, 0xff, 0x07,
     0x03, 0x65, 0x0c, 0x5d, 0xae, 0xff, 0x00, 0x00, 0x00, 0x02, 0xff, 0x08,
     0x03, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x01, 0xff, 0x09,
     0x03, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x01, 0xff, 0x0a,
     0x04, 0x00, 0x00, 0x00, 0x00, 0xff, 0x50, 0x69, 0x63, 0x6f, 0x38, 0x37,
     0x31, 0x30, 0x00, 0xff, 0x0b, 0x03, 0x00, 0x00, 0x00, 0x00, 0xff, 0xc0,
     0xa8, 0x01, 0x01, 0xff, 0x0c, 0x03, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00,
     0x00, 0x13, 0x89, 0xff, 0x0d, 0x03, 0x00, 0x00, 0x00, 0x00, 0xff, 0xc0,
     0xa8, 0x01, 0xff, 0xff, 0x0e, 0x03, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00,
     0x00, 0xa8, 0xca, 0xff, 0x0f, 0x04, 0x00, 0x00, 0x00, 0x00, 0xff, 0x70,
     0x69, 0x63, 0x6f, 0x32, 0x32, 0x34, 0x35, 0x00, 0xff, 0x10, 0x03, 0x00,
     0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x01, 0xff, 0x11, 0x03, 0x64,
     0xe2, 0xb5, 0x94, 0xff, 0x00, 0x00, 0x00, 0x64, 0xff, 0x12, 0x03, 0x64,
     0xe3, 0xa0, 0xc9, 0xff, 0x00, 0x00, 0x00, 0x0a, 0xff, 0x13, 0x03, 0x00,
     0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x01, 0xff, 0x14, 0x03, 0x00,
     0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x28, 0xff, 0x15, 0x03, 0x00,
     0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x01, 0xff, 0x16, 0x03, 0x00,
     0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x01, 0xff, 0x17, 0x03, 0x00,
     0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x01, 0xff, 0x18, 0x03, 0x00,
     0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x01, 0xff, 0x19, 0x03, 0x00,
     0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x01, 0xff, 0x1a, 0x01, 0x00,
     0x00, 0x00, 0x00, 0xff, 0x1b, 0x03, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00,
     0x00, 0x00, 0x3c, 0xff, 0x1c, 0x03, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00,
     0x00, 0x00, 0x00, 0xff, 0x1d, 0x03, 0x65, 0x1a, 0x5b, 0xd6, 0xff, 0x00,
     0x00, 0x00, 0x18, 0xff, 0x57, 0xea},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x41, 0x85, 0xde, 0xc3, 0x46,
     0x00, 0x54, 0xff, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0xff, 0x01,
     0x03, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x07, 0xff,
     0x02, 0x01, 0x00, 0x00, 0x00, 0x00, 0xff, 0x03, 0x01, 0x85, 0xde,
     0xc3, 0x46, 0xff, 0x04, 0x03, 0x65, 0x33, 0xcf, 0xb8, 0xff, 0x00,
     0x00, 0x1c, 0x20, 0xff, 0x05, 0x03, 0x3f, 0xce, 0x97, 0x24, 0xff,
     0x00, 0x00, 0x00, 0x00, 0xff, 0x06, 0x03, 0x00, 0x00, 0x00, 0x00,
     0xff, 0x00, 0x00, 0x00, 0xff, 0xff, 0x07, 0x03, 0x00, 0x00, 0x00,
     0x00, 0xff, 0xc9, 0x62, 0xf6, 0x46, 0xff, 0x4f, 0x4f},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x41, 0x85, 0xde, 0xc3, 0x46,
     0x00, 0x6c, 0xff, 0x00, 0x01, 0x00, 0x00, 0x00, 0x02, 0xff, 0x01,
     0x03, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x04, 0xff,
     0x02, 0x01, 0x00, 0x00, 0x00, 0x00, 0xff, 0x03, 0x04, 0x00, 0x00,
     0x00, 0x00, 0xff, 0x53, 0x65, 0x6e, 0x73, 0x6f, 0x72, 0x20, 0x31,
     0x00, 0xff, 0x04, 0x01, 0x85, 0xde, 0xc3, 0x46, 0xff, 0x05, 0x01,
     0x00, 0x00, 0x00, 0x01, 0xff, 0x06, 0x03, 0xd2, 0xad, 0x42, 0x4e,
     0xff, 0x00, 0x00, 0x00, 0x00, 0xff, 0x07, 0x03, 0x69, 0xc8, 0x7c,
     0x55, 0xff, 0x00, 0x00, 0x00, 0x00, 0xff, 0x08, 0x03, 0x00, 0x00,
     0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff, 0xff, 0x09, 0x03, 0x00,
     0x00, 0x00, 0x00, 0xff, 0x69, 0x96, 0x0d, 0x9a, 0xff, 0x1a, 0x82},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x41, 0x85, 0xde, 0xc3, 0x46, 0x00,
     0x16, 0xff, 0x00, 0x01, 0x00, 0x00, 0x00, 0x03, 0xff, 0x01, 0x03, 0x00,
     0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0xff, 0x16, 0x9f},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x41, 0x85, 0xde, 0xc3, 0x46, 0x00,
     0x16, 0xff, 0x00, 0x01, 0x00, 0x00, 0x00, 0x04, 0xff, 0x01, 0x03, 0x00,
     0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0xff, 0x19, 0x5b},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x41, 0x85, 0xde, 0xc3, 0x46, 0x00,
     0x72, 0xff, 0x00, 0x01, 0x00, 0x00, 0x00, 0x05, 0xff, 0x01, 0x03, 0x00,
     0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x05, 0xff, 0x02, 0x01, 0x00,
     0x00, 0x00, 0x00, 0xff, 0x03, 0x04, 0x00, 0x00, 0x00, 0x00, 0xff, 0x42,
     0x61, 0x72, 0x6f, 0x6d, 0x65, 0x74, 0x65, 0x72, 0x00, 0xff, 0x04, 0x01,
     0x85, 0xde, 0xc3, 0x46, 0xff, 0x05, 0x03, 0x00, 0x00, 0x00, 0x00, 0xff,
     0x00, 0x00, 0x00, 0x00, 0xff, 0x06, 0x03, 0x00, 0x00, 0x00, 0x00, 0xff,
     0x00, 0x00, 0x00, 0x01, 0xff, 0x07, 0x03, 0x00, 0x00, 0x00, 0x00, 0xff,
     0x00, 0x00, 0x00, 0xff, 0xff, 0x08, 0x03, 0x00, 0x00, 0x00, 0x00, 0xff,
     0x00, 0x00, 0x00, 0x01, 0xff, 0x09, 0x03, 0x00, 0x00, 0x00, 0x00, 0xff,
     0x6a, 0xe5, 0xc8, 0x5d, 0xff, 0x07, 0xd5},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x41, 0x85, 0xde, 0xc3, 0x46, 0x00,
     0x59, 0xff, 0x00, 0x01, 0x00, 0x00, 0x00, 0x06, 0xff, 0x01, 0x03, 0x00,
     0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x01, 0xff, 0x02, 0x01, 0x00,
     0x00, 0x00, 0x00, 0xff, 0x03, 0x04, 0x00, 0x00, 0x00, 0x00, 0xff, 0x50,
     0x49, 0x43, 0x4f, 0x20, 0x49, 0x4e, 0x54, 0x45, 0x52, 0x4e, 0x41, 0x4c,
     0x00, 0xff, 0x04, 0x01, 0x85, 0xde, 0xc3, 0x46, 0xff, 0x05, 0x01, 0x00,
     0x00, 0x00, 0x01, 0xff, 0x06, 0x03, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00,
     0x00, 0x00, 0xff, 0xff, 0x07, 0x03, 0x00, 0x00, 0x00, 0x00, 0xff, 0x78,
     0x66, 0x4c, 0x81, 0xff, 0x60, 0xcd},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x41, 0x85, 0xde, 0xc3, 0x46, 0x00,
     0x16, 0xff, 0x00, 0x01, 0x00, 0x00, 0x00, 0x07, 0xff, 0x01, 0x03, 0x00,
     0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0xff, 0x74, 0xac},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x41, 0x85, 0xde, 0xc3, 0x46, 0x00,
     0x16, 0xff, 0x00, 0x01, 0x00, 0x00, 0x00, 0x08, 0xff, 0x01, 0x03, 0x00,
     0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0xff, 0xbf, 0x0e},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x41, 0x85, 0xde, 0xc3, 0x46, 0x00,
     0x16, 0xff, 0x00, 0x01, 0x00, 0x00, 0x00, 0x09, 0xff, 0x01, 0x03, 0x00,
     0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0xff, 0x6b, 0x24},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x41, 0x85, 0xde, 0xc3, 0x46, 0x00,
     0x5a, 0xff, 0x00, 0x01, 0x00, 0x00, 0x00, 0x0a, 0xff, 0x01, 0x03, 0x64,
     0x39, 0xb5, 0xea, 0xff, 0x00, 0x00, 0x00, 0x01, 0xff, 0x02, 0x01, 0x00,
     0x00, 0x00, 0x00, 0xff, 0x03, 0x04, 0x64, 0x39, 0xb5, 0xea, 0xff, 0x53,
     0x54, 0x31, 0x30, 0x37, 0x20, 0x5b, 0x35, 0x35, 0x39, 0x36, 0x5d, 0x20,
     0x31, 0x00, 0xff, 0x04, 0x01, 0x08, 0x56, 0x2d, 0xfc, 0xff, 0x05, 0x01,
     0x00, 0x00, 0x00, 0x01, 0xff, 0x06, 0x03, 0x64, 0x39, 0xb5, 0xea, 0xff,
     0x00, 0x00, 0x00, 0x1a, 0xff, 0x07, 0x03, 0x00, 0x00, 0x00, 0x00, 0xff,
     0x5c, 0x20, 0x52, 0x0b, 0xff, 0x21, 0xcb},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x41, 0x85, 0xde, 0xc3, 0x46, 0x00,
     0x5a, 0xff, 0x00, 0x01, 0x00, 0x00, 0x00, 0x0b, 0xff, 0x01, 0x03, 0x64,
     0x39, 0xb5, 0xea, 0xff, 0x00, 0x00, 0x00, 0x01, 0xff, 0x02, 0x01, 0x00,
     0x00, 0x00, 0x00, 0xff, 0x03, 0x04, 0x64, 0x39, 0xb5, 0xea, 0xff, 0x53,
     0x54, 0x31, 0x30, 0x37, 0x20, 0x5b, 0x35, 0x35, 0x39, 0x36, 0x5d, 0x20,
     0x32, 0x00, 0xff, 0x04, 0x01, 0x08, 0x56, 0x2d, 0xfc, 0xff, 0x05, 0x01,
     0x00, 0x00, 0x00, 0x02, 0xff, 0x06, 0x03, 0x64, 0x39, 0xb5, 0xea, 0xff,
     0x00, 0x00, 0x00, 0x1c, 0xff, 0x07, 0x03, 0x00, 0x00, 0x00, 0x00, 0xff,
     0x53, 0x2e, 0xe4, 0x5d, 0xff, 0xe7, 0xeb},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x41, 0x85, 0xde, 0xc3, 0x46, 0x00,
     0x5a, 0xff, 0x00, 0x01, 0x00, 0x00, 0x00, 0x0c, 0xff, 0x01, 0x03, 0x64,
     0x39, 0xb5, 0xea, 0xff, 0x00, 0x00, 0x00, 0x01, 0xff, 0x02, 0x01, 0x00,
     0x00, 0x00, 0x00, 0xff, 0x03, 0x04, 0x64, 0x39, 0xb5, 0xea, 0xff, 0x53,
     0x54, 0x31, 0x30, 0x37, 0x20, 0x5b, 0x35, 0x35, 0x39, 0x36, 0x5d, 0x20,
     0x33, 0x00, 0xff, 0x04, 0x01, 0x08, 0x56, 0x2d, 0xfc, 0xff, 0x05, 0x01,
     0x00, 0x00, 0x00, 0x03, 0xff, 0x06, 0x03, 0x64, 0x39, 0xb5, 0xea, 0xff,
     0x00, 0x00, 0x00, 0xff, 0xff, 0x07, 0x03, 0x00, 0x00, 0x00, 0x00, 0xff,
     0x57, 0x1e, 0xe8, 0x91, 0xff, 0xaa, 0x7a},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x41, 0x85, 0xde, 0xc3, 0x46, 0x00,
     0x66, 0xff, 0x00, 0x01, 0x00, 0x00, 0x00, 0x0d, 0xff, 0x01, 0x03, 0x64,
     0x39, 0xb5, 0xea, 0xff, 0x00, 0x00, 0x00, 0x06, 0xff, 0x02, 0x01, 0x00,
     0x00, 0x00, 0x00, 0xff, 0x03, 0x04, 0x64, 0x39, 0xb5, 0xea, 0xff, 0x53,
     0x54, 0x31, 0x30, 0x37, 0x20, 0x5b, 0x35, 0x35, 0x39, 0x36, 0x5d, 0x20,
     0x31, 0x00, 0xff, 0x04, 0x01, 0x08, 0x56, 0x2d, 0xfc, 0xff, 0x05, 0x01,
     0x00, 0x00, 0x00, 0x01, 0xff, 0x06, 0x03, 0x64, 0x39, 0xb5, 0xea, 0xff,
     0x00, 0x00, 0x00, 0x00, 0xff, 0x07, 0x03, 0x64, 0x39, 0xb5, 0xea, 0xff,
     0x00, 0x00, 0x00, 0x1d, 0xff, 0x08, 0x03, 0x00, 0x00, 0x00, 0x00, 0xff,
     0x39, 0xd8, 0x68, 0x8f, 0xff, 0x71, 0x46},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x41, 0x85, 0xde, 0xc3, 0x46, 0x00,
     0x66, 0xff, 0x00, 0x01, 0x00, 0x00, 0x00, 0x0e, 0xff, 0x01, 0x03, 0x64,
     0x39, 0xb5, 0xea, 0xff, 0x00, 0x00, 0x00, 0x06, 0xff, 0x02, 0x01, 0x00,
     0x00, 0x00, 0x00, 0xff, 0x03, 0x04, 0x64, 0x39, 0xb5, 0xea, 0xff, 0x53,
     0x54, 0x31, 0x30, 0x37, 0x20, 0x5b, 0x35, 0x35, 0x39, 0x36, 0x5d, 0x20,
     0x32, 0x00, 0xff, 0x04, 0x01, 0x08, 0x56, 0x2d, 0xfc, 0xff, 0x05, 0x01,
     0x00, 0x00, 0x00, 0x02, 0xff, 0x06, 0x03, 0x64, 0x39, 0xb5, 0xea, 0xff,
     0x00, 0x00, 0x00, 0x00, 0xff, 0x07, 0x03, 0x64, 0x39, 0xb5, 0xea, 0xff,
     0x00, 0x00, 0x00, 0x1e, 0xff, 0x08, 0x03, 0x00, 0x00, 0x00, 0x00, 0xff,
     0xdc, 0xc7, 0x9f, 0x4e, 0xff, 0x6d, 0x3c},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x41, 0x85, 0xde, 0xc3, 0x46, 0x00,
     0x66, 0xff, 0x00, 0x01, 0x00, 0x00, 0x00, 0x0f, 0xff, 0x01, 0x03, 0x64,
     0x39, 0xb5, 0xea, 0xff, 0x00, 0x00, 0x00, 0x06, 0xff, 0x02, 0x01, 0x00,
     0x00, 0x00, 0x00, 0xff, 0x03, 0x04, 0x64, 0x39, 0xb5, 0xea, 0xff, 0x53,
     0x54, 0x31, 0x30, 0x37, 0x20, 0x5b, 0x35, 0x35, 0x39, 0x36, 0x5d, 0x20,
     0x33, 0x00, 0xff, 0x04, 0x01, 0x08, 0x56, 0x2d, 0xfc, 0xff, 0x05, 0x01,
     0x00, 0x00, 0x00, 0x03, 0xff, 0x06, 0x03, 0x64, 0x39, 0xb5, 0xea, 0xff,
     0x00, 0x00, 0x00, 0x00, 0xff, 0x07, 0x03, 0x64, 0x39, 0xb5, 0xea, 0xff,
     0x00, 0x00, 0x00, 0x1f, 0xff, 0x08, 0x03, 0x00, 0x00, 0x00, 0x00, 0xff,
     0x9d, 0x5b, 0x04, 0x6f, 0xff, 0xcb, 0x31},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x41, 0x85, 0xde, 0xc3, 0x46, 0x00,
     0x66, 0xff, 0x00, 0x01, 0x00, 0x00, 0x00, 0x10, 0xff, 0x01, 0x03, 0x64,
     0x39, 0xb5, 0xea, 0xff, 0x00, 0x00, 0x00, 0x06, 0xff, 0x02, 0x01, 0x00,
     0x00, 0x00, 0x00, 0xff, 0x03, 0x04, 0x64, 0x39, 0xb5, 0xea, 0xff, 0x53,
     0x54, 0x31, 0x30, 0x37, 0x20, 0x5b, 0x35, 0x35, 0x39, 0x36, 0x5d, 0x20,
     0x34, 0x00, 0xff, 0x04, 0x01, 0x08, 0x56, 0x2d, 0xfc, 0xff, 0x05, 0x01,
     0x00, 0x00, 0x00, 0x04, 0xff, 0x06, 0x03, 0x64, 0x39, 0xb5, 0xea, 0xff,
     0x00, 0x00, 0x00, 0x00, 0xff, 0x07, 0x03, 0x64, 0x39, 0xb5, 0xea, 0xff,
     0x00, 0x00, 0x00, 0xff, 0xff, 0x08, 0x03, 0x00, 0x00, 0x00, 0x00, 0xff,
     0xf4, 0xd1, 0x7f, 0x4a, 0xff, 0xc0, 0xd2},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x41, 0x85, 0xde, 0xc3, 0x46, 0x00,
     0x58, 0xff, 0x00, 0x01, 0x00, 0x00, 0x00, 0x11, 0xff, 0x01, 0x03, 0x64,
     0x39, 0xb5, 0xea, 0xff, 0x00, 0x00, 0x00, 0x0e, 0xff, 0x02, 0x01, 0x00,
     0x00, 0x00, 0x00, 0xff, 0x03, 0x04, 0x64, 0x39, 0xb5, 0xea, 0xff, 0x53,
     0x54, 0x31, 0x30, 0x37, 0x20, 0x5b, 0x35, 0x35, 0x39, 0x36, 0x5d, 0x00,
     0xff, 0x04, 0x01, 0x08, 0x56, 0x2d, 0xfc, 0xff, 0x05, 0x01, 0x00, 0x00,
     0x00, 0x01, 0xff, 0x06, 0x03, 0x64, 0x39, 0xb5, 0xea, 0xff, 0x00, 0x00,
     0x00, 0xff, 0xff, 0x07, 0x03, 0x00, 0x00, 0x00, 0x00, 0xff, 0x49, 0xa6,
     0x97, 0xa3, 0xff, 0x42, 0xb4},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x41, 0x85, 0xde, 0xc3, 0x46, 0x00,
     0xac, 0xff, 0x00, 0x01, 0x00, 0x00, 0x00, 0x12, 0xff, 0x01, 0x03, 0x64,
     0x72, 0x39, 0xbd, 0xff, 0x00, 0x00, 0x00, 0x02, 0xff, 0x02, 0x01, 0x00,
     0x00, 0x00, 0x00, 0xff, 0x03, 0x04, 0x64, 0x72, 0x39, 0xbd, 0xff, 0x53,
     0x43, 0x33, 0x30, 0x33, 0x20, 0x5b, 0x35, 0x34, 0x39, 0x39, 0x5d, 0x00,
     0xff, 0x04, 0x01, 0x2a, 0x1b, 0xce, 0xdb, 0xff, 0x05, 0x01, 0x00, 0x00,
     0x00, 0x01, 0xff, 0x06, 0x03, 0x64, 0x72, 0x39, 0xbd, 0xff, 0x00, 0x00,
     0x00, 0x64, 0xff, 0x07, 0x03, 0x64, 0x72, 0x39, 0xbd, 0xff, 0x00, 0x00,
     0x00, 0x02, 0xff, 0x08, 0x03, 0x64, 0x72, 0x39, 0xbd, 0xff, 0x00, 0x00,
     0x00, 0x01, 0xff, 0x09, 0x03, 0x64, 0x72, 0x39, 0xbd, 0xff, 0x00, 0x00,
     0x00, 0x01, 0xff, 0x0a, 0x03, 0x64, 0x72, 0x3e, 0x77, 0xff, 0x00, 0x00,
     0x00, 0x18, 0xff, 0x0b, 0x03, 0x64, 0x72, 0x39, 0xbd, 0xff, 0x00, 0x00,
     0x00, 0x01, 0xff, 0x0c, 0x03, 0x00, 0x00, 0x00, 0x00, 0xff, 0xaf, 0xa3,
     0x75, 0x2b, 0xff, 0x0d, 0x03, 0x64, 0x72, 0x39, 0xbd, 0xff, 0x00, 0x00,
     0x00, 0x7f, 0xff, 0x0e, 0x03, 0x64, 0x72, 0x39, 0xbd, 0xff, 0x00, 0x00,
     0x00, 0x02, 0xff, 0x9e, 0x3a},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x41, 0x85, 0xde, 0xc3, 0x46, 0x00,
     0x5a, 0xff, 0x00, 0x01, 0x00, 0x00, 0x00, 0x13, 0xff, 0x01, 0x03, 0x64,
     0x72, 0x39, 0xbd, 0xff, 0x00, 0x00, 0x00, 0x01, 0xff, 0x02, 0x01, 0x00,
     0x00, 0x00, 0x00, 0xff, 0x03, 0x04, 0x64, 0x72, 0x39, 0xbd, 0xff, 0x53,
     0x43, 0x33, 0x30, 0x33, 0x20, 0x5b, 0x35, 0x34, 0x39, 0x39, 0x5d, 0x20,
     0x31, 0x00, 0xff, 0x04, 0x01, 0x2a, 0x1b, 0xce, 0xdb, 0xff, 0x05, 0x01,
     0x00, 0x00, 0x00, 0x01, 0xff, 0x06, 0x03, 0x64, 0x72, 0x3e, 0x77, 0xff,
     0x00, 0x00, 0x00, 0x18, 0xff, 0x07, 0x03, 0x00, 0x00, 0x00, 0x00, 0xff,
     0xe3, 0x6d, 0xa2, 0xc2, 0xff, 0x98, 0xe1},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x41, 0x85, 0xde, 0xc3, 0x46, 0x00,
     0x5a, 0xff, 0x00, 0x01, 0x00, 0x00, 0x00, 0x14, 0xff, 0x01, 0x03, 0x64,
     0x72, 0x39, 0xbd, 0xff, 0x00, 0x00, 0x00, 0x01, 0xff, 0x02, 0x01, 0x00,
     0x00, 0x00, 0x00, 0xff, 0x03, 0x04, 0x64, 0x72, 0x39, 0xbd, 0xff, 0x53,
     0x43, 0x33, 0x30, 0x33, 0x20, 0x5b, 0x35, 0x34, 0x39, 0x39, 0x5d, 0x20,
     0x32, 0x00, 0xff, 0x04, 0x01, 0x2a, 0x1b, 0xce, 0xdb, 0xff, 0x05, 0x01,
     0x00, 0x00, 0x00, 0x02, 0xff, 0x06, 0x03, 0x64, 0x77, 0x7a, 0xd0, 0xff,
     0x00, 0x00, 0x00, 0x1b, 0xff, 0x07, 0x03, 0x00, 0x00, 0x00, 0x00, 0xff,
     0x92, 0xcd, 0x27, 0x18, 0xff, 0x42, 0x57},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x41, 0x85, 0xde, 0xc3, 0x46, 0x00,
     0x66, 0xff, 0x00, 0x01, 0x00, 0x00, 0x00, 0x15, 0xff, 0x01, 0x03, 0x64,
     0x72, 0x39, 0xbd, 0xff, 0x00, 0x00, 0x00, 0x06, 0xff, 0x02, 0x01, 0x00,
     0x00, 0x00, 0x00, 0xff, 0x03, 0x04, 0x64, 0x72, 0x39, 0xbd, 0xff, 0x53,
     0x43, 0x33, 0x30, 0x33, 0x20, 0x5b, 0x35, 0x34, 0x39, 0x39, 0x5d, 0x20,
     0x31, 0x00, 0xff, 0x04, 0x01, 0x2a, 0x1b, 0xce, 0xdb, 0xff, 0x05, 0x01,
     0x00, 0x00, 0x00, 0x01, 0xff, 0x06, 0x03, 0x64, 0x72, 0x39, 0xbd, 0xff,
     0x00, 0x00, 0x00, 0x00, 0xff, 0x07, 0x03, 0x64, 0x72, 0x39, 0xbd, 0xff,
     0x00, 0x00, 0x00, 0xff, 0xff, 0x08, 0x03, 0x00, 0x00, 0x00, 0x00, 0xff,
     0x19, 0x54, 0x5d, 0x50, 0xff, 0xe1, 0x12},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x41, 0x85, 0xde, 0xc3, 0x46, 0x00,
     0x66, 0xff, 0x00, 0x01, 0x00, 0x00, 0x00, 0x16, 0xff, 0x01, 0x03, 0x64,
     0x72, 0x39, 0xbd, 0xff, 0x00, 0x00, 0x00, 0x06, 0xff, 0x02, 0x01, 0x00,
     0x00, 0x00, 0x00, 0xff, 0x03, 0x04, 0x64, 0x72, 0x39, 0xbd, 0xff, 0x53,
     0x43, 0x33, 0x30, 0x33, 0x20, 0x5b, 0x35, 0x34, 0x39, 0x39, 0x5d, 0x20,
     0x32, 0x00, 0xff, 0x04, 0x01, 0x2a, 0x1b, 0xce, 0xdb, 0xff, 0x05, 0x01,
     0x00, 0x00, 0x00, 0x02, 0xff, 0x06, 0x03, 0x64, 0x72, 0x39, 0xbd, 0xff,
     0x00, 0x00, 0x00, 0x00, 0xff, 0x07, 0x03, 0x64, 0x72, 0x39, 0xbd, 0xff,
     0x00, 0x00, 0x00, 0xff, 0xff, 0x08, 0x03, 0x00, 0x00, 0x00, 0x00, 0xff,
     0x4d, 0x19, 0x42, 0xef, 0xff, 0xdb, 0xc7},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x41, 0x85, 0xde, 0xc3, 0x46, 0x00,
     0x66, 0xff, 0x00, 0x01, 0x00, 0x00, 0x00, 0x17, 0xff, 0x01, 0x03, 0x64,
     0x72, 0x39, 0xbd, 0xff, 0x00, 0x00, 0x00, 0x06, 0xff, 0x02, 0x01, 0x00,
     0x00, 0x00, 0x00, 0xff, 0x03, 0x04, 0x64, 0x72, 0x39, 0xbd, 0xff, 0x53,
     0x43, 0x33, 0x30, 0x33, 0x20, 0x5b, 0x35, 0x34, 0x39, 0x39, 0x5d, 0x20,
     0x33, 0x00, 0xff, 0x04, 0x01, 0x2a, 0x1b, 0xce, 0xdb, 0xff, 0x05, 0x01,
     0x00, 0x00, 0x00, 0x03, 0xff, 0x06, 0x03, 0x64, 0x72, 0x39, 0xbd, 0xff,
     0x00, 0x00, 0x00, 0x00, 0xff, 0x07, 0x03, 0x64, 0x72, 0x39, 0xbd, 0xff,
     0x00, 0x00, 0x00, 0x19, 0xff, 0x08, 0x03, 0x00, 0x00, 0x00, 0x00, 0xff,
     0x12, 0x7e, 0x7a, 0x38, 0xff, 0x45, 0xa5},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x41, 0x85, 0xde, 0xc3, 0x46, 0x01,
     0x06, 0xff, 0x00, 0x01, 0x00, 0x00, 0x00, 0x18, 0xff, 0x01, 0x03, 0x64,
     0x72, 0x3e, 0x77, 0xff, 0x00, 0x00, 0x00, 0x09, 0xff, 0x02, 0x01, 0x00,
     0x00, 0x00, 0x00, 0xff, 0x03, 0x04, 0x64, 0x72, 0x3e, 0x77, 0xff, 0x42,
     0x75, 0x6c, 0x6c, 0x74, 0x72, 0x6f, 0x6e, 0x00, 0xff, 0x04, 0x03, 0x00,
     0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x13, 0xff, 0x05, 0x03, 0x64,
     0x72, 0x3e, 0x77, 0xff, 0x00, 0x00, 0x75, 0x30, 0xff, 0x06, 0x03, 0x64,
     0x72, 0x3e, 0x77, 0xff, 0xff, 0xff, 0xfc, 0x18, 0xff, 0x07, 0x03, 0x64,
     0x72, 0x3e, 0x77, 0xff, 0xff, 0xff, 0xfc, 0x18, 0xff, 0x08, 0x03, 0x64,
     0x72, 0x3e, 0x77, 0xff, 0x00, 0x00, 0x00, 0x06, 0xff, 0x09, 0x03, 0x64,
     0x72, 0x3e, 0x77, 0xff, 0x00, 0x00, 0x00, 0x7d, 0xff, 0x0a, 0x03, 0x64,
     0x77, 0x7a, 0x8b, 0xff, 0x00, 0x00, 0x00, 0x19, 0xff, 0x0b, 0x03, 0x64,
     0x72, 0x3e, 0x77, 0xff, 0x00, 0x00, 0x00, 0xff, 0xff, 0x0c, 0x03, 0x00,
     0x00, 0x00, 0x00, 0xff, 0x48, 0x22, 0x6a, 0x52, 0xff, 0x0d, 0x03, 0x64,
     0x72, 0x3e, 0x77, 0xff, 0x00, 0x00, 0x00, 0x02, 0xff, 0x0e, 0x03, 0x64,
     0x72, 0x3e, 0x77, 0xff, 0x00, 0x00, 0x00, 0xc8, 0xff, 0x0f, 0x03, 0x64,
     0x72, 0x3e, 0x77, 0xff, 0x00, 0x00, 0x03, 0xe8, 0xff, 0x10, 0x03, 0x64,
     0x72, 0x3e, 0x77, 0xff, 0x00, 0x00, 0x00, 0x01, 0xff, 0x11, 0x03, 0x64,
     0x72, 0x3e, 0x77, 0xff, 0x00, 0x00, 0x27, 0x10, 0xff, 0x12, 0x03, 0x64,
     0x72, 0x3e, 0x77, 0xff, 0x00, 0x00, 0x00, 0x01, 0xff, 0x13, 0x03, 0x64,
     0x72, 0x3e, 0x77, 0xff, 0x00, 0x00, 0x00, 0x73, 0xff, 0x14, 0x03, 0x64,
     0x72, 0x3e, 0x77, 0xff, 0x00, 0x00, 0x00, 0x0a, 0xff, 0x15, 0x03, 0x64,
     0x72, 0x3e, 0x77, 0xff, 0x00, 0x00, 0x01, 0x2c, 0xff, 0x48, 0xa2},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x41, 0x85, 0xde, 0xc3, 0x46, 0x00,
     0xa8, 0xff, 0x00, 0x01, 0x00, 0x00, 0x00, 0x19, 0xff, 0x01, 0x03, 0x64,
     0x72, 0x3f, 0x6a, 0xff, 0x00, 0x00, 0x00, 0x03, 0xff, 0x02, 0x01, 0x00,
     0x00, 0x00, 0x00, 0xff, 0x03, 0x04, 0x64, 0x7b, 0x3f, 0x64, 0xff, 0x42,
     0x61, 0x74, 0x74, 0x65, 0x72, 0x69, 0x65, 0x00, 0xff, 0x04, 0x01, 0x00,
     0x00, 0x00, 0x00, 0xff, 0x05, 0x01, 0x00, 0x00, 0x00, 0x00, 0xff, 0x06,
     0x03, 0x64, 0x72, 0x3f, 0x7e, 0xff, 0x00, 0x00, 0x00, 0x01, 0xff, 0x07,
     0x03, 0x64, 0x72, 0x3f, 0x6a, 0xff, 0x00, 0x00, 0x00, 0x00, 0xff, 0x08,
     0x03, 0x64, 0x72, 0x3f, 0x6a, 0xff, 0x00, 0x00, 0x00, 0xff, 0xff, 0x09,
     0x03, 0x64, 0x72, 0x3f, 0x6a, 0xff, 0x00, 0x00, 0x00, 0x01, 0xff, 0x0a,
     0x03, 0x64, 0x72, 0x3f, 0x6a, 0xff, 0x00, 0x00, 0x00, 0x00, 0xff, 0x0b,
     0x03, 0x64, 0x72, 0x3f, 0x6a, 0xff, 0x00, 0x00, 0x00, 0x00, 0xff, 0x0c,
     0x03, 0x00, 0x00, 0x00, 0x00, 0xff, 0x4b, 0xeb, 0xdc, 0xb2, 0xff, 0x0d,
     0x03, 0x64, 0x72, 0x3f, 0x6a, 0xff, 0x00, 0x00, 0x00, 0x01, 0xff, 0x0e,
     0x03, 0x64, 0x72, 0x3f, 0x6a, 0xff, 0x00, 0x00, 0x00, 0x00, 0xff, 0xc1,
     0x03},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x41, 0x85, 0xde, 0xc3, 0x46, 0x01,
     0x18, 0xff, 0x00, 0x01, 0x00, 0x00, 0x00, 0x1a, 0xff, 0x01, 0x03, 0x64,
     0x73, 0x5c, 0x09, 0xff, 0x00, 0x00, 0x00, 0x08, 0xff, 0x02, 0x01, 0x00,
     0x00, 0x00, 0x00, 0xff, 0x03, 0x04, 0x64, 0x73, 0x5c, 0x09, 0xff, 0x46,
     0x72, 0x69, 0x73, 0x63, 0x68, 0x77, 0x61, 0x73, 0x73, 0x65, 0x72, 0x00,
     0xff, 0x04, 0x01, 0x36, 0xe8, 0x50, 0x6d, 0xff, 0x05, 0x01, 0x00, 0x00,
     0x00, 0x00, 0xff, 0x06, 0x03, 0x64, 0x73, 0x5c, 0x09, 0xff, 0x00, 0x00,
     0x00, 0x01, 0xff, 0x07, 0x03, 0x64, 0x73, 0x5c, 0x09, 0xff, 0x00, 0x00,
     0x03, 0xe8, 0xff, 0x08, 0x03, 0x64, 0x73, 0x5c, 0x09, 0xff, 0x00, 0x00,
     0x00, 0x02, 0xff, 0x09, 0x03, 0x64, 0x81, 0xac, 0x12, 0xff, 0x00, 0x00,
     0x00, 0x21, 0xff, 0x0a, 0x03, 0x64, 0x81, 0xac, 0x12, 0xff, 0x00, 0x64,
     0x02, 0x0a, 0xff, 0x0b, 0x03, 0x64, 0x81, 0xac, 0x12, 0xff, 0x00, 0xc8,
     0x03, 0x36, 0xff, 0x0c, 0x03, 0x64, 0x81, 0xaf, 0x0a, 0xff, 0x01, 0x2c,
     0x03, 0x84, 0xff, 0x0d, 0x03, 0x64, 0x81, 0xaf, 0x0a, 0xff, 0x01, 0x90,
     0x03, 0xf4, 0xff, 0x0e, 0x03, 0x64, 0x81, 0xaf, 0x0a, 0xff, 0x01, 0xf4,
     0x04, 0x42, 0xff, 0x0f, 0x03, 0x64, 0x81, 0xaf, 0x0a, 0xff, 0x02, 0x58,
     0x04, 0x88, 0xff, 0x10, 0x03, 0x64, 0x81, 0xaf, 0x0a, 0xff, 0x02, 0xbc,
     0x04, 0xe3, 0xff, 0x11, 0x03, 0x64, 0x81, 0xaf, 0x43, 0xff, 0x03, 0x52,
     0x05, 0x47, 0xff, 0x12, 0x03, 0x64, 0x81, 0xaf, 0xce, 0xff, 0x03, 0xe8,
     0x05, 0xae, 0xff, 0x13, 0x03, 0x64, 0x73, 0x5c, 0x09, 0xff, 0x7f, 0xff,
     0xff, 0xff, 0xff, 0x14, 0x03, 0x64, 0x73, 0x5c, 0x09, 0xff, 0x00, 0x00,
     0x00, 0xff, 0xff, 0x15, 0x03, 0x64, 0x73, 0x5c, 0x09, 0xff, 0x00, 0x00,
     0x00, 0x01, 0xff, 0x16, 0x03, 0x00, 0x00, 0x00, 0x00, 0xff, 0x6d, 0xfe,
     0xe7, 0xb0, 0xff, 0x17, 0x03, 0x64, 0x73, 0x5c, 0x09, 0xff, 0x00, 0x00,
     0x00, 0x01, 0xff, 0xb6, 0x10},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x41, 0x85, 0xde, 0xc3, 0x46, 0x01,
     0x0d, 0xff, 0x00, 0x01, 0x00, 0x00, 0x00, 0x1b, 0xff, 0x01, 0x03, 0x64,
     0x77, 0x7a, 0xd0, 0xff, 0x00, 0x00, 0x00, 0x09, 0xff, 0x02, 0x01, 0x00,
     0x00, 0x00, 0x00, 0xff, 0x03, 0x04, 0x64, 0x77, 0x7a, 0xd0, 0xff, 0x53,
     0x74, 0x61, 0x72, 0x74, 0x65, 0x72, 0x62, 0x61, 0x74, 0x74, 0x65, 0x72,
     0x69, 0x65, 0x00, 0xff, 0x04, 0x03, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00,
     0x00, 0x00, 0x14, 0xff, 0x05, 0x03, 0x64, 0x77, 0x7a, 0xd0, 0xff, 0x00,
     0x00, 0x27, 0x10, 0xff, 0x06, 0x03, 0x64, 0x77, 0x7a, 0xd0, 0xff, 0xff,
     0xff, 0xfc, 0x18, 0xff, 0x07, 0x03, 0x64, 0x77, 0x7a, 0xd0, 0xff, 0xff,
     0xff, 0xfc, 0x18, 0xff, 0x08, 0x03, 0x64, 0x77, 0x7a, 0xd0, 0xff, 0x00,
     0x00, 0x00, 0x03, 0xff, 0x09, 0x03, 0x64, 0x77, 0x7a, 0xd0, 0xff, 0x00,
     0x00, 0x00, 0x7d, 0xff, 0x0a, 0x03, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00,
     0x00, 0x00, 0xff, 0xff, 0x0b, 0x03, 0x64, 0x77, 0x7a, 0xd0, 0xff, 0x00,
     0x00, 0x00, 0xff, 0xff, 0x0c, 0x03, 0x00, 0x00, 0x00, 0x00, 0xff, 0x22,
     0x94, 0x91, 0xfb, 0xff, 0x0d, 0x03, 0x64, 0x77, 0x7a, 0xd0, 0xff, 0x00,
     0x00, 0x00, 0x02, 0xff, 0x0e, 0x03, 0x64, 0x77, 0x7a, 0xd0, 0xff, 0x00,
     0x00, 0x00, 0xc8, 0xff, 0x0f, 0x03, 0x64, 0x77, 0x7a, 0xd0, 0xff, 0x00,
     0x00, 0x03, 0xe8, 0xff, 0x10, 0x03, 0x64, 0x77, 0x7a, 0xd0, 0xff, 0x00,
     0x00, 0x00, 0x01, 0xff, 0x11, 0x03, 0x64, 0x77, 0x7a, 0xd0, 0xff, 0x00,
     0x00, 0x27, 0x10, 0xff, 0x12, 0x03, 0x64, 0x77, 0x7a, 0xd0, 0xff, 0x00,
     0x00, 0x00, 0x01, 0xff, 0x13, 0x03, 0x64, 0x77, 0x7a, 0xd0, 0xff, 0x00,
     0x00, 0x00, 0x73, 0xff, 0x14, 0x03, 0x64, 0x77, 0x7a, 0xd0, 0xff, 0x00,
     0x00, 0x00, 0x0a, 0xff, 0x15, 0x03, 0x64, 0x77, 0x7a, 0xd0, 0xff, 0x00,
     0x00, 0x01, 0x2c, 0xff, 0x53, 0xb4},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x41, 0x85, 0xde, 0xc3, 0x46, 0x01,
     0x14, 0xff, 0x00, 0x01, 0x00, 0x00, 0x00, 0x1c, 0xff, 0x01, 0x03, 0x64,
     0x78, 0x6d, 0x87, 0xff, 0x00, 0x00, 0x00, 0x08, 0xff, 0x02, 0x01, 0x00,
     0x00, 0x00, 0x00, 0xff, 0x03, 0x04, 0x64, 0x78, 0x6d, 0x87, 0xff, 0x41,
     0x62, 0x77, 0x61, 0x73, 0x73, 0x65, 0x72, 0x00, 0xff, 0x04, 0x01, 0x46,
     0x2a, 0xcd, 0x7d, 0xff, 0x05, 0x01, 0x00, 0x00, 0x00, 0x00, 0xff, 0x06,
     0x03, 0x64, 0x78, 0x6d, 0x87, 0xff, 0x00, 0x00, 0x00, 0x03, 0xff, 0x07,
     0x03, 0x64, 0x81, 0xc1, 0x07, 0xff, 0x00, 0x00, 0x02, 0xbc, 0xff, 0x08,
     0x03, 0x64, 0x78, 0x6d, 0x87, 0xff, 0x00, 0x00, 0x00, 0x02, 0xff, 0x09,
     0x03, 0x64, 0x81, 0xb2, 0x9c, 0xff, 0x00, 0x46, 0x00, 0x5b, 0xff, 0x0a,
     0x03, 0x64, 0x81, 0xb3, 0x70, 0xff, 0x00, 0x8c, 0x00, 0xd4, 0xff, 0x0b,
     0x03, 0x64, 0x81, 0xbc, 0x3b, 0xff, 0x00, 0xd2, 0x01, 0x85, 0xff, 0x0c,
     0x03, 0x64, 0x81, 0xbc, 0xf7, 0xff, 0x01, 0x18, 0x02, 0x2f, 0xff, 0x0d,
     0x03, 0x64, 0x81, 0xbd, 0xbe, 0xff, 0x01, 0x5e, 0x02, 0xce, 0xff, 0x0e,
     0x03, 0x64, 0x81, 0xbe, 0x66, 0xff, 0x01, 0xa4, 0x03, 0x7b, 0xff, 0x0f,
     0x03, 0x64, 0x81, 0xbe, 0xfd, 0xff, 0x01, 0xea, 0x04, 0x01, 0xff, 0x10,
     0x03, 0x64, 0x81, 0xbf, 0x9f, 0xff, 0x02, 0x30, 0x04, 0xc5, 0xff, 0x11,
     0x03, 0x64, 0x81, 0xc0, 0x5a, 0xff, 0x02, 0x76, 0x06, 0x2f, 0xff, 0x12,
     0x03, 0x64, 0x81, 0xc1, 0x01, 0xff, 0x02, 0xbc, 0x07, 0x8b, 0xff, 0x13,
     0x03, 0x64, 0x78, 0x6d, 0x87, 0xff, 0x7f, 0xff, 0xff, 0xff, 0xff, 0x14,
     0x03, 0x64, 0x78, 0x6d, 0x87, 0xff, 0x00, 0x00, 0x00, 0xff, 0xff, 0x15,
     0x03, 0x64, 0x78, 0x6d, 0x87, 0xff, 0x00, 0x00, 0x00, 0x01, 0xff, 0x16,
     0x03, 0x00, 0x00, 0x00, 0x00, 0xff, 0x11, 0x19, 0x50, 0x40, 0xff, 0x17,
     0x03, 0x64, 0x78, 0x6d, 0x87, 0xff, 0x00, 0x00, 0x00, 0x01, 0xff, 0xa8,
     0x20},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x41, 0x85, 0xde, 0xc3, 0x46, 0x00,
     0xa5, 0xff, 0x00, 0x01, 0x00, 0x00, 0x00, 0x1d, 0xff, 0x01, 0x03, 0x64,
     0x79, 0xd9, 0xc7, 0xff, 0x00, 0x00, 0x00, 0x03, 0xff, 0x02, 0x01, 0x00,
     0x00, 0x00, 0x00, 0xff, 0x03, 0x04, 0x64, 0x79, 0xe1, 0xb2, 0xff, 0x49,
     0x6e, 0x6e, 0x65, 0x6e, 0x00, 0xff, 0x04, 0x01, 0x00, 0x00, 0x00, 0x00,
     0xff, 0x05, 0x01, 0x00, 0x00, 0x00, 0x00, 0xff, 0x06, 0x03, 0x64, 0x79,
     0xd9, 0xc7, 0xff, 0x00, 0x00, 0x00, 0x01, 0xff, 0x07, 0x03, 0x64, 0x79,
     0xd9, 0xc7, 0xff, 0x00, 0x00, 0x00, 0x00, 0xff, 0x08, 0x03, 0x64, 0x79,
     0xd9, 0xc7, 0xff, 0x00, 0x00, 0x00, 0xff, 0xff, 0x09, 0x03, 0x64, 0x79,
     0xd9, 0xc7, 0xff, 0x00, 0x00, 0x00, 0x01, 0xff, 0x0a, 0x03, 0x64, 0x79,
     0xd9, 0xc7, 0xff, 0x00, 0x00, 0x00, 0x00, 0xff, 0x0b, 0x03, 0x64, 0x79,
     0xd9, 0xc7, 0xff, 0x00, 0x00, 0x00, 0x00, 0xff, 0x0c, 0x03, 0x00, 0x00,
     0x00, 0x00, 0xff, 0x39, 0xf3, 0xee, 0xbc, 0xff, 0x0d, 0x03, 0x64, 0x79,
     0xd9, 0xc7, 0xff, 0x00, 0x00, 0x00, 0x03, 0xff, 0x0e, 0x03, 0x64, 0x79,
     0xd9, 0xc7, 0xff, 0x00, 0x00, 0x00, 0x00, 0xff, 0x22, 0xff},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x41, 0x85, 0xde, 0xc3, 0x46, 0x00,
     0xa6, 0xff, 0x00, 0x01, 0x00, 0x00, 0x00, 0x1e, 0xff, 0x01, 0x03, 0x64,
     0x79, 0xe1, 0xc6, 0xff, 0x00, 0x00, 0x00, 0x03, 0xff, 0x02, 0x01, 0x00,
     0x00, 0x00, 0x00, 0xff, 0x03, 0x04, 0x64, 0x79, 0xe1, 0xc6, 0xff, 0x41,
     0x75, 0xa3, 0x65, 0x6e, 0x20, 0x00, 0xff, 0x04, 0x01, 0x00, 0x00, 0x00,
     0x00, 0xff, 0x05, 0x01, 0x00, 0x00, 0x00, 0x00, 0xff, 0x06, 0x03, 0x64,
     0x79, 0xe1, 0xc6, 0xff, 0x00, 0x00, 0x00, 0x01, 0xff, 0x07, 0x03, 0x64,
     0x79, 0xe1, 0xc6, 0xff, 0x00, 0x00, 0x00, 0x00, 0xff, 0x08, 0x03, 0x64,
     0x79, 0xe1, 0xc6, 0xff, 0x00, 0x00, 0x00, 0xff, 0xff, 0x09, 0x03, 0x64,
     0x79, 0xe1, 0xc6, 0xff, 0x00, 0x00, 0x00, 0x01, 0xff, 0x0a, 0x03, 0x64,
     0x79, 0xe1, 0xc6, 0xff, 0x00, 0x00, 0x00, 0x00, 0xff, 0x0b, 0x03, 0x64,
     0x79, 0xe1, 0xc6, 0xff, 0x00, 0x00, 0x00, 0x00, 0xff, 0x0c, 0x03, 0x00,
     0x00, 0x00, 0x00, 0xff, 0x40, 0xcd, 0xf9, 0xee, 0xff, 0x0d, 0x03, 0x64,
     0x79, 0xe1, 0xc6, 0xff, 0x00, 0x00, 0x00, 0x02, 0xff, 0x0e, 0x03, 0x64,
     0x79, 0xe1, 0xc6, 0xff, 0x00, 0x00, 0x00, 0x00, 0xff, 0x07, 0x0c},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x41, 0x85, 0xde, 0xc3, 0x46, 0x00,
     0xa6, 0xff, 0x00, 0x01, 0x00, 0x00, 0x00, 0x1f, 0xff, 0x01, 0x03, 0x64,
     0x7a, 0xf8, 0xbf, 0xff, 0x00, 0x00, 0x00, 0x03, 0xff, 0x02, 0x01, 0x00,
     0x00, 0x00, 0x00, 0xff, 0x03, 0x04, 0x64, 0x7a, 0xf8, 0xbf, 0xff, 0x42,
     0x6f, 0x69, 0x6c, 0x65, 0x72, 0x00, 0xff, 0x04, 0x01, 0x00, 0x00, 0x00,
     0x00, 0xff, 0x05, 0x01, 0x00, 0x00, 0x00, 0x00, 0xff, 0x06, 0x03, 0x64,
     0x7a, 0xf8, 0xbf, 0xff, 0x00, 0x00, 0x00, 0x01, 0xff, 0x07, 0x03, 0x64,
     0x7a, 0xf8, 0xbf, 0xff, 0x00, 0x00, 0x00, 0x00, 0xff, 0x08, 0x03, 0x64,
     0x7a, 0xf8, 0xbf, 0xff, 0x00, 0x00, 0x00, 0xff, 0xff, 0x09, 0x03, 0x64,
     0x7a, 0xf8, 0xbf, 0xff, 0x00, 0x00, 0x00, 0x01, 0xff, 0x0a, 0x03, 0x64,
     0x7a, 0xf8, 0xbf, 0xff, 0x00, 0x00, 0x00, 0x00, 0xff, 0x0b, 0x03, 0x64,
     0x7a, 0xf8, 0xbf, 0xff, 0x00, 0x00, 0x00, 0x00, 0xff, 0x0c, 0x03, 0x00,
     0x00, 0x00, 0x00, 0xff, 0x76, 0x6d, 0xe3, 0x7e, 0xff, 0x0d, 0x03, 0x64,
     0x7a, 0xf8, 0xbf, 0xff, 0x00, 0x00, 0x00, 0x07, 0xff, 0x0e, 0x03, 0x64,
     0x7a, 0xf8, 0xbf, 0xff, 0x00, 0x00, 0x00, 0x00, 0xff, 0x79, 0x25},
};

const std::vector<uint8_t> raw_state_response = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xb0, 0x85, 0xde, 0xc3, 0x46, 0x01,
    0x14, 0xff, 0x00, 0x01, 0x65, 0x46, 0x86, 0xea, 0xff, 0x01, 0x01, 0x65,
    0x46, 0xa2, 0xdf, 0xff, 0x02, 0x01, 0xff, 0xff, 0xfc, 0x16, 0xff, 0x03,
    0x01, 0x00, 0x01, 0x7e, 0xbf, 0xff, 0x04, 0x01, 0xff, 0xff, 0xff, 0xb9,
    0xff, 0x05, 0x01, 0x00, 0x00, 0x33, 0xcc, 0xff, 0x09, 0x01, 0x65, 0x1a,
    0x5b, 0xd6, 0xff, 0x0a, 0x01, 0xff, 0xff, 0xfc, 0x16, 0xff, 0x0b, 0x01,
    0x00, 0x00, 0x00, 0x00, 0xff, 0x0c, 0x01, 0x00, 0x00, 0x00, 0x23, 0xff,
    0x0d, 0x01, 0x00, 0x00, 0x00, 0x00, 0xff, 0x0e, 0x01, 0x00, 0x00, 0x4a,
    0xb1, 0xff, 0x0f, 0x01, 0x00, 0x00, 0x50, 0x77, 0xff, 0x10, 0x01, 0x00,
    0x00, 0x15, 0x67, 0xff, 0x11, 0x01, 0x00, 0x00, 0xff, 0xff, 0xff, 0x12,
    0x01, 0x3f, 0xce, 0x97, 0x24, 0xff, 0x13, 0x01, 0xff, 0xff, 0xff, 0x85,
    0xff, 0x14, 0x01, 0x00, 0x03, 0xd8, 0xa8, 0xff, 0x15, 0x01, 0x00, 0x00,
    0x34, 0x02, 0xff, 0x16, 0x01, 0x00, 0x00, 0x2f, 0xe4, 0xff, 0x17, 0x01,
    0x00, 0x00, 0xff, 0xff, 0xff, 0x18, 0x01, 0x00, 0x00, 0xff, 0xff, 0xff,
    0x19, 0x01, 0x00, 0x00, 0x4c, 0xc3, 0xff, 0x1a, 0x01, 0x36, 0xf0, 0x67,
    0x02, 0xff, 0x1b, 0x01, 0xff, 0xff, 0xff, 0x85, 0xff, 0x1c, 0x01, 0x00,
    0x00, 0x34, 0x02, 0xff, 0x1d, 0x01, 0x7f, 0xff, 0xff, 0xff, 0xff, 0x1e,
    0x01, 0xff, 0xf6, 0x71, 0xae, 0xff, 0x1f, 0x01, 0x00, 0x00, 0x00, 0x65,
    0xff, 0x20, 0x01, 0x00, 0x00, 0x00, 0x00, 0xff, 0x21, 0x01, 0x1d, 0x50,
    0x1f, 0x18, 0xff, 0x22, 0x01, 0x7f, 0xff, 0xff, 0xff, 0xff, 0x23, 0x01,
    0x00, 0x00, 0x2f, 0xe4, 0xff, 0x24, 0x01, 0x7f, 0xff, 0xff, 0xff, 0xff,
    0x25, 0x01, 0x7f, 0xff, 0xff, 0xff, 0xff, 0x26, 0x01, 0x00, 0x34, 0x00,
    0x25, 0xff, 0x27, 0x01, 0x00, 0x00, 0x00, 0x6b, 0xff, 0x28, 0x01, 0x00,
    0x00, 0x00, 0x5b, 0xff, 0x29, 0x01, 0x00, 0x00, 0x01, 0x8a, 0xff, 0xa4,
    0xe9};

} // namespace spymarine
