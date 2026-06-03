#include "Paths.hpp"

std::filesystem::path Paths::Assets(){
    return "../assets";
}

std::filesystem::path Paths::Shaders(){
    return Assets() / "shaders/";
}