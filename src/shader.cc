#include <shader.h>
#include <fstream>
#include <sstream>

ShaderProgram::ShaderProgram(const std::string& vpath, const std::string& fpath)
{
    std::ifstream vfile, ffile;
    vfile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    ffile.exceptions(std::ifstream::failbit | std::ifstream::badbit);


    std::string vShaderSource, fShaderSource;
    try
    {

        vfile.open(vpath);
        ffile.open(fpath);
        std::stringstream vShaderStream, fShaderStream;

        vShaderStream << vfile.rdbuf();
        fShaderStream << ffile.rdbuf();

        vfile.close();
        ffile.close();

        vShaderSource = vShaderStream.str();
        fShaderSource = fShaderStream.str();

    }
    catch(std::ifstream::failure e)
    {
        std::cerr << "ERROR::SHADER::FILE_READING_FAILED\n" << e.what();
        throw e;
    }

    const char* vShaderCStr = vShaderSource.c_str();
    const char* fShaderCStr = fShaderSource.c_str();


    unsigned int vertex, fragment;

    int success;
    char infoLog[512];

    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCStr, NULL);
    glCompileShader(vertex);

    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        std::cerr << "ERROR::VERTEX_SHADER_COMPILE_ERROR\n" << infoLog << std::endl;;
    }

    
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCStr, NULL);
    glCompileShader(fragment);
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        std::cerr << "ERROR::FRAGMENT_SHADER_COMPILE_ERROR\n" << infoLog << std::endl;;
    }


    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);

    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        std::cerr << "ERROR::PROGRAM_LINK_ERROR\n" << infoLog << std::endl;
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);

}

ShaderProgram::~ShaderProgram()
{
}

void ShaderProgram::use()
{
    glUseProgram(ID);
}

unsigned int ShaderProgram::getProgram() const
{
    return ID;
}

void ShaderProgram::setBool(const std::string& name, bool value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void ShaderProgram::setInt(const std::string& name, int value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void ShaderProgram::setFloat(const std::string& name, float value) const
{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}