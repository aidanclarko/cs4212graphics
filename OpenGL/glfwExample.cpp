#include <cstdlib>
#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "GLSL.h"

#include "vec3.h"
#include "PerspectiveCamera.h"
#include "Triangle.h"
#include "ObjMesh.h"
#include "GLMTSphere.h"

int CheckGLErrors(const char *s)
{
    int errCount = 0;
    return errCount;
}

int main(void)
{
    /* Initialize the library */
    if (!glfwInit()) {
        exit (-1);
    }
    // throw std::runtime_error("Error! initialization of glfw failed!");

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    /* Create a windowed mode window and its OpenGL context */
    int winWidth = 1000;
    float aspectRatio = 1.0; // 16.0 / 9.0; // winWidth / (float)winHeight;
    int winHeight = winWidth / aspectRatio;
    
    GLFWwindow* window = glfwCreateWindow(winWidth, winHeight, "GLFW Example", NULL, NULL);
    if (!window) {
        std::cerr << "GLFW did not create a window!" << std::endl;
        
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    GLenum err=glewInit();
    if(err != GLEW_OK) {
        std::cerr <<"GLEW Error! glewInit failed, exiting."<< std::endl;
        exit(EXIT_FAILURE);
    }

    const GLubyte* renderer = glGetString (GL_RENDERER);
    const GLubyte* version = glGetString (GL_VERSION);
    std::cout << "Renderer: " << renderer << std::endl;
    std::cout << "OpenGL version supported: " << version << std::endl;

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glClearColor(0.5, 0.65, 0.43, 1.0);

    int fb_width, fb_height;
    glfwGetFramebufferSize(window, &fb_width, &fb_height);
    glViewport(0, 0, fb_width, fb_height);

    // Need to set a projection matrix that fits the aspect ratio set
    // by the window frame.
    //
    // The ortho parameters, in order: left, right, bottom, top, zNear, zFar
    float halfWidth = 15.0 / 2.0;
    float halfHeight = halfWidth;

    float left = -halfWidth;
    float right = halfWidth;

    float bottom = -halfHeight;
    float top = halfHeight;

    float near = 5.0f;
    float far = -5.0f;

    // glm::mat4 M_ortho = glm::ortho(-halfWidth, halfWidth, -halfHeight, halfHeight, near, far);
    vec3 m_pos(1, 2,0), m_viewDir(0,0,-1);

    PerspectiveCamera cam(fb_width, fb_height, m_pos, m_viewDir, 0.5f, 1.0f, 3.14159f/4.0f, 1.0f, 0.1f, 100.0f);
    glm::mat4 M_pers = cam.getPerspectiveMatrix();


    GLint major_version;
    glGetIntegerv(GL_MAJOR_VERSION, &major_version);
    std::cout << "GL_MAJOR_VERSION: " << major_version << std::endl;

    GLuint m_triangleVBO[1], m_VAO;
    

    // create a Vertex Array Buffer to hold our triangle data                                               
    glGenBuffers(1, m_triangleVBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_triangleVBO[0]);

    // this is the actual triangle data that will be copied to                                              
    // the GPU memory
    //bunny raw data could easily be implemented here                                                                                       
    // std::vector< float > host_VertexBuffer{
    //     -3.0f, -3.0f, 0.0f,    1.0f , 0.0f, 0.0f,                             
    //      3.0f, -3.0f, 0.0f,    0.0f, 1.0f, 0.0f,                                 
    //      0.0f, 3.0f, 0.0f,     0.0f, 0.0f, 1.0f 
    // };     

   
    GLMSphere g(4);
    std::vector<Triangle> triList = g.makeMesh(3);

    ObjMesh obj("../../src/json/abysswatchers.obj");
    std::vector< VertexPoint > host_VertexBuffer;
    
    // auto vb2 = t2.toVertexBuffer();
    // host_VertexBuffer.insert(host_VertexBuffer.end(), vb2.begin(), vb2.end());
    // t.toVertexBuffer();
    
    for(int i = 0; i < triList.size(); i++) {
        auto faces = triList.at(i).toVertexBuffer();
        host_VertexBuffer.insert(host_VertexBuffer.end(), faces.begin(), faces.end());
    }

    int numBytes = host_VertexBuffer.size() * sizeof(VertexPoint);

    // copy the numBytes from host_VertexBuffer t the GPU and store in                                      
    // the currently bound VBO                                                                              
    glBufferData(GL_ARRAY_BUFFER, numBytes, host_VertexBuffer.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // once copied, we no longer need the data on the host (CPU) :) 
    int vertexCount = host_VertexBuffer.size();                                            
    host_VertexBuffer.clear();

    // create a vertex array object that will map the attributes in                                         
    // our vertex buffer to different location attributes for our                                           
    // shaders                                                                                              
    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    // VAO details here - we only have 1 attribute or location                                              
    // (Position of the vertex)    
    
    glBindBuffer(GL_ARRAY_BUFFER, m_triangleVBO[0]);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPoint), (void*)offsetof(VertexPoint, point));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPoint), (void*)offsetof(VertexPoint, normal));

    glBindVertexArray(0);



    // Create a shader using my GLSLObject class                                                            
    sivelab::GLSLObject shader;
    // shader.addShader( "vertexShader_passthrough.glsl", sivelab::GLSLObject::VERTEX_SHADER );
    shader.addShader( "vertexShader_blinn.glsl", sivelab::GLSLObject::VERTEX_SHADER );
    shader.addShader( "fragmentShader_blinn.glsl", sivelab::GLSLObject::FRAGMENT_SHADER );
    // shader.addShader( "fragmentShader_passthrough.glsl", sivelab::GLSLObject::FRAGMENT_SHADER );
    shader.createProgram();

    GLuint projMatrixID, viewMatrixID, modelMatrixID, normalMatrixID, lightPosID, diffID, camPosID, specularID, shininessID;
    projMatrixID = shader.createUniform( "projMatrix" );
    viewMatrixID = shader.createUniform( "viewMatrix" );
    modelMatrixID = shader.createUniform( "modelMatrix" );
    normalMatrixID = shader.createUniform( "normalMatrix" );
    lightPosID = shader.createUniform( "lightPos" );
    camPosID = shader.createUniform( "camPos" );
    specularID = shader.createUniform( "specular" );
    shininessID = shader.createUniform( "shininess" );
    diffID = shader.createUniform( "diffuse" );

    glm::mat4 modelTransform = glm::mat4(1.0);

    float rot  = 0.0f;
    modelTransform = glm::rotate(modelTransform, rot, glm::vec3(0, 1, 0));


    glm::mat4 M_normal = glm::mat4(1.0);

    glm::vec3 lightPos(5, 10, 10);
    

    //Shader Components
    glm::vec3 diffuse(0.263, 0.678, 0.306);
    glm::vec3 specular(1.0f, 1.0f, 1.0f);
    float shininess = 120.0f;


    double timeDiff = 0.0, startFrameTime = 0.0, endFrameTime = 0.0;

    
    
    /* Loop until the user closes the window */
    /* game loop above is initialization getting itn on gpu, load
        scene file 
        todo: load triangle
    
    */

    
    float rotationAngle = 0.0f;
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    


    while (!glfwWindowShouldClose(window))
    {  
        glm::vec3 camPos = cam.getGLMPos();
        endFrameTime = glfwGetTime();
        timeDiff = endFrameTime - startFrameTime;
        startFrameTime = glfwGetTime();


        // Clear the window's buffer (or clear the screen to our
        // background color)
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 M_view = cam.lookAt();
        /* Render your objects here */
        shader.activate();
        modelTransform = glm::mat4(1.0);
        modelTransform = glm::rotate(modelTransform, rotationAngle, glm::vec3(1, 1, 0));

        M_normal = glm::transpose(glm::inverse(modelTransform));

        rotationAngle += 0.00;
        if(rotationAngle > 2.0 * 3.14159) rotationAngle = 0.0f;
        
        

        // copy from the host to the device the view matrix and the projection matrix                                                                                       
        glUniformMatrix4fv(projMatrixID, 1, GL_FALSE, glm::value_ptr( M_pers ));
        glUniformMatrix4fv(viewMatrixID, 1, GL_FALSE, glm::value_ptr( M_view ));
        glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, glm::value_ptr( modelTransform ));
        glUniformMatrix4fv(normalMatrixID, 1, GL_FALSE, glm::value_ptr( M_normal ));
        glUniform3fv(lightPosID, 1, glm::value_ptr(lightPos));
        glUniform3fv(camPosID, 1, glm::value_ptr(camPos));
        glUniform3fv(diffID, 1, glm::value_ptr(diffuse));
        glUniform3fv(specularID, 1, glm::value_ptr(specular));
        glUniform1f(shininessID, shininess);


        glBindVertexArray(m_VAO);
        glDrawArrays(GL_TRIANGLES, 0, vertexCount);
        glBindVertexArray(0);

        shader.deactivate();

        // Swap the front and back buffers
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();

        float moveRatePerFrame = 0.05;

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            cam.moveForward(moveRatePerFrame);
        }
        else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            cam.moveLeft(moveRatePerFrame);
        }
        else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            cam.moveBack(moveRatePerFrame);
        }
        else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            cam.moveRight(moveRatePerFrame);
        }

        if (glfwGetKey( window, GLFW_KEY_T ) == GLFW_PRESS) {
            std::cout << "fps: " << 1.0/timeDiff << std::endl;
        }
        if (glfwGetKey( window, GLFW_KEY_ESCAPE ) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, 1);
        }
    }
  
    glfwTerminate();
    return 0;
}
