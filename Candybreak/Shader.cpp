#include "Shader.h"

Shader::Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath, const char* computePath)
{
	ID = glCreateProgram();

	GLuint vertexShaderID, fragmentShaderID, geometryShaderID, computeShaderID;
	std::string vertexCode;
	std::string fragmentCode;
	std::string geometryCode;
	std::string computeCode;

	if (vertexPath != nullptr)
	{
		readShaderCode(vertexPath, vertexCode);
		vertexShaderID = createShader(vertexCode.c_str(), GL_VERTEX_SHADER);
		glAttachShader(ID, vertexShaderID);
	}
	if (fragmentPath != nullptr)
	{
		readShaderCode(fragmentPath, fragmentCode);
		fragmentShaderID = createShader(fragmentCode.c_str(), GL_FRAGMENT_SHADER);
		glAttachShader(ID, fragmentShaderID);
	}
	if (geometryPath != nullptr)
	{
		readShaderCode(geometryPath, geometryCode);
		geometryShaderID = createShader(geometryCode.c_str(), GL_GEOMETRY_SHADER);
		glAttachShader(ID, geometryShaderID);
	}
	if (computePath != nullptr)
	{
		readShaderCode(computePath, computeCode);
		computeShaderID = createShader(computeCode.c_str(), GL_COMPUTE_SHADER);
		glAttachShader(ID, computeShaderID);
	}

	glLinkProgram(ID);
	checkCompileErrors(ID, "PROGRAM");

	if (vertexPath != nullptr)
		glDeleteShader(vertexShaderID);
	if (fragmentPath != nullptr)
		glDeleteShader(fragmentShaderID);
	if (geometryPath != nullptr)
		glDeleteShader(geometryShaderID);
	if (computePath != nullptr)
		glDeleteShader(computeShaderID);
}

Shader::~Shader()
{
	
}

void Shader::readShaderCode(const char* path, std::string &code)
{
	std::ifstream file;
	file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		file.open(path);
		std::stringstream stream;
		stream << file.rdbuf();
		file.close();
		code = stream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ: " << path << std::endl;
	}
}

GLuint Shader::createShader(const char* code, GLenum type)
{
	GLuint id = glCreateShader(type);
	glShaderSource(id, 1, &code, NULL);
	glCompileShader(id);
	checkCompileErrors(id, "SHADER");

	return id;
}

// activate the shader
// ------------------------------------------------------------------------
void Shader::use()
{
	glUseProgram(ID);
}

// utility uniform functions
// ------------------------------------------------------------------------
void Shader::setBool(const std::string &name, bool value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string &name, int value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, float value) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setVec2(const std::string &name, const glm::vec2 &value) const
{
	glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}

void Shader::setVec2(const std::string &name, float x, float y) const
{
	glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
}

void Shader::setVec3(const std::string &name, const glm::vec3 &value) const
{
	glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}

void Shader::setVec3(const std::string &name, float x, float y, float z) const
{
	glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
}

void Shader::setVec4(const std::string &name, const glm::vec4 &value) const
{
	glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}

void Shader::setVec4(const std::string &name, float x, float y, float z, float w)
{
	glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
}

void Shader::setMat2(const std::string &name, const glm::mat2 &mat) const
{
	glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setMat3(const std::string &name, const glm::mat3 &mat) const
{
	glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setMat4(const std::string &name, const glm::mat4 &mat) const
{
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

// utility function for checking shader compilation/linking errors.
// ------------------------------------------------------------------------
void Shader::checkCompileErrors(GLuint shader, std::string type)
{
	GLint success;
	GLchar infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
}