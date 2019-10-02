#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <utility>
#include <dataset.h>
#include <spacer.h>
#include <map>
#include <shader.h>
#include <externdef.h>

const int screenWidth = 800;
const int screenHeight = 600;

float cameraSpeed = 0.05f;
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 10.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

float lastFrame = 0.0f;
float deltaTime = 0.0f;

float pitch;
float yaw;

bool first = true;
float lastX = 400, lastY = 300;

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow *windows, double xpos, double ypos)
{
    float xoffset = xpos - lastX;
    float yoffset = ypos - lastY;
    lastX = xpos;
    lastY = ypos;

    if (first)
    {
        xoffset = 0;
        yoffset = 0;
        first = false;
    }

    float sensitivity = 0.05f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch -= yoffset;

    if (pitch > 89.0f)
    {
        pitch = 89.0f;
    }

    if (pitch < -89.0f)
    {
        pitch = -89.0f;
    }
}

void input_process(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }

    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    cameraSpeed = 20.5f * deltaTime;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        cameraPos += cameraSpeed * cameraFront;
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        cameraPos -= cameraSpeed * cameraFront;
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    }
}

glm::vec3 conv(const Eigen::Vector3d &v)
{
    return glm::vec3((float)v.x(), (float)v.y(), (float)v.z());
}

std::vector<glm::vec3> convs(const std::vector<Eigen::Vector3d> &vs)
{
    std::vector<glm::vec3> ret;
    for (const auto &x : vs)
    {
        ret.emplace_back((float)x.x() + 50, (float)x.y() + 50, (float)x.z() +50);
    }
    return ret;
}

void convsadd(std::vector<glm::vec3> &ret, const std::vector<Eigen::Vector3d> &vs)
{
    for (const auto &x : vs)
    {
        ret.emplace_back((float)x.x() , (float)x.y() , (float)x.z() );
    }
}

std::vector<Eigen::Vector3d> genVertexSetFromLeaf(Spacer::Node *leaf)
{
    std::vector<Eigen::Vector3d> ret;

    Eigen::Vector3d z(0, 0, 0);

    Spacer::Node *p = leaf;
    while (p)
    {
        auto m = p->getTransformGlobal();
        ret.emplace_back(m * z);
        p = p->getParent();
    }
    return ret;
}

int main()
{
    if (!glfwInit())
    {
        std::cerr << "glfwInit() failed\n";
        std::exit(1);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(screenWidth, screenHeight, "Spacer GUI", NULL, NULL);

    if (!window)
    {
        std::cerr << "glfwCreateWinodw() failed\n";
        std::exit(2);
    }

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD\n";
        std::exit(3);
    }

    glViewport(0, 0, 800, 600);

    dataSetInit();

    std::map<std::string, Spacer::Node *> nodeTable;

    Spacer::Node torso("torso", Eigen::Vector3d(0, 0, 0), Eigen::Vector3d(0, 0, 0), Eigen::AngleAxisd(0, Eigen::Vector3d(1, 1, 1)));

    nodeTable["torso"] = torso.getRawPointer();

    for (size_t i = 0; i < sizeof(parts) / sizeof(BodyPart); ++i)
    {
        BodyPart &bp = parts[i];
        std::cout << "adding child : " << bp.name << std::endl;
        Spacer::Node *parent = nodeTable[bp.parent];
        Spacer::Node *child = parent->addChild(bp.name, bp.translation, bp.anchor, Eigen::AngleAxisd(0, bp.axis));
        nodeTable[bp.name] = child;
    }

    std::string leaf[] = {
        "head",
        "lowerarm_r",
        "lowerarm_l",
        "foot_r",
        "foot_l",
    };

    ShaderProgram program(pathRelatedToRoot("shader/shader.vs"), pathRelatedToRoot("shader/shader.fs"));

    std::vector<glm::vec3> test;
    test.emplace_back(0.5f, 0.5f, 0.0f);
    test.emplace_back(0.5f, -0.5f, 0.0f);
    test.emplace_back(-0.5f, 0.5f, 0.0f);
    std::vector<glm::vec3> vertics;

    size_t sizesum = 0;

    std::vector<std::pair<size_t, size_t>> size_pairs;
    for (auto &x : leaf)
    {
        auto &&g = genVertexSetFromLeaf(nodeTable[x]);
        for(auto &y : g)
        {
            std::cout << y << std::endl;
        }
        convsadd(vertics, g);
        size_pairs.push_back(std::make_pair(sizesum, g.size()));
        sizesum += g.size();
    }
    std::cout << "sizesum : " << sizesum << std::endl;
    std::cout << "sizesum : " << vertics.size()<< std::endl;

    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(vertics), vertics, GL_STATIC_DRAW);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(float)*3*test.size(), test.data(), GL_STATIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * vertics.size(), vertics.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    program.use();
    GLint modelloc = glGetUniformLocation(program.getProgram(), "model");
    GLint viewloc = glGetUniformLocation(program.getProgram(), "view");
    GLint projloc = glGetUniformLocation(program.getProgram(), "proj");

    glm::mat4 model(1.0f), proj(1.0f);

    Spacer::Node *a = nodeTable["shoulder_r"];
    double angle;


    while (!glfwWindowShouldClose(window))
    {
        sizesum = 0;
        glClearColor(0.2f, 0.2f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        //a->setAngel(glfwGetTime() * 10);

        vertics.clear();
        std::vector<std::pair<size_t, size_t>> size_pairs;
        for (auto &x : leaf)
        {
            auto &&g = genVertexSetFromLeaf(nodeTable[x]);
            convsadd(vertics, g);
            size_pairs.push_back(std::make_pair(sizesum, g.size()));
            sizesum += g.size();
        }
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * vertics.size(), vertics.data(), GL_STATIC_DRAW);

        input_process(window);
        glm::vec3 direction;
        direction.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
        direction.y = sin(glm::radians(pitch));
        direction.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));

        cameraFront = glm::normalize(direction);

        glm::mat4 view(1.0f);
        //view = glm::lookAt(glm::vec3(camX, 0.0, camZ), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0,0.0));
        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

        glUniformMatrix4fv(viewloc, 1, GL_FALSE, glm::value_ptr(view));

        model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(0.1, 0.1, 0.1));
        model = glm::translate(model, glm::vec3(0, 0, -10));
        proj = glm::perspective(glm::radians(45.0f), 8.0f / 6.0f, 0.1f, 500.0f);

        glUniformMatrix4fv(modelloc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewloc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projloc, 1, GL_FALSE, glm::value_ptr(proj));


        for (size_t i = 0; i < size_pairs.size(); ++i)
        {
            glDrawArrays(GL_LINE_STRIP, size_pairs[i].first, size_pairs[i].second);
        }



        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    return 0;
}