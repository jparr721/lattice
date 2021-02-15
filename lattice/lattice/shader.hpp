//
//  shader.hpp
//  lattice
//
//  Created by Jarred Parr on 2/2/21.
//

#ifndef shader_hpp
#define shader_hpp

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include <GL/glew.h>

class Shader {
  public:
    Shader() = default;
    ~Shader() = default;

    GLuint Initialize(std::string vpath, std::string fpath) {
        GLuint v, f;
        std::stringstream ss;
        char log[512];

        v = glCreateShader(GL_VERTEX_SHADER);
        std::fstream vf(vpath);
        if (vf.good()) {
            ss << vf.rdbuf();
            std::string str = ss.str();
            const GLchar* g = str.c_str();
            glShaderSource(v, 1, &g, nullptr);
            glCompileShader(v);
            glGetShaderInfoLog(v, 512, nullptr, log);
            std::cout << "Vertex Shader: " << log << std::endl;
        } else {
            std::cout << "Bad path " << vpath.c_str() << std::endl;
            return static_cast<GLuint>(-1);
        }

        ss.str("");
        f = glCreateShader(GL_FRAGMENT_SHADER);
        std::fstream ff(fpath);
        if (ff.good()) {
            ss << ff.rdbuf();
            std::string str = ss.str();
            const GLchar* g = str.c_str();
            glShaderSource(f, 1, &g, nullptr);
            glCompileShader(f);
            glGetShaderInfoLog(f, 512, nullptr, log);
            std::cout << "Fragment Shader: " << log << std::endl;
        } else {
            std::cout << "Bad path " << fpath.c_str() << std::endl;
            return static_cast<GLuint>(-1);
        }

        GLuint prog;
        prog = glCreateProgram();
        glAttachShader(prog, v);
        glAttachShader(prog, f);
        glLinkProgram(prog);
        glGetProgramInfoLog(prog, 512, nullptr, log);
        std::cout << "Program: " << log << std::endl;
        glDeleteShader(v);
        glDeleteShader(f);
        return prog;
    }
};
#endif /* shader_hpp */
