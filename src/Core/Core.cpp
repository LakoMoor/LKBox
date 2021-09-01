#include "Core.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../Shader/shader_m.h"
#include "../Camera/camera.h"
#include "../Model/model.h"


#include <iostream>
#include <chrono>
#include <fstream>
#include <sstream>
#include <stb_image.h>


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
void UIFlipFlop(GLFWwindow* window);

unsigned int loadTexture(const char* path);
unsigned int loadCubemap(vector<std::string> faces);

static bool showWindow = true;

bool isHold = true;
bool rotateModel = false;

// Константы
int SCR_WIDTH;
int SCR_HEIGHT;


// Камера
Camera camera(glm::vec3(0.0f, 10.0f, 10.0f));

float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// Тайминги
float deltaTime = 0.0f;
float lastFrame = 0.0f;

//Imgui redactor
float RotationModelX;
float RotationModelY = 1.0f;
float RotationModelZ = 7.5f;

float RotateModelsF;

Core::Core()
{
    auto t_start = std::chrono::high_resolution_clock::now();

    ifstream ifs("../config.lke");
    if(!ifs)
    {
        ofstream outfile ("../config.lke");
        outfile << "LKBOX\n1280 720\n0\n" << endl;
        outfile.close();
        cout << "create config" << endl;
    }
    else
    {
        cout << "already config !" << endl;
    }
    string title = "None";
    SCR_WIDTH = 640;
    SCR_HEIGHT = 480;
    if(ifs.is_open())
    {
        getline(ifs, title);     
        ifs >> SCR_WIDTH >> SCR_HEIGHT;      
    }
    ifs.close();


    // glfw: инициализация и конфигурирование
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    

/*
    Shader ourShader("../resources/Shader/1.model_loading.vs", "../resources/Shader/1.model_loading.fs");
    Shader ourShaderCube("../resources/Shader/2.model_loading.vs", "../resources/Shader/2.model_loading.fs");
    Shader skyboxShader("../resources/Shader/6.1.skybox.vs", "../resources/Shader/6.1.skybox.fs");
*/
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // раскомментируйте эту строку, если используете macOS
#endif

    // glfw: создание окна
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LKBox", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    int frames = 0;
    double t, t0, fps;
    char title_string[10];

    t0 = glfwGetTime();

    // Сообщаем GLFW, чтобы он захватил наш курсор
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: загрузка всех указателей на OpenGL-функции
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return;
    }

    // Сообщаем stb_image.h, чтобы он перевернул загруженные текстуры относительно y-оси (до загрузки модели)
    stbi_set_flip_vertically_on_load(true);

    // Конфигурирование глобального состояния OpenGL
    glEnable(GL_DEPTH_TEST);

    // Компилирование нашей шейдерной программы
    Shader HonakaShader("../resources/shader/1.model_loading.vs", "../resources/Shader/1.model_loading.fs");
    Shader RoomShader("../resources/shader/2.model_loading.vs", "../resources/Shader/2.model_loading.fs");
    Shader skyboxShader("../resources/shader/6.1.skybox.vs", "../resources/Shader/6.1.skybox.fs");
    
    //Shader lightingShader("../resources/shader/5.4.light_casters.vs", "../resources/shader/5.4.light_casters.fs");
    //Shader lightCubeShader("../resources/shader/5.4.light_cube.vs", "../resources/shader/5.4.light_cube.fs");

    // Загрузка моделей
    Model HonakaModel("../resources/objects/honoka/Honoka Kosaka (No Brand Girls).obj");
    Model RoomModel("../resources/objects/room/room.obj");

    float skyboxVertices[] = {
         // координаты         
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

    // VAO скайбокса
    unsigned int skyboxVAO, skyboxVBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    
    //Загрузка текстур
    vector<std::string> faces
    {
        "../resources/textures/skybox/cube.jpg",
        "../resources/textures/skybox/cube.jpg",
        "../resources/textures/skybox/cube.jpg",
        "../resources/textures/skybox/cube.jpg",
        "../resources/textures/skybox/cube.jpg",
        "../resources/textures/skybox/cube.jpg"
    };
    unsigned int cubemapTexture = loadCubemap(faces);
    // Конфигурация шейдеров
    //shader.use();
    //shader.setInt("texture1", 0);

    skyboxShader.use();
    skyboxShader.setInt("skybox", 0);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();

    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();

    // Setup Dear ImGui style
    ImGui::LKBox();
    
    // Цикл рендеринга
    while (!glfwWindowShouldClose(window))
    {
        // Логическая часть работы со временем для каждого кадра
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        auto t_now = std::chrono::high_resolution_clock::now();
        float time = std::chrono::duration_cast<std::chrono::duration<float>>(t_now - t_start).count();

        

        // Обработка ввода
        processInput(window);

        // Рендеринг
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //ImGUI
        Core::UICore();

        // Убеждаемся, что активировали шейдер прежде, чем настраивать uniform-переменные/объекты_рисования
        HonakaShader.use();
        RoomShader.use();

        // Преобразования Вида/Проекции (Персонаж)
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        HonakaShader.setMat4("projection", projection);
        HonakaShader.setMat4("view", view);

        // Рендеринг загруженной модели (Персонаж)
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(RotationModelX, RotationModelY, RotationModelZ)); // смещаем вниз чтобы быть в центре сцены x y z
        model = glm::rotate(model, time * RotateModelsF, glm::vec3(1.0f, time * 1.0f, 1.0f)); // Вращение x y z
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// объект слишком большой для нашей сцены, поэтому немного уменьшим его x y z
        HonakaShader.setMat4("model", model);
        HonakaModel.Draw(HonakaShader);

        // Преобразования Вида/Проекции (Комната)
        //glm::mat4 projection1 = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        //glm::mat4 view1 = camera.GetViewMatrix();
        //RoomShader.setMat4("projection1", projection1);
        //RoomShader.setMat4("view1", view1);

        // Рендеринг загруженной модели (Комната)
        glm::mat4 model2 = glm::mat4(1.0f);
        model2 = glm::translate(model2, glm::vec3(1.0f, 1.0f, 1.0f)); // смещаем вниз чтобы быть в центре сцены x y z
        model2 = glm::rotate(model2, 0.0f, glm::vec3(1.0f,1.0f, 1.0f)); // Вращение x y z
        model2 = glm::scale(model2, glm::vec3(1.0f, 1.0f, 1.0f));	// объект слишком большой для нашей сцены, поэтому немного уменьшим его x y z
        RoomShader.setMat4("model1", model2);
        RoomModel.Draw(RoomShader);


        // glfw: fps
        
        t = glfwGetTime();
        if((t - t0) > 1.0 || frames == 0)
        {
            fps = (double)frames / (t - t0);
            sprintf(title_string, "LKBox |FPS: %.1f|", fps);
            glfwSetWindowTitle(window, title_string);
            t0 = t;
            frames = 0;
        }
        frames ++;
        /*std::cout << "FPS: "<< fps << std::endl;
        system("CLS");*/

        // Отрисовываем скайбокс последним
        glDepthFunc(GL_LEQUAL); // меняем функцию глубины, чтобы обеспечить прохождение теста глубины, когда значения равны содержимому буфера глубины
        skyboxShader.use();
        view = glm::mat4(glm::mat3(camera.GetViewMatrix())); // убираем из матрицы вида секцию, отвечающую за операцию трансляции
        skyboxShader.setMat4("view", view);
        skyboxShader.setMat4("projection", projection);
			
        // Куб скайбокса
        glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS); // восстанавливаем стандартное значение функции теста глубины


        // Render dear imgui into screen
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // glfw: обмен содержимым front- и back- буферов. Отслеживание событий ввода/вывода (была ли нажата/отпущена кнопка, перемещен курсор мыши и т.п.)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    // Опционально: освобождаем все ресурсы, как только они выполнили свое предназначение
    glDeleteVertexArrays(1, &skyboxVAO);
    glDeleteBuffers(1, &skyboxVAO);
    // glfw: завершение, освобождение всех выделенных ранее GLFW-реcурсов
    glfwTerminate();
    return;
}

void UIFlipFlop(GLFWwindow* window)
{
    /*
    if(ImGui::IsKeyDown('`'))
    {
       con = true;
    }
    else if (!ImGui::IsKeyDown('') && con && !isHold) 
    {
        con = false;
        isHold = true;
    }
    else if(!ImGui::IsKeyDown('`') && con && isHold)
    {
        con = false;
        isHold = false;
    }*/
}


void Core::UICore()
{
    //New Frame IMGUI
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    //imgui GUI

    
    if(showWindow)
    {
        ImGui::SetNextWindowSize(ImVec2(400,200), ImGuiCond_FirstUseEver);
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        //ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
        if(isHold == false)
        {
            if(ImGui::Begin("Pos", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoResize))
            {
                ImGui::Separator();
                ImGui::SliderFloat("X", &RotationModelX ,0.0f,180.0f);
                ImGui::Separator();
                ImGui::SliderFloat("Y", &RotationModelY ,1.0f,180.0f);
                ImGui::Separator();
                ImGui::SliderFloat("Z", &RotationModelZ ,7.5f,180.0f);
                ImGui::Separator();
                ImGui::Checkbox("Rotorrrrr", &rotateModel);
                ImGui::Separator();
                //ImGui::Separator();
                ImGui::End(); 
            }
        }
    }

    if(rotateModel == true)
    {
        RotateModelsF = glm::radians(180.0f) ;
    }
    else if(rotateModel == false)
    {
        RotateModelsF = glm::radians(0.0f) ;
    }
}

// Обработка всех событий ввода: запрос GLFW о нажатии/отпускании кнопки мыши в данном кадре и соответствующая обработка данных событий
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // Отрисовка в режиме каркаса
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_RELEASE)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // Курсор
    if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        glfwSetCursorPosCallback(window, mouse_callback);
        isHold = true;
    }

    if (glfwGetKey(window, GLFW_KEY_F2) == GLFW_PRESS)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
        glfwSetCursorPosCallback(window, NULL);
        isHold = false;
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}



// glfw: всякий раз, когда изменяются размеры окна (пользователем или операционной системой), вызывается данная callback-функция
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // Убеждаемся, что окно просмотра соответствует новым размерам окна.
    // Обратите внимание, ширина и высота будут значительно больше, чем указано, на Retina-дисплеях
    glViewport(0, 0, width, height);
}

// glfw: всякий раз, когда перемещается мышь, вызывается данная callback-функция
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // перевернуто, так как y-координаты идут снизу вверх

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: всякий раз, когда прокручивается колесико мыши, вызывается данная callback-функция
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    //camera.ProcessMouseScroll(yoffset);
}

unsigned int loadTexture(char const* path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

// Загрузка текстуры кубической карты из 6 отдельных текстур-граней.
// Порядок:
// +X (правая грань)
// -X (левая грань)
// +Y (верх)
// -Y (низ)
// +Z (фронтальная грань) 
// -Z (тыльная грань)
// -------------------------------------------------------
unsigned int loadCubemap(vector<std::string> faces)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}
