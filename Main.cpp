#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Camera.h"
#include "Model.h"
#include "ModelMatrix.h"
#include "Material.h"
#include "Shape.h"
#include "Cube.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "Spotlight.h"

#include <iostream>

// gets called each time window is resized so that viewport (rendering window) is resized as well
void framebufferSizeCallback(GLFWwindow* window, int width, int height);

void mouseCallback(GLFWwindow* window, double xpos, double ypos);

// for zooming in and out of scenes
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);

void processKeyInput(GLFWwindow* window);

void renderLoadingDialog();

unsigned int loadTexture(const char* path);

std::string modelUserInput;
std::string shapeDiffuseUserInput;
std::string shapeSpecularUserInput;
std::string inputType;
bool diffuseMapSelected = false;
bool specularMapSelected = false;
char inputBuffer[256] = { 0 };

// Global variables for storing dialog state
bool showLoadingDialog = false;
bool loadingDialogIsOpen = false;

// settings
const unsigned int SCREEN_WIDTH = 1920;
const unsigned int SCREEN_HEIGHT = 1080;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 10.0f));
float lastX = SCREEN_WIDTH / 2.0f;
float lastY = SCREEN_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

float backgroundColor[4] = { 0.68f, 0.85f, 0.9f, 1.0f };  // light blue

bool cursorIsEnabled = false;

std::vector<Model> models;
std::vector<int> modelsToDelete;

glm::mat4 projection;
glm::mat4 view;

// materials (from http://devernay.free.fr/cours/opengl/materials.html):

// metals and stones
Material emeraldMaterial(glm::vec3(0.0215f, 0.1745f, 0.0215f), glm::vec3(0.07568f, 0.61424f, 0.07568f), glm::vec3(0.633f, 0.727811f, 0.633f), 0.6f);
Material jadeMaterial(glm::vec3(0.135f, 0.2225f, 0.1575f), glm::vec3(0.54f, 0.89f, 0.63f), glm::vec3(0.316228f, 0.316228f, 0.316228f), 0.1f);
Material obsidianMaterial(glm::vec3(0.05375f, 0.05f, 0.06625f), glm::vec3(0.18275f, 0.17f, 0.22525f), glm::vec3(0.332741f, 0.328634f, 0.346435f), 0.3f);
Material pearlMaterial(glm::vec3(0.25f, 0.20725f, 0.20725f), glm::vec3(1.0f, 0.829f, 0.829f), glm::vec3(0.296648f, 0.296648f, 0.296648f), 0.088f);
Material rubyMaterial(glm::vec3(0.1745f, 0.01175f, 0.01175f), glm::vec3(0.61424f, 0.04136f, 0.04136f), glm::vec3(0.727811f, 0.626959f, 0.626959f), 0.6f);
Material turquoiseMaterial(glm::vec3(0.1f, 0.18725f, 0.1745f), glm::vec3(0.396f, 0.74151f, 0.69102f), glm::vec3(0.297254f, 0.30829f, 0.306678f), 0.1f);
Material brassMaterial(glm::vec3(0.329412f, 0.223529f, 0.027451f), glm::vec3(0.780392f, 0.780392f, 0.113725f), glm::vec3(0.992157f, 0.941176f, 0.807843f), 0.21794872f);
Material bronzeMaterial(glm::vec3(0.2125f, 0.1275f, 0.054f), glm::vec3(0.714f, 0.4284f, 0.18144f), glm::vec3(0.393548f, 0.271906f, 0.166721f), 0.2f);
Material chromeMaterial(glm::vec3(0.25f, 0.25f, 0.25f), glm::vec3(0.4f, 0.4f, 0.4f), glm::vec3(0.774597f, 0.774597f, 0.774597f), 0.6f);
Material copperMaterial(glm::vec3(0.19125f, 0.0735f, 0.0225f), glm::vec3(0.7038f, 0.27048f, 0.0828f), glm::vec3(0.256777f, 0.137622f, 0.086014f), 0.1f);
Material goldMaterial(glm::vec3(0.24725f, 0.1995f, 0.0745f), glm::vec3(0.75164f, 0.60648f, 0.22648f), glm::vec3(0.628281f, 0.555802f, 0.366065f), 0.4f);
Material silverMaterial(glm::vec3(0.19225f, 0.19225f, 0.19225f), glm::vec3(0.50754f, 0.50754f, 0.50754f), glm::vec3(0.508273f, 0.508273f, 0.508273f), 0.4f);

// plastics
Material blackPlasticMaterial(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.01f, 0.01f, 0.01f), glm::vec3(0.5f, 0.5f, 0.5f), 0.25f);
Material cyanPlasticMaterial(glm::vec3(0.0f, 0.1f, 0.06f), glm::vec3(0.0f, 0.50980392f, 0.50980392f), glm::vec3(0.50196078f, 0.50196078f, 0.50196078f), 0.25f);
Material greenPlasticMaterial(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.1f, 0.35f, 0.1f), glm::vec3(0.45f, 0.55f, 0.45f), 0.25f);
Material redPlasticMaterial(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.5f, 0.0f, 0.0f), glm::vec3(0.7f, 0.6f, 0.6f), 0.25f);
Material whitePlasticMaterial(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.55f, 0.55f, 0.55f), glm::vec3(0.7f, 0.7f, 0.7f), 0.25f);
Material yellowPlasticMaterial(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.5f, 0.5f, 0.0f), glm::vec3(0.6f, 0.6f, 0.5f), 0.25f);

// rubbers
Material blackRubberMaterial(glm::vec3(0.02f, 0.02f, 0.02f), glm::vec3(0.01f, 0.01f, 0.01f), glm::vec3(0.4f, 0.4f, 0.4f), 0.078125f);
Material cyanRubberMaterial(glm::vec3(0.0f, 0.05f, 0.05f), glm::vec3(0.4f, 0.5f, 0.5f), glm::vec3(0.04f, 0.7f, 0.7f), 0.078125f);
Material greenRubberMaterial(glm::vec3(0.0f, 0.05f, 0.0f), glm::vec3(0.4f, 0.5f, 0.4f), glm::vec3(0.04f, 0.7f, 0.04f), 0.078125f);
Material redRubberMaterial(glm::vec3(0.05f, 0.0f, 0.0f), glm::vec3(0.5f, 0.4f, 0.4f), glm::vec3(0.7f, 0.04f, 0.04f), 0.078125f);
Material whiteRubberMaterial(glm::vec3(0.05f, 0.05f, 0.05f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.7f, 0.7f, 0.7f), 0.078125f);
Material yellowRubberMaterial(glm::vec3(0.05f, 0.05f, 0.0f), glm::vec3(0.5f, 0.5f, 0.4f), glm::vec3(0.7f, 0.7f, 0.04f), 0.078125f);

Material customMaterial(glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.5f, 0.5f, 0.5f), 0.5);

Material materials[] = { customMaterial, emeraldMaterial, jadeMaterial, obsidianMaterial, pearlMaterial, rubyMaterial, turquoiseMaterial, brassMaterial, bronzeMaterial, chromeMaterial, copperMaterial,
                         goldMaterial, silverMaterial, blackPlasticMaterial, cyanPlasticMaterial, greenPlasticMaterial, redPlasticMaterial, whitePlasticMaterial, yellowPlasticMaterial,
                         blackRubberMaterial, cyanRubberMaterial, greenRubberMaterial, redRubberMaterial, whiteRubberMaterial, yellowRubberMaterial };
const char* materialNames[] = { "Custom", "Emerald", "Jade", "Oobsidian", "Pearl", "Ruby", "Turquoise", "Brass", "Bronze", "Chrome", "Copper", "Gold", "Silver", "Black Plastic", "Cyan Plastic",
                               "Green Plastic", "Red Plastic", "White Plastic", "Yellow Plastic", "Black Rubber", "Cyan Rubber", "Green Rubber", "Red Rubber", "White Rubber", "Yellow Rubber" };
const int materialCount = IM_ARRAYSIZE(materialNames);

std::vector<Shape*> shapes;
std::vector<int> shapesToDelete;

glm::mat4 model;

float step = 1.0f;
float stepFast = 5.0f;

std::vector<DirectionalLight> dirLights;
std::vector<PointLight> pointLights;
std::vector<Spotlight> spotLights;      // right now only 1 spotlight (1st person pov flashlight)

bool flashlightIsEnabled = false;

std::vector<int> dirLightsToDelete;
std::vector<int> pointLightsToDelete;

const int MAX_NUM_LIGHTS = 10;

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Engine", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetScrollCallback(window, scrollCallback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
    stbi_set_flip_vertically_on_load(true);

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile shaders
    // -------------------------
    Shader materialLightingShader("Vertex Shaders/material_lighting_vs.txt", "Fragment Shaders/material_lighting_fs.txt");
    Shader lightSourceShader("Vertex Shaders/light_source_vs.txt", "Fragment Shaders/light_source_fs.txt");
    Shader modelShader("Vertex Shaders/model_vs.txt", "Fragment Shaders/model_fs.txt");
    Shader textureLightingShader("Vertex Shaders/texture_lighting_vs.txt", "Fragment Shaders/texture_lighting_fs.txt");
 
    // imgui initialization
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processKeyInput(window);

        // let imgui know you're on new frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // imgui window
        ImGui::Begin("Main Window");

        ImGui::PushItemWidth(100);  // sets width of all input textboxes to 100 px

        // Create a tab bar
        ImGui::Text("SETTINGS");
        if (ImGui::BeginTabBar("SETTINGS"))
        {
            // render
            // ------
            if (ImGui::BeginTabItem("GENERAL"))
            {
                ImGui::SetNextItemWidth(200);   // since this textbox has a different initalize size than others
                ImGui::ColorEdit4("Background Color", backgroundColor);

                ImGui::EndTabItem();
            }
            glClearColor(backgroundColor[0], backgroundColor[1], backgroundColor[2], backgroundColor[3]);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // camera imgui options
            if (ImGui::BeginTabItem("CAMERA"))
            {
                ImGui::InputFloat("Camera Movement Speed", &camera.MovementSpeed, 0.1f, 1.0f, "%.3f");
                ImGui::InputFloat("Camera Mouse Sensitivity", &camera.MouseSensitivity, 0.1f, 1.0f, "%.3f");
                if (ImGui::Button("Reset Camera"))
                {
                    // only position and axes get reset
                    Camera resetCam(glm::vec3(0.0f, 0.0f, 10.0f));
                    resetCam.MovementSpeed = camera.MovementSpeed;
                    resetCam.MouseSensitivity = camera.MouseSensitivity;
                    camera = resetCam;
                }

                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("LIGHT"))
            {
                if (ImGui::Button("Add Directional Light") && dirLights.size() < MAX_NUM_LIGHTS)
                {
                    dirLights.push_back(DirectionalLight());
                }
                ImGui::SameLine();
                if (ImGui::Button("Add Point Light") && pointLights.size() < MAX_NUM_LIGHTS)
                {
                    pointLights.push_back(PointLight());
                }
                ImGui::SameLine();
                if (ImGui::Button("Delete ALL Lights"))
                {
                    dirLights.clear();
                    pointLights.clear();
                }

                // directional light options
                for (int i = 0; i < dirLights.size(); i++) {
                    string idNum = std::to_string(i);

                    ImGui::Text(("Directional Light " + idNum + " Settings").c_str());

                    ImGui::Checkbox(("Enable##_dir_light_" + idNum).c_str(), &dirLights[i].isEnabled);

                    glm::vec3 amb = dirLights[i].ambient;
                    glm::vec3 diff = dirLights[i].diffuse;
                    glm::vec3 spec = dirLights[i].specular;

                    float ambientIntensity[3] = { amb[0], amb[1], amb[2] };
                    float diffuseIntensity[3] = { diff[0], diff[1], diff[2] };
                    float specularIntensity[3] = { spec[0], spec[1], spec[2] };

                    ImGui::ColorEdit3(("Ambient Intensity##_dir_light_" + idNum).c_str(), ambientIntensity);
                    ImGui::ColorEdit3(("Diffuse Intensity##_dir_light_" + idNum).c_str(), diffuseIntensity);
                    ImGui::ColorEdit3(("Specular Intensity##_dir_light_" + idNum).c_str(), specularIntensity);

                    dirLights[i].ambient = glm::make_vec3(ambientIntensity);
                    dirLights[i].diffuse = glm::make_vec3(diffuseIntensity);
                    dirLights[i].specular = glm::make_vec3(specularIntensity);

                    ImGui::Text("Direction:");
                    ImGui::InputFloat(("X##_light_dir_" + idNum).c_str(), &dirLights[i].direction[0], 0.1f, 1.0f, "%.3f");
                    ImGui::SameLine();
                    ImGui::InputFloat(("Y##_light_dir_" + idNum).c_str(), &dirLights[i].direction[1], 0.1f, 1.0f, "%.3f");
                    ImGui::SameLine();
                    ImGui::InputFloat(("Z##_light_dir_" + idNum).c_str(), &dirLights[i].direction[2], 0.1f, 1.0f, "%.3f");

                    if (ImGui::Button(("Delete Directional Light " + idNum).c_str()))
                    {
                        dirLightsToDelete.push_back(i);
                    }
                }

                ImGui::Spacing();
                ImGui::Separator();
                ImGui::Spacing();

                for (int i = 0; i < pointLights.size(); i++) {
                    string idNum = std::to_string(i);

                    ImGui::Text(("Point Light " + idNum + " Settings").c_str());

                    ImGui::Checkbox(("Enable##_point_light_" + idNum).c_str(), &pointLights[i].isEnabled);

                    glm::vec3 amb = pointLights[i].ambient;
                    glm::vec3 diff = pointLights[i].diffuse;
                    glm::vec3 spec = pointLights[i].specular;

                    float ambientIntensity[3] = { amb[0], amb[1], amb[2] };
                    float diffuseIntensity[3] = { diff[0], diff[1], diff[2] };
                    float specularIntensity[3] = { spec[0], spec[1], spec[2] };

                    ImGui::ColorEdit3(("Ambient Intensity##_point_light_" + idNum).c_str(), ambientIntensity);
                    ImGui::ColorEdit3(("Diffuse Intensity##_point_light_" + idNum).c_str(), diffuseIntensity);
                    ImGui::ColorEdit3(("Specular Intensity##_point_light_" + idNum).c_str(), specularIntensity);

                    pointLights[i].ambient = glm::make_vec3(ambientIntensity);
                    pointLights[i].diffuse = glm::make_vec3(diffuseIntensity);
                    pointLights[i].specular = glm::make_vec3(specularIntensity);

                    ImGui::Text("Position:");
                    ImGui::InputFloat(("X##_point_light_pos_" + idNum).c_str(), &pointLights[i].position[0], 0.1f, 1.0f, "%.3f");
                    ImGui::SameLine();
                    ImGui::InputFloat(("Y##_point_light_pos_" + idNum).c_str(), &pointLights[i].position[1], 0.1f, 1.0f, "%.3f");
                    ImGui::SameLine();
                    ImGui::InputFloat(("Z##_point_light_pos_" + idNum).c_str(), &pointLights[i].position[2], 0.1f, 1.0f, "%.3f");

                    if (ImGui::Button(("Delete Point Light " + idNum).c_str()))
                    {
                        pointLightsToDelete.push_back(i);
                    }
                }

                ImGui::Spacing();
                ImGui::Separator();
                ImGui::Spacing();

                ImGui::Checkbox("Enable Flashlight", &flashlightIsEnabled);

                ImGui::EndTabItem();
            }

            for (int j : dirLightsToDelete) {
                dirLights.erase(dirLights.begin() + j);
            }
            dirLightsToDelete.clear(); // to prevent vector iterator going out of bounds

            for (int j : pointLightsToDelete) {
                pointLights.erase(pointLights.begin() + j);
            }
            pointLightsToDelete.clear(); // to prevent vector iterator going out of bounds

            // activate material light shader
            materialLightingShader.use();
            materialLightingShader.setVec3("viewPos", camera.Position);

            materialLightingShader.setInt("numDirLights", 0);
            materialLightingShader.setInt("numPointLights", 0);
            materialLightingShader.setInt("numSpotLights", 1);

            // directional lights
            DirectionalLight::setShaderVariables(materialLightingShader, dirLights);

            // point lights
            PointLight::setShaderVariables(materialLightingShader, pointLights);

            // spotLight
            Spotlight::setShaderVariables(materialLightingShader, camera, flashlightIsEnabled);

            if (ImGui::BeginTabItem("SHAPE"))
            {
                if (ImGui::Button("Add Cube"))
                {
                    shapes.push_back(new Cube());
                }
                ImGui::SameLine();
                if (ImGui::Button("Delete ALL Cubes"))
                {
                    for (Shape* s : shapes) delete s;   // deallocate mem. since it was init. with "new"
                    shapes.clear();
                }

                // Create the dropdown menu
                for (int i = 0; i < shapes.size(); i++) {
                    string idNum = std::to_string(i);

                    ImGui::Text(("Shape " + idNum + " Settings").c_str());

                    if (ImGui::Combo(("Material selection##" + idNum).c_str(), &shapes[i]->selectedMaterial, materialNames, materialCount))
                    {
                        shapes[i]->material = materials[shapes[i]->selectedMaterial];
                    }
                    if (shapes[i]->selectedMaterial == 0) {
                        glm::vec3 amb = shapes[i]->material.getAmbientIntensity();
                        glm::vec3 diff = shapes[i]->material.getDiffuseColor();
                        glm::vec3 spec = shapes[i]->material.getSpecularColor();
                        float shin = shapes[i]->material.getShininess() / 128.0f;

                        float ambientIntensity[3] = { amb[0], amb[1], amb[2] };
                        float diffuseColor[3] = { diff[0], diff[1], diff[2] };
                        float specularColor[3] = { spec[0], spec[1], spec[2] };

                        ImGui::ColorEdit3(("Ambient Intensity##" + idNum).c_str(), ambientIntensity);
                        ImGui::ColorEdit3(("Diffuse Color##" + idNum).c_str(), diffuseColor);
                        ImGui::ColorEdit3(("Specular Color##" + idNum).c_str(), specularColor);
                        ImGui::SliderFloat(("Shininess##" + idNum).c_str(), &shin, 0.0f, 1.0f);

                        shapes[i]->material = Material(glm::make_vec3(ambientIntensity), glm::make_vec3(diffuseColor), glm::make_vec3(specularColor), shin);
                    }
                    if (ImGui::Button(("Add Diffuse Map to Shape " + idNum).c_str()))
                    {
                        showLoadingDialog = true;
                        inputType = "Texture";
                        diffuseMapSelected = true;
                    }
                    if (ImGui::Button(("Add Specular Map to Shape " + idNum).c_str()))
                    {
                        showLoadingDialog = true;
                        inputType = "Texture";
                        specularMapSelected = true;
                    }
                    if (!shapeDiffuseUserInput.empty())
                    {
                        shapes[i]->diffuseMap = loadTexture(("Textures/" + shapeDiffuseUserInput).c_str());

                        shapeDiffuseUserInput.clear();
                        diffuseMapSelected = false;

                    }
                    if (!shapeSpecularUserInput.empty())
                    {
                        shapes[i]->specularMap = loadTexture(("Textures/" + shapeSpecularUserInput).c_str());

                        shapeSpecularUserInput.clear();
                        specularMapSelected = false;
                    }

                    // world transformation
                    ImGui::Text("Scale:");
                    ImGui::InputFloat(("X##_shp_scl_" + idNum).c_str(), &shapes[i]->matrix.scale[0], 0.1f, 1.0f, "%.3f");
                    ImGui::SameLine();
                    ImGui::InputFloat(("Y##_shp_scl_" + idNum).c_str(), &shapes[i]->matrix.scale[1], 0.1f, 1.0f, "%.3f");
                    ImGui::SameLine();
                    ImGui::InputFloat(("Z##_shp_scl_" + idNum).c_str(), &shapes[i]->matrix.scale[2], 0.1f, 1.0f, "%.3f");

                    ImGui::Text("Rotation:");
                    ImGui::InputFloat(("X##_shp_rot_" + idNum).c_str(), &shapes[i]->matrix.rotation[0], step, stepFast, "%.3f");
                    ImGui::SameLine();
                    ImGui::InputFloat(("Y##_shp_rot_" + idNum).c_str(), &shapes[i]->matrix.rotation[1], step, stepFast, "%.3f");
                    ImGui::SameLine();
                    ImGui::InputFloat(("Z##_shp_rot_" + idNum).c_str(), &shapes[i]->matrix.rotation[2], step, stepFast, "%.3f");

                    ImGui::Text("Translation:");
                    ImGui::InputFloat(("X##_shp_transl_" + idNum).c_str(), &shapes[i]->matrix.translation[0], step, stepFast, "%.3f");
                    ImGui::SameLine();
                    ImGui::InputFloat(("Y##_shp_transl_" + idNum).c_str(), &shapes[i]->matrix.translation[1], step, stepFast, "%.3f");
                    ImGui::SameLine();
                    ImGui::InputFloat(("Z##_shp_transl_" + idNum).c_str(), &shapes[i]->matrix.translation[2], step, stepFast, "%.3f");

                    if (ImGui::Button(("Delete Shape " + idNum).c_str()))
                    {
                        shapesToDelete.push_back(i);
                    }
                }

                ImGui::EndTabItem();
            }

            for (int i = 0; i < shapes.size(); i++) {
                shapes[i]->material.setShaderVariables(materialLightingShader);

                // view/projection transformations for shapes
                projection = glm::perspective(glm::radians(camera.Zoom), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
                view = camera.GetViewMatrix();

                materialLightingShader.setMat4("projection", projection);
                materialLightingShader.setMat4("view", view);
                materialLightingShader.setMat4("model", shapes[i]->matrix.getMatrix());

                // render the cube
                shapes[i]->draw();
            }
            for (int j : shapesToDelete) {
                delete shapes[j];
                shapes.erase(shapes.begin() + j);
            }
            shapesToDelete.clear(); // to prevent vector iterator going out of bounds
            
            // also draw the lamp object(s)
            lightSourceShader.use();

            for (int i = 0; i < pointLights.size(); i++) {
                projection = glm::perspective(glm::radians(camera.Zoom), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
                view = camera.GetViewMatrix();

                lightSourceShader.setMat4("projection", projection);
                lightSourceShader.setMat4("view", view);

                pointLights[i].shape.matrix.translation = pointLights[i].position;
                lightSourceShader.setMat4("model", pointLights[i].shape.matrix.getMatrix());

                lightSourceShader.setBool("lightSourceIsEnabled", pointLights[i].isEnabled);

                pointLights[i].shape.draw();
            }

            // don't forget to enable shader before setting uniforms
            modelShader.use();
            modelShader.setVec3("viewPos", camera.Position);
            modelShader.setInt("numDirLights", 0);
            modelShader.setInt("numPointLights", 0);
            modelShader.setInt("numSpotLights", 1);

            // for model loading
            if (ImGui::BeginTabItem("MODEL"))
            {
                if (ImGui::Button("Load Model"))
                {
                    showLoadingDialog = true;
                    inputType = "Model";
                }
                ImGui::SameLine();
                if (ImGui::Button("Delete ALL Models"))
                {
                    models.clear();
                }

                if (!modelUserInput.empty())
                {
                    models.push_back(Model("Models/" + modelUserInput));     // add model itself with meshes and textures

                    if (!models.back().fileExists) {
                        models.pop_back();  // doesn't make sense to keep a model that has an invalid path
                    }

                    modelUserInput.clear();
                }

                // provides functionality to modify scale, rotation, and translation in real time, delete and draws models
                for (int i = 0; i < models.size(); i++) {
                    string idNum = std::to_string(i);

                    ImGui::Text(("Model " + idNum + " Settings").c_str());

                    // render the loaded model
                    ImGui::Text("Scale:");      // uniform scaling for now... 
                    ImGui::InputFloat(("XYZ##_mdl_scl_" + idNum).c_str(), &models[i].matrix.scale[0], 0.1f, 1.0f, "%.3f");
                    models[i].matrix.scale = glm::vec3(models[i].matrix.scale[0], models[i].matrix.scale[0], models[i].matrix.scale[0]);

                    ImGui::Text("Rotation:");
                    ImGui::InputFloat(("X##_mdl_rot_" + idNum).c_str(), &models[i].matrix.rotation[0], step, stepFast, "%.3f");
                    ImGui::SameLine();
                    ImGui::InputFloat(("Y##_mdl_rot_" + idNum).c_str(), &models[i].matrix.rotation[1], step, stepFast, "%.3f");
                    ImGui::SameLine();
                    ImGui::InputFloat(("Z##_mdl_rot_" + idNum).c_str(), &models[i].matrix.rotation[2], step, stepFast, "%.3f");

                    ImGui::Text("Translation:");
                    ImGui::InputFloat(("X##_mdl_transl_" + idNum).c_str(), &models[i].matrix.translation[0], step, stepFast, "%.3f");
                    ImGui::SameLine();
                    ImGui::InputFloat(("Y##_mdl_transl_" + idNum).c_str(), &models[i].matrix.translation[1], step, stepFast, "%.3f");
                    ImGui::SameLine();
                    ImGui::InputFloat(("Z##_mdl_transl_" + idNum).c_str(), &models[i].matrix.translation[2], step, stepFast, "%.3f");

                    ImGui::SliderFloat(("Shininess##_mdl_" + idNum).c_str(), &models[i].shininess, 0.0f, 1.0f);

                    // button options
                    if (ImGui::Button(("Reset Model " + idNum).c_str()))
                    {
                        models[i].matrix.scale = glm::vec3(1.0f, 1.0f, 1.0f);
                        models[i].matrix.rotation = glm::vec3();
                        models[i].matrix.translation = glm::vec3();
                    }
                    ImGui::SameLine();
                    if (ImGui::Button(("Focus On Model " + idNum).c_str()))
                    {
                        Camera focusCam(glm::vec3(models[i].matrix.translation[0], models[i].matrix.translation[1], models[i].matrix.translation[2] + 10.0f));
                        focusCam.MovementSpeed = camera.MovementSpeed;
                        focusCam.MouseSensitivity = camera.MouseSensitivity;
                        camera = focusCam;
                    }
                    ImGui::SameLine();
                    if (ImGui::Button(("Delete Model " + idNum).c_str()))
                    {
                        modelsToDelete.push_back(i);
                    }
                }

                ImGui::EndTabItem();
            }

            // directional lights
            DirectionalLight::setShaderVariables(modelShader, dirLights);

            // point lights
            PointLight::setShaderVariables(modelShader, pointLights);

            // spotLight
            Spotlight::setShaderVariables(modelShader, camera, flashlightIsEnabled);

            // view/projection transformations for model
            for (int i = 0; i < models.size(); i++) {
                projection = glm::perspective(glm::radians(camera.Zoom), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
                view = camera.GetViewMatrix();
                modelShader.setMat4("projection", projection);
                modelShader.setMat4("view", view);
                modelShader.setMat4("model", models[i].matrix.getMatrix());
                modelShader.setFloat("shininess", models[i].shininess * 128.0f);
                models[i].Draw(modelShader);
            }

        ImGui::EndTabBar();
        }

        for (int j : modelsToDelete) {
            models.erase(models.begin() + j);
        }
        modelsToDelete.clear(); // to prevent vector iterator going out of bounds

        ImGui::PopItemWidth();  // remove input textbox width

        // end imgui window (not shutdown)
        ImGui::End();

        // Render the input dialog
        renderLoadingDialog();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    //glDeleteVertexArrays(1, &cubeVAO);
    //glDeleteVertexArrays(1, &lightCubeVAO);
    //glDeleteBuffers(1, &VBO);

    // stop imgui
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();

    // Clean up
    for (Shape* shape : shapes) {
        delete shape;  // Delete each object
    }
    shapes.clear();  // Optionally clear the vector

    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processKeyInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (!loadingDialogIsOpen) {    // so that camera doesnt move and cursor doesnt dissappear when trying to load model
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            camera.ProcessKeyboard(FORWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            camera.ProcessKeyboard(BACKWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            camera.ProcessKeyboard(LEFT, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            camera.ProcessKeyboard(RIGHT, deltaTime);

        if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
            if (cursorIsEnabled) {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                cursorIsEnabled = false;
            }
            else {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                cursorIsEnabled = true;
            }
        }
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouseCallback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    if (!cursorIsEnabled) camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    if (!cursorIsEnabled) camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

// Function to render the model loading dialog
void renderLoadingDialog()
{
    std::string userInput;

    if (showLoadingDialog)
    {
        ImGui::OpenPopup((inputType + " Loading").c_str());
        showLoadingDialog = false; // Reset the flag so the popup only opens once
        loadingDialogIsOpen = true; // so that pressing "F" (to disable camera rotation and movement) doesn't make cursor disappear
    }

    if (ImGui::BeginPopupModal((inputType + " Loading").c_str(), NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Text(("Enter the path of the file, starting from the \"" + inputType + "s\" folder, excluding \"" + inputType + "s\":").c_str());

        // Create an input text field
        ImGui::InputText("##input", inputBuffer, IM_ARRAYSIZE(inputBuffer));

        if (ImGui::Button("Submit"))
        {
            // Process the input
            userInput = inputBuffer;
            ImGui::CloseCurrentPopup();
            loadingDialogIsOpen = false;

            // Clear the input buffer 
            std::memset(inputBuffer, 0, sizeof(inputBuffer));
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel"))
        {
            ImGui::CloseCurrentPopup();
            loadingDialogIsOpen = false;

            // Clear the input buffer 
            std::memset(inputBuffer, 0, sizeof(inputBuffer));
        }

        ImGui::EndPopup();
    }

    if (inputType == "Model") modelUserInput = userInput;
    else if (inputType == "Texture" && diffuseMapSelected) shapeDiffuseUserInput = userInput;
    else if (inputType == "Texture" && specularMapSelected) shapeSpecularUserInput = userInput;
}

// utility function for loading a 2D texture from file
// ---------------------------------------------------
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