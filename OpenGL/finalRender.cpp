#include <cstdlib>
#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "png++/png.hpp"

#define GLM_FORCE_RADIANS
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "GLSL.h"

#include "vec3.h"
#include "PerspectiveCamera.h"
#include "Triangle.h"
#include "ObjMesh.h"
#include "GLMSphere.h"
#include "render_helpers.h"
#include "Fireworks.h"
#include "ParticleObj.h"
#include "ParticleWave.h"
#include "Stars.h"
#include "Trees.h"

int CheckGLErrors(const char *s)
{
    int errCount = 0;
    return errCount;
}

GLuint handleTexture(std::string texFileName) {
    png::image<png::rgba_pixel> texPNGImage;
    texPNGImage.read(texFileName);

    int pngWidth = texPNGImage.get_width();
    int pngHeight = texPNGImage.get_height();

    std::vector<float> texData(pngWidth * pngHeight * 4);  // 4 channels

    size_t idx = 0;
    for(size_t row = 0; row < pngHeight; row++) {
        for(size_t col = 0; col < pngWidth; col++) {
            png::rgba_pixel pixel = texPNGImage[pngHeight - row - 1][col];
            texData[idx++] = pixel.red   / 255.0f;
            texData[idx++] = pixel.green / 255.0f;
            texData[idx++] = pixel.blue  / 255.0f;
            texData[idx++] = pixel.alpha / 255.0f;
        }
    }

    GLuint texID;
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, pngWidth, pngHeight, 0, GL_RGBA, GL_FLOAT, texData.data());
    glBindTexture(GL_TEXTURE_2D, 0);
    return texID;
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
    glClearColor(0, 0, 0, 1.0);

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
    vec3 m_pos(0, 0, 20);  
    vec3 m_viewDir(0, 0, -1);

    PerspectiveCamera cam(fb_width, fb_height, m_pos, m_viewDir, 0.5f, 1.0f, 3.14159f/4.0f, 1.0f, 0.1f, 100.0f);
    glm::mat4 M_pers = cam.getPerspectiveMatrix();


    GLint major_version;
    glGetIntegerv(GL_MAJOR_VERSION, &major_version);
    std::cout << "GL_MAJOR_VERSION: " << major_version << std::endl;

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////

    //textures

    GLuint texID = handleTexture("noise.png");
    GLuint treeTexID = handleTexture("tree2.png");


    GLuint m_triangleVBO[1], m_VAO;

    // create a Vertex Array Buffer to hold our triangle data                                               
    glGenBuffers(1, m_triangleVBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_triangleVBO[0]);

    ObjMesh obj("../../src/json/terrain.obj");

    Trees t(obj, 5.0, 50 );

    std::shared_ptr<std::vector<Triangle>> triList = std::make_shared<std::vector< Triangle >>(obj.getFaces());
    std::vector< VertexPoint > host_VertexBuffer;

    for(int i = 0; i < triList->size(); i++) {
        auto faces = triList->at(i).toVertexBuffer();
        host_VertexBuffer.insert(host_VertexBuffer.end(), faces.begin(), faces.end());
    }

    int numBytes = host_VertexBuffer.size() * sizeof(VertexPoint);

    // copy the numBytes from host_VertexBuffer t the GPU and store in                                      
    // the currently bound VBO                                                                              
    glBufferData(GL_ARRAY_BUFFER, numBytes, host_VertexBuffer.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    int vertexCount = host_VertexBuffer.size();   
    std::cout << vertexCount << std::endl;                                         
    host_VertexBuffer.clear();

    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, m_triangleVBO[0]);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPoint), (void*)offsetof(VertexPoint, point));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPoint), (void*)offsetof(VertexPoint, normal));

    glBindVertexArray(0);


    ParticleObj f1(
        "../../src/json/hand.obj",
        glm::vec4(1, 1, 1, 1),
        glm::vec4(0.0, 0.0002, 0, 0),
        10.0f, 1000, 4.0
    );

    Stars s(
        10000, glm::vec4(1, 1, 1, 1), 2.0f
    );

    f1.initParticles();
    s.initParticles();
    t.initParticles();
    f1.initBuffers();
    s.initBuffers();
    t.initBuffers();
    f1.setupAttributes();
    s.setupAttributes();
    t.setupAttributes();
    

    // Create a shader using my GLSLObject class 
    // just a single instance no need to initUpdateShader for all particle types                                                 
    sivelab::GLSLObject shader, shaderHand, updateShaderHand, shaderStars, updateShaderStars, treeShader, updateTreeShader;

    shader.addShader( "vertexShader_lambertian.glsl", sivelab::GLSLObject::VERTEX_SHADER );
    shader.addShader( "fragmentShader_lambertian.glsl", sivelab::GLSLObject::FRAGMENT_SHADER );
    shader.createProgram();

    f1.initDrawShader(shaderHand);
    s.initDrawShader(shaderStars);
    t.initDrawShader(treeShader);
    f1.initUpdateShader(updateShaderHand);
    s.initUpdateShader(updateShaderStars);
    t.initUpdateShader(updateTreeShader);

   

    
    //3D objects
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

    //particles
    GLuint projMatrixPID, viewMatrixPID, modelMatrixPID, texUnitFireID, texUnitTreeID, texUnitID;
    projMatrixPID = shaderHand.createUniform( "projMatrix" );
    viewMatrixPID = shaderHand.createUniform( "viewMatrix" );
    modelMatrixPID = shaderHand.createUniform( "modelMatrix" );
    texUnitID = shader.createUniform( "texUnit" );
    texUnitTreeID = shaderHand.createUniform( "texUnitTree" );


    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texID);
    glUniform1i(texUnitID, texID);

    treeShader.activate();
    GLuint treeTexUnitID = treeShader.createUniform("texUnitTree");
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, treeTexID);
    glUniform1i(treeTexUnitID, 1);
    treeShader.deactivate();


    glm::mat4 modelTransform = glm::mat4(1.0);

    float rot  = 0.0f;


    glm::mat4 M_normal = glm::mat4(1.0);


    glm::vec3 lightPos(-6, 2, 20);
    

    // //Shader Components
    glm::vec3 diffuse(0.263, 0.80, 0.306);
    glm::vec3 specular(1.0f, 1.0f, 1.0f);
    float shininess = 120.0f;


    double timeDiff = 0.0, startFrameTime = 0.0, endFrameTime = 0.0;


    
    float rotationAngle = 0.0f;
    
    //MESH MODE
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
    float camAngle = 0.0f;

    int current = 0;
    glEnable(GL_PROGRAM_POINT_SIZE);

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
        M_normal = glm::transpose(glm::inverse( modelTransform ));
        /* Render your objects here */

        glEnable(GL_RASTERIZER_DISCARD);

        updateShaderHand.activate();
        f1.update(current);
        updateShaderHand.deactivate();

        updateShaderStars.activate();
        s.update(current);
        updateShaderStars.deactivate();

        updateTreeShader.activate();
        t.update(current);
        updateTreeShader.deactivate();

        glDisable(GL_RASTERIZER_DISCARD);

        shader.activate();

        glBindVertexArray(m_VAO);

        // copy from the host to the device the view matrix and the projection matrix                                                                                       
        glUniformMatrix4fv(projMatrixID, 1, GL_FALSE, glm::value_ptr( M_pers ));
        glUniformMatrix4fv(viewMatrixID, 1, GL_FALSE, glm::value_ptr( M_view ));
        glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, glm::value_ptr(modelTransform));
        glUniformMatrix4fv(normalMatrixID, 1, GL_FALSE, glm::value_ptr( M_normal ));
        glUniform3fv(lightPosID, 1, glm::value_ptr(lightPos));
        glUniform3fv(camPosID, 1, glm::value_ptr(camPos));
        glUniform3fv(diffID, 1, glm::value_ptr(diffuse));
        glUniform3fv(specularID, 1, glm::value_ptr(specular));
        glUniform1f(shininessID, shininess);
        glDrawArrays(GL_TRIANGLES, 0, vertexCount);

        glBindVertexArray(0);
        shader.deactivate();
                                                                                    
        int next = 1 - current;

        shaderHand.activate();
        glUniformMatrix4fv(projMatrixPID, 1, GL_FALSE, glm::value_ptr(M_pers));
        glUniformMatrix4fv(viewMatrixPID, 1, GL_FALSE, glm::value_ptr(M_view));

        // enable additive blendig for fire effect
        glEnable(GL_BLEND);
        glBlendEquation(GL_FUNC_ADD);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);
        glDepthMask(GL_FALSE);
        
        f1.draw(next);

        glDepthMask(GL_TRUE);
        glDisable(GL_BLEND);

        shaderHand.deactivate();

        shaderStars.activate();
        glUniformMatrix4fv(projMatrixPID, 1, GL_FALSE, glm::value_ptr(M_pers));
        glUniformMatrix4fv(viewMatrixPID, 1, GL_FALSE, glm::value_ptr(M_view));

        s.draw(next);
        shaderStars.deactivate();

        treeShader.activate();
        glUniformMatrix4fv(projMatrixPID, 1, GL_FALSE, glm::value_ptr(M_pers));
        glUniformMatrix4fv(viewMatrixPID, 1, GL_FALSE, glm::value_ptr(M_view));
        t.draw(next);
        treeShader.deactivate();


        glBindVertexArray(0);
        
        current = next;

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

        else if(glfwGetKey(window, GLFW_KEY_LEFT)) {
            cam.rotateY(0.02f);
        }

        else if(glfwGetKey(window, GLFW_KEY_RIGHT)) {
            cam.rotateY(-0.02f);
        }

        else if(glfwGetKey(window, GLFW_KEY_UP)) {
            cam.rotateX(0.02f);
        }

        else if(glfwGetKey(window, GLFW_KEY_DOWN)) {
            cam.rotateX(-0.02f);
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
