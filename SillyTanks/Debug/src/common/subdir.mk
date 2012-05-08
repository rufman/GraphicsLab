################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/common/Camera2D.cpp \
../src/common/Camera3D.cpp \
../src/common/Material.cpp \
../src/common/PLYModel.cpp \
../src/common/TGAImage.cpp \
../src/common/TGATexture.cpp \
../src/common/TextBox.cpp \
../src/common/Texture.cpp \
../src/common/Time.cpp \
../src/common/Types.cpp \
../src/common/Utils.cpp 

OBJS += \
./src/common/Camera2D.o \
./src/common/Camera3D.o \
./src/common/Material.o \
./src/common/PLYModel.o \
./src/common/TGAImage.o \
./src/common/TGATexture.o \
./src/common/TextBox.o \
./src/common/Texture.o \
./src/common/Time.o \
./src/common/Types.o \
./src/common/Utils.o 

CPP_DEPS += \
./src/common/Camera2D.d \
./src/common/Camera3D.d \
./src/common/Material.d \
./src/common/PLYModel.d \
./src/common/TGAImage.d \
./src/common/TGATexture.d \
./src/common/TextBox.d \
./src/common/Texture.d \
./src/common/Time.d \
./src/common/Types.d \
./src/common/Utils.d 


# Each subdirectory must supply rules for building sources it contributes
src/common/%.o: ../src/common/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/Developer/SDKs/MacOSX10.7.sdk/usr/X11/include/GL -I/System/Library/Frameworks/OpenGL.framework/Versions/A/Headers -I/System/Library/Frameworks/GLUT.framework/Versions/A/Headers -O0 -g3 -Wall -c -fmessage-length=0 -arch i686 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


