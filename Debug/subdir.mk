################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Camera.cpp \
../FreeFlyCamera.cpp \
../Light.cpp \
../Model.cpp \
../Shader.cpp \
../Terrain.cpp \
../TerrainNode.cpp \
../VertexBuffer.cpp \
../Window.cpp \
../main.cpp \
../noiseutils.cpp 

OBJS += \
./Camera.o \
./FreeFlyCamera.o \
./Light.o \
./Model.o \
./Shader.o \
./Terrain.o \
./TerrainNode.o \
./VertexBuffer.o \
./Window.o \
./main.o \
./noiseutils.o 

CPP_DEPS += \
./Camera.d \
./FreeFlyCamera.d \
./Light.d \
./Model.d \
./Shader.d \
./Terrain.d \
./TerrainNode.d \
./VertexBuffer.d \
./Window.d \
./main.d \
./noiseutils.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


