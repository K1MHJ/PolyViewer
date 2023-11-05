#pragma once

#include "logGL.hpp"
#include "Shader.hpp"
#include "Geometry.hpp"

using namespace Geometry;

namespace GL{
    void RenderGObject(GBox* pBox, float x, float y, const glm::mat4 & mvp)
    {
        float vertices[] = {
            pBox->topRight.X + x, pBox->topRight.Y + y,
            pBox->topRight.X + x, pBox->botLeft.Y + y,
            pBox->botLeft.X + x, pBox->botLeft.Y + y,
            pBox->botLeft.X + x, pBox->topRight.Y + y
        };
        unsigned int indices[] = {
            0, 1, 3,
            1, 2, 3
        };  
        std::unique_ptr<Shader> m_Shader;
        m_Shader = std::make_unique<Shader>("res/shaders/Box.shader");
        m_Shader->Bind();
        m_Shader->SetUniform4f("color", 1.0f, 0.0f, 0.0f, 1.0f);
        m_Shader->SetUniformMat4f("mvp", mvp);
        m_Shader->Unbind();

        GLuint EBO;
        GLuint VBO;
        GLuint VAO;
        glGenBuffers(1, &EBO);
        glGenBuffers(1, &VBO);
        glGenVertexArrays(1, &VAO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); 

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        //glVertexAttribDivisor(0,0);
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        glBindVertexArray(VAO);
        m_Shader->Bind();
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        //glDrawElementsInstanced(GL_TRIANGLE_STRIP, 6, GL_UNSIGNED_INT, NULL, count_instance);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        m_Shader->Unbind();

        glDeleteBuffers(1, &EBO);
        glDeleteBuffers(1, &VBO);
        glDeleteVertexArrays(1, &VAO);
    }
}

