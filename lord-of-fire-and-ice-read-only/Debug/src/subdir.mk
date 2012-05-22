################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/application.cpp \
../src/button.cpp \
../src/camera2D.cpp \
../src/camera3D.cpp \
../src/directionalLight.cpp \
../src/dock.cpp \
../src/drawable2D.cpp \
../src/fileUtils.cpp \
../src/fog.cpp \
../src/gameSaveFunctions.cpp \
../src/graphicsSettings.cpp \
../src/inputManager.cpp \
../src/introScreen.cpp \
../src/light.cpp \
../src/loadingScreen.cpp \
../src/main.cpp \
../src/map.cpp \
../src/material.cpp \
../src/menu.cpp \
../src/menuManager.cpp \
../src/npc.cpp \
../src/npcManager.cpp \
../src/pathFinding.cpp \
../src/player.cpp \
../src/positionalLight.cpp \
../src/reflectionTexture.cpp \
../src/scene.cpp \
../src/sceneryObject.cpp \
../src/sceneryObjectManager.cpp \
../src/sector.cpp \
../src/shadowTexture.cpp \
../src/sky.cpp \
../src/sun.cpp \
../src/textBox.cpp \
../src/texture.cpp \
../src/tgaTexture.cpp \
../src/time.cpp \
../src/utils.cpp \
../src/window.cpp \
../src/windowManager.cpp 

OBJS += \
./src/application.o \
./src/button.o \
./src/camera2D.o \
./src/camera3D.o \
./src/directionalLight.o \
./src/dock.o \
./src/drawable2D.o \
./src/fileUtils.o \
./src/fog.o \
./src/gameSaveFunctions.o \
./src/graphicsSettings.o \
./src/inputManager.o \
./src/introScreen.o \
./src/light.o \
./src/loadingScreen.o \
./src/main.o \
./src/map.o \
./src/material.o \
./src/menu.o \
./src/menuManager.o \
./src/npc.o \
./src/npcManager.o \
./src/pathFinding.o \
./src/player.o \
./src/positionalLight.o \
./src/reflectionTexture.o \
./src/scene.o \
./src/sceneryObject.o \
./src/sceneryObjectManager.o \
./src/sector.o \
./src/shadowTexture.o \
./src/sky.o \
./src/sun.o \
./src/textBox.o \
./src/texture.o \
./src/tgaTexture.o \
./src/time.o \
./src/utils.o \
./src/window.o \
./src/windowManager.o 

CPP_DEPS += \
./src/application.d \
./src/button.d \
./src/camera2D.d \
./src/camera3D.d \
./src/directionalLight.d \
./src/dock.d \
./src/drawable2D.d \
./src/fileUtils.d \
./src/fog.d \
./src/gameSaveFunctions.d \
./src/graphicsSettings.d \
./src/inputManager.d \
./src/introScreen.d \
./src/light.d \
./src/loadingScreen.d \
./src/main.d \
./src/map.d \
./src/material.d \
./src/menu.d \
./src/menuManager.d \
./src/npc.d \
./src/npcManager.d \
./src/pathFinding.d \
./src/player.d \
./src/positionalLight.d \
./src/reflectionTexture.d \
./src/scene.d \
./src/sceneryObject.d \
./src/sceneryObjectManager.d \
./src/sector.d \
./src/shadowTexture.d \
./src/sky.d \
./src/sun.d \
./src/textBox.d \
./src/texture.d \
./src/tgaTexture.d \
./src/time.d \
./src/utils.d \
./src/window.d \
./src/windowManager.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


