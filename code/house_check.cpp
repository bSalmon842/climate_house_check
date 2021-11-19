/*
Project: Climate Impact
File: house_check.cpp
Author: Brock Salmon
Notice: (C) Copyright 2021 by Brock Salmon. All Rights Reserved
*/

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "glad/glad.h"

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

#include "house_check_utility.h"

enum CountrySelect
{
    Country_Australia,
    Country_UnitedStates,
    Country_UnitedKingdom,
    Country_China,
};

s32 main(s32 argc, char **argv)
{
    b32 glfwInitResult = glfwInit();
    ASSERT(glfwInitResult);
    
    char *glslVersion = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    GLFWwindow *window = glfwCreateWindow(720, 360, "Climate Impact", 0, 0);
    ASSERT(window);
    
    glfwMakeContextCurrent(window);
    
    b32 gladResult = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    ASSERT(gladResult);
    
    glfwSwapInterval(1);
    
    s32 screenWidth, screenHeight;
    glfwGetFramebufferSize(window, &screenWidth, &screenHeight);
    glViewport(0, 0, screenWidth, screenHeight);
    
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glslVersion);
    
    ImGui::StyleColorsDark();
    
    f32 kwh = 0.0f;
    f32 co2 = 0.0f;
    s32 currCountry = Country_Australia;
    char *countryStrings[] = {"Australian", "United States", "United Kingdom", "Chinese"};
    
    f32 kwhAve[] = {19.0f, 30.0f, 8.0f, 28.5f};
    f32 co2Ave[] = {12.471f, 20.55f, 48.0f, 44.93f};
    f32 co2RangeLo[] = {10.83f, 15.0f, 46.85f, 38.0f};
    f32 co2RangeUp[] = {26.256f, 30.0f, 55.34f, 50.0f};
    
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        
        ImGui::SetNextWindowPos(ImVec2(screenWidth * 0.025f, screenHeight * 0.025f));
        ImGui::SetNextWindowSize(ImVec2(screenWidth * 0.95f, screenHeight * 0.3f));
        ImGui::Begin("Input", 0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
        {
            ImGui::Text("Household average Kilowatts per Hour: ");
            ImGui::SameLine();
            ImGui::PushItemWidth(screenWidth * 0.4f);
            ImGui::InputFloat("", &kwh, 0.0f, 0.0f, "%.2f");
            if (kwh < 0.0f)
            {
                kwh = 0.0f;
            }
            else if (kwh > 1000.0f)
            {
                kwh = 1000.0f;
            }
            ImGui::PopItemWidth();
            
            ImGui::NewLine();
            if (ImGui::Button("Compare to AU"))
            {
                currCountry = Country_Australia;
            }
            
            ImGui::SameLine();
            if (ImGui::Button("Compare to US"))
            {
                currCountry = Country_UnitedStates;
            }
            
            ImGui::SameLine();
            if (ImGui::Button("Compare to UK"))
            {
                currCountry = Country_UnitedKingdom;
            }
            
            ImGui::SameLine();
            if (ImGui::Button("Compare to CH"))
            {
                currCountry = Country_China;
            }
        }
        ImGui::End();
        
        ImGui::SetNextWindowPos(ImVec2(screenWidth * 0.025f, screenHeight * 0.375f));
        ImGui::SetNextWindowSize(ImVec2(screenWidth * 0.95f, screenHeight * 0.60f));
        ImGui::Begin("Output", 0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
        
        if (kwh > 0.0f)
        {
            co2 = (kwh * 656.4f) / 1000.0f;
            ImGui::Text("Your household is producing approx %.02f kilograms of CO2 per hour", co2); // Total CO2 emissions
            
            ImGui::NewLine();
            if (kwh < kwhAve[currCountry])
            {
                ImGui::Text("Your household is using %.02fkw/h less electricity than the average %s Household", Abs(kwh - kwhAve[currCountry]), countryStrings[currCountry]);
            }
            else if (kwh > kwhAve[currCountry])
            {
                ImGui::Text("Your household is using %.02fkw/h more electricity than the average %s Household", Abs(kwh - kwhAve[currCountry]), countryStrings[currCountry]);
            }
            else
            {
                ImGui::Text("Your household is using the same amount of electricity as the average %s Household", countryStrings[currCountry]);
            }
            
            ImGui::NewLine();
            
            if (co2 >= co2RangeLo[currCountry] && co2 <= co2RangeUp[currCountry])
            {
                ImGui::Text("Your household is in the average emissions range for %s Households", countryStrings[currCountry]);
            }
            else if (co2 < co2RangeLo[currCountry])
            {
                ImGui::Text("Your household is below the emissions range for %s Households", countryStrings[currCountry]);
            }
            else if (co2 > co2RangeUp[currCountry])
            {
                ImGui::Text("Your household is above the emissions range for %s Households", countryStrings[currCountry]);
            }
            
            if (co2 < co2Ave[currCountry])
            {
                ImGui::Text("Your household is creating %.02fkg less CO2 per hour than the average %s Household", Abs(co2 - co2Ave[currCountry]), countryStrings[currCountry]);
            }
            else if (co2 > co2Ave[currCountry])
            {
                ImGui::Text("Your household is creating %.02fkg more CO2 per hourthan the average %s Household", Abs(co2 - co2Ave[currCountry]), countryStrings[currCountry]);
            }
            else
            {
                ImGui::Text("Your household is creating the same amount of CO2 per hour as the average %s Household", countryStrings[currCountry]);
            }
        }
        
        ImGui::End();
        
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        
        glfwGetFramebufferSize(window, &screenWidth, &screenHeight);
        glViewport(0, 0, screenWidth, screenHeight);
        glfwSwapBuffers(window);
    }
    
    return 0;
}