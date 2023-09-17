#include "AnimationPch.h"

#include <GLFW/glfw3.h>

#include "Application.h"

#include "Core/ApplicationCore.h"
#include "Core/Logger/GLDebug.h"
#include "Core/Logger/Log.h"
#include "Graphics/RenderApi.h"
#include "Graphics/OpenGL/Buffers/IndexBuffer.h"

#include "Graphics/OpenGL/Buffers/VertexBuffer.h"

namespace Animator
{
	Application* Application::instance = nullptr;

	Application::Application(const std::string& name, uint32_t width, uint32_t height)
	{
		if (!instance)
		{
			Log::Initialize();
		}

		ANIM_ASSERT(!instance, "Application Already exists!");
		instance = this;

		window = std::unique_ptr<IWindow>(IWindow::Create({ name, width, height }));

		// Bind Event Callback here
	}

	void Application::Initialize()
	{
		
	}

	void Application::Run()
	{
		const char* vertexShaderSource = "#version 460 core\n"
			"layout (location = 0) in vec3 aPos;\n"
			"void main()\n"
			"{\n"
			"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
			"}\0";

		const char* fragmentShaderSource = "#version 460 core\n"
			"out vec4 FragColor;\n"
			"void main()\n"
			"{\n"
			"	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
			"}\0";

		// Vertex Shader
		unsigned int vertexShader;
		vertexShader = GL_CALL(glCreateShader, GL_VERTEX_SHADER);

		GL_CALL(glShaderSource, vertexShader, 1, &vertexShaderSource, nullptr);
		GL_CALL(glCompileShader, vertexShader);

		int success;
		char infoLog[512];
		GL_CALL(glGetShaderiv, vertexShader, GL_COMPILE_STATUS, &success);

		if (!success)
		{
			GL_CALL(glGetShaderInfoLog, vertexShader, 512, nullptr, infoLog);
			ANIM_ASSERT(false, "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n{0}", infoLog);
		}

		// Fragment Shader
		unsigned int fragmentShader;
		fragmentShader = GL_CALL(glCreateShader, GL_FRAGMENT_SHADER);

		GL_CALL(glShaderSource, fragmentShader, 1, &fragmentShaderSource, nullptr);
		GL_CALL(glCompileShader, fragmentShader);

		GL_CALL(glGetShaderiv, fragmentShader, GL_COMPILE_STATUS, &success);

		if (!success)
		{
			GL_CALL(glGetShaderInfoLog, fragmentShader, 512, nullptr, infoLog);
			ANIM_ASSERT(false, "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n{0}", infoLog);
		}

		// Creating Shader Program
		unsigned int shaderProgram;
		shaderProgram = GL_CALL(glCreateProgram);

		GL_CALL(glAttachShader, shaderProgram, vertexShader);
		GL_CALL(glAttachShader, shaderProgram, fragmentShader);
		GL_CALL(glLinkProgram, shaderProgram);

		GL_CALL(glGetProgramiv, shaderProgram, GL_LINK_STATUS, &success);
		if (!success)
		{
			GL_CALL(glGetShaderInfoLog, fragmentShader, 512, nullptr, infoLog);
			ANIM_ASSERT(false, "ERROR::SHADER::LINK_FAILED\n{0}", infoLog);
		}

		GL_CALL(glUseProgram, shaderProgram);

		GL_CALL(glDeleteShader, vertexShader);
		GL_CALL(glDeleteShader, fragmentShader);

		// set up vertex data (and buffer(s)) and configure vertex attributes
		// ------------------------------------------------------------------
		constexpr float vertices[] = {
			 0.5f,  0.5f, 0.0f,  // top right
			 0.5f, -0.5f, 0.0f,  // bottom right
			-0.5f, -0.5f, 0.0f,  // bottom left
			-0.5f,  0.5f, 0.0f   // top left 
		};

		constexpr unsigned int indices[] = {  // note that we start from 0!
			0, 1, 3,  // first Triangle
			1, 2, 3   // second Triangle
		};

		unsigned int VAO;
		GL_CALL(glCreateVertexArrays, 1, &VAO);
		GL_CALL(glBindVertexArray, VAO);

		//VertexArray vertexArrayObject;

		VertexBuffer vertexBufferObject(vertices, sizeof(vertices));

		IndexBuffer indexBufferObject(indices, sizeof(indices));

		// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).

		//GL_CALL(glVertexAttribPointer, 0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		//GL_CALL(glEnableVertexAttribArray, 0);

		GL_CALL(glVertexArrayAttribBinding, VAO, 0, 0);
		GL_CALL(glVertexArrayAttribFormat, VAO, 0, 4, GL_FLOAT, GL_FALSE, 3 * sizeof(float));
		GL_CALL(glEnableVertexArrayAttrib, VAO, 0);

		// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
		//GL_CALL(glBindBuffer, GL_ARRAY_BUFFER, 0);
		//vertexBufferObject.UnBind();

		// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
		// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
		//GL_CALL(glBindVertexArray, 0);
		//vertexArrayObject.Bind();

		// GAME LOOP ////////////////////////////
		while (running && !window->WindowShouldClose())
		{
			RenderApi::GetContext()->ClearColor();
			RenderApi::GetContext()->ClearBuffer();

			//GL_CALL(glUseProgram, shaderProgram);
			//GL_CALL(glBindVertexArray, VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
			//glDrawArrays(GL_TRIANGLES, 0, 6);
			//GL_CALL(glDrawArrays, GL_TRIANGLES, 0, 6);
			//GL_CALL(glDrawElements, GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
			// glBindVertexArray(0); // no need to unbind it every time 

			// Swap buffer and poll events
			window->Update();
		}

		GL_CALL(glDeleteVertexArrays, 1, &VAO);
		//GL_CALL(glDeleteBuffers, 1, &VBO);
		//GL_CALL(glDeleteBuffers, 1, &EBO);
		GL_CALL(glDeleteProgram, shaderProgram);
	}

	void Application::Render()
	{
	}

	bool Application::Shutdown()
	{
		running = false;
		return true;
	}
}
