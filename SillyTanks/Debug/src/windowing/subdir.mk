################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/windowing/Application.cpp \
../src/windowing/Window.cpp \
../src/windowing/WindowManager.cpp 

OBJS += \
./src/windowing/Application.o \
./src/windowing/Window.o \
./src/windowing/WindowManager.o 

CPP_DEPS += \
./src/windowing/Application.d \
./src/windowing/Window.d \
./src/windowing/WindowManager.d 


# Each subdirectory must supply rules for building sources it contributes
src/windowing/%.o: ../src/windowing/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/Developer/SDKs/MacOSX10.7.sdk/usr/X11/include/GL -I/System/Library/Frameworks/OpenGL.framework/Versions/A/Headers -I/System/Library/Frameworks/GLUT.framework/Versions/A/Headers -O0 -g3 -Wall -c -fmessage-length=0 -arch i686 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


