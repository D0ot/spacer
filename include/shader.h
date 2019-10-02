#ifndef SHADER_H_
#define SHADER_H_

#include <glad/glad.h>
#include <string>
#include <iostream>

class ShaderProgram
{
private:
    unsigned int ID;
public:
    ShaderProgram(const std::string& vertexPath, const std::string& fragPath);
    ~ShaderProgram();
    void use();
    unsigned int getProgram()const;
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;

};

#endif