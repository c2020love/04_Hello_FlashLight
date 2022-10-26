#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include<math.h>
class Shader
{

public:
    //程序ID
    unsigned int ID;
    //构造器读取并构建着色器
    Shader(const char *vertexPath, const char *fragmentPath)
    {
        //从文件中获取顶点/片段着色器
        std::string vertexCode;
        std::string fragmentCode;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;
        //抛常
        vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try
        {
            //打开着色器的编译文件
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);
            std::stringstream vShaderStream, fShaderStream;

            //读取俩着色器文件里的数据到缓冲中
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();

            //关闭文件
            vShaderFile.close();
            fShaderFile.close();

            //将有stringStream保存的数据转成string
            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();
        }
        catch (std::ifstream::failure& e)
        {
            std::cout << "读取着色器文件数据失败" <<e.what()<< std::endl;
        }
        // string 转化为char*类型
        const char *vShaderCode = vertexCode.c_str();
        const char *fShaderCode = fragmentCode.c_str();

        //@编译链接着色器program

        unsigned int vertex, fragment;
        GLint success;
        GLchar inflog[2048];

        //创建着色器对象
        vertex=glCreateShader(GL_VERTEX_SHADER);
        fragment=glCreateShader(GL_FRAGMENT_SHADER);

        //先编译顶点着色器
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);

        //打印编译问题
        glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(vertex, 2048, NULL, inflog);
            std::cout << "顶点着色器编译失败" <<inflog<< std::endl;
        }

        //再编译片段着色器
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);

        //打印编译问题
        glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(fragment, 2048, NULL, inflog);
            std::cout << "片段着色器编译失败" << std::endl;
        }

        //创建着色器程序shader——Program来连接这两个着色器
        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        glLinkProgram(ID);
        glGetProgramiv(ID, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(ID, 512, NULL, inflog);
            std::cout << "着色器程序连接失败\n"
                      << inflog << std::endl;
        }
        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }
    //激活程序
    void use()
    {
        //根据ID运行着色器程序
        glUseProgram(ID);
    }

    // uniform工具函数
    void setBool(const std::string &name, bool value) const
    {
        glUniform1i(glGetUniformLocation(ID,name.c_str()),(int)value);

    }
    void setInt(const std::string &name, int value) const
    {
        glUniform1i(glGetUniformLocation(ID,name.c_str()),value);
    }
    void setFloat(const std::string &name, float value) const
    {
        glUniform1f(glGetUniformLocation(ID,name.c_str()),value);
    }

    void setMat4(const std::string &name,const glm::mat4 &mat) const
    {
        glUniformMatrix4fv(glGetUniformLocation(ID,name.c_str()),1,GL_FALSE,&mat[0][0]);
    }

    void setVec3(const std::string &name, float x,float y,float z) const
    {
        glUniform3f(glGetUniformLocation(ID,name.c_str()),x,y,z);
    }

    void setVec3(const std::string &name, const glm::vec3 &vec) const
    {
        glUniform3fv(glGetUniformLocation(ID,name.c_str()),1,&vec[0]);
    }
};
#endif
