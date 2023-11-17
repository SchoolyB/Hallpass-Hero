# This file compile the main.c file in the src/C folder into a temp.exe file in the build folder
gcc -o ../build/temp.exe  ../src/c/main.c


if [ $? -eq 0 ]; then
    echo "\e[32mBuild Successful!\e[0m" 
else
    echo "\e[31mBuild Failed.\e[0m"
    exit 1
fi
