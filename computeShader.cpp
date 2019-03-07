#include "computeShader.h"


	ComputeShader::ComputeShader(const char* computePath)
	{
		//std::cout << "CREATING BASE SHADER" << std::endl;
		std::string computeShadercode = loadShaderFromFile(computePath);

		const char * computeShaderString = computeShadercode.c_str();
		// 2. compile shaders
		unsigned int compute;
		int success;
		char infoLog[512];
		// vertex shader
		compute = glCreateShader(GL_COMPUTE_SHADER);
		glShaderSource(compute, 1, &computeShaderString, NULL);
		glCompileShader(compute);
		checkCompileErrors(compute, "COMPUTE", getShaderName(computePath));
		// shader Program
		ID = glCreateProgram();
		glAttachShader(ID, compute);
		glLinkProgram(ID);
		checkCompileErrors(ID, "PROGRAM", "");
		// delete the shaders as they're linked into our program now and no longer necessary
		glDeleteShader(compute);
		std::cout << "COMPUTE SHADER " << getShaderName(computePath) << " SUCCESSFULLY COMPILED!" << std::endl;
	}

	ComputeShader::~ComputeShader() {

	}

	void ComputeShader::use()
	{
		glUseProgram(ID);
	}
	// utility uniform functions
	// ------------------------------------------------------------------------
	void ComputeShader::setBool(const std::string &name, bool value) const
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
	}
	// ------------------------------------------------------------------------
	void ComputeShader::setInt(const std::string &name, int value) const
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	}
	// ------------------------------------------------------------------------
	void ComputeShader::setFloat(const std::string &name, float value) const
	{
		glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
	}
	void ComputeShader::setVec2(const std::string &name, glm::vec2 vector) const
	{
		unsigned int location = glGetUniformLocation(ID, name.c_str());
		glUniform2fv(location, 1, glm::value_ptr(vector));
	}
	void ComputeShader::setVec3(const std::string &name, glm::vec3 vector) const
	{
		unsigned int location = glGetUniformLocation(ID, name.c_str());
		glUniform3fv(location, 1, glm::value_ptr(vector));
	}

	void ComputeShader::setSampler2D(const std::string &name, unsigned int texture, int id) const
	{
		glActiveTexture(GL_TEXTURE0 + id);
		glBindTexture(GL_TEXTURE_2D, texture);
		this->setInt(name, id);
	}

	void ComputeShader::setSampler3D(const std::string &name, unsigned int texture, int id) const
	{
		glActiveTexture(GL_TEXTURE0 + id);
		glBindTexture(GL_TEXTURE_3D, texture);
		this->setInt(name, id);
	}

	void ComputeShader::setVec4(const std::string &name, glm::vec4 vector) const
	{
		unsigned int location = glGetUniformLocation(ID, name.c_str());
		glUniform4fv(location, 1, glm::value_ptr(vector));
	}

	void ComputeShader::setMat4(const std::string &name, glm::mat4 matrix) const
	{
		unsigned int mat = glGetUniformLocation(ID, name.c_str());
		glUniformMatrix4fv(mat, 1, false, glm::value_ptr(matrix));
	}
	void ComputeShader::checkCompileErrors(unsigned int shader, std::string type, std::string shaderName)
	{
		int success;
		char infoLog[1024];
		if (type != "PROGRAM")
		{
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "ERROR: SHADER" << shaderName << "COMPILATION ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
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

		//if (success) {
		//	std::cout << type + " SHADER SUCCESSFULLY COMPILED AND/OR LINKED!" << std::endl;
		//}
	}
	std::string ComputeShader::loadShaderFromFile(const char* shaderPath) {
		std::string shaderCode;
		std::ifstream shaderFile;
		shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try
		{
			// open files
			shaderFile.open(shaderPath);
			std::stringstream shaderStream;
			// read file's buffer contents into streams
			shaderStream << shaderFile.rdbuf();
			// close file handlers
			shaderFile.close();
			// convert stream into string
			shaderCode = shaderStream.str();
		}
		catch (std::ifstream::failure e)
		{
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		}
		return shaderCode;

	}

	std::string ComputeShader::getShaderName(const char* path) {
		std::string pathstr = std::string(path);
		const size_t last_slash_idx = pathstr.find_last_of("/");
		if (std::string::npos != last_slash_idx)
		{
			pathstr.erase(0, last_slash_idx + 1);
		}
		return pathstr;
	}

