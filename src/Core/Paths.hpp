#pragma once

#include <filesystem>

class Paths
{
public:
    static std::filesystem::path Assets();
    static std::filesystem::path Shaders();
};