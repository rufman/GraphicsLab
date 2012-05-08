################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/scene/particleEngine/Explosion.cpp \
../src/scene/particleEngine/Smoke.cpp \
../src/scene/particleEngine/Sparkle.cpp 

OBJS += \
./src/scene/particleEngine/Explosion.o \
./src/scene/particleEngine/Smoke.o \
./src/scene/particleEngine/Sparkle.o 

CPP_DEPS += \
./src/scene/particleEngine/Explosion.d \
./src/scene/particleEngine/Smoke.d \
./src/scene/particleEngine/Sparkle.d 


# Each subdirectory must supply rules for building sources it contributes
src/scene/particleEngine/%.o: ../src/scene/particleEngine/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/Developer/SDKs/MacOSX10.7.sdk/usr/X11/include/GL -I/System/Library/Frameworks/OpenGL.framework/Versions/A/Headers -I/System/Library/Frameworks/GLUT.framework/Versions/A/Headers -O0 -g3 -Wall -c -fmessage-length=0 -arch i686 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


