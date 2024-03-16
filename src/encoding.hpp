#pragma once

#include <Windows.h>

#include <string>
#include <format>
#include <exception>

class chcp {
    UINT old_cp;

public:
    chcp(UINT cp) {
        old_cp = GetConsoleOutputCP();
        SetConsoleOutputCP(cp);
    }

    ~chcp() {
        if (old_cp != 0) {
            SetConsoleOutputCP(old_cp);
        }
    }
};

inline std::wstring to_wstring(const std::string& s, UINT cp) {
    int len = MultiByteToWideChar(cp, 0, s.c_str(), s.length(), nullptr, 0);
    if (len < 0) {
        throw std::runtime_error(std::format("failed to convert string (cp:{}) to wstring", cp));
    }
    std::wstring result(len, L'\0');
    MultiByteToWideChar(cp, 0, s.c_str(), s.length(), result.data(), len);
    return result;
}

inline std::string to_string(const std::wstring& ws, UINT cp) {
    int len = WideCharToMultiByte(cp, 0, ws.c_str(), ws.length(), nullptr, 0, nullptr, nullptr);
    if (len < 0) {
        throw std::runtime_error(std::format("failed to convert wstring to string (cp:{})", cp));
    }
    std::string result(len, '\0');
    WideCharToMultiByte(cp, 0, ws.c_str(), ws.length(), result.data(), len, nullptr, nullptr);
    return result;
}

inline std::string convert_encoding(const std::string& s, UINT src_cp, UINT dst_cp) {
    auto wide = to_wstring(s, src_cp);
    return to_string(wide, dst_cp);
}
