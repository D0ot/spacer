#include <string>

extern const std::string PROJECT_ROOT_DIR = EXT_ROOT_DIR;

std::string pathRelatedToRoot(const std::string& str)
{
    return PROJECT_ROOT_DIR + str;
}
