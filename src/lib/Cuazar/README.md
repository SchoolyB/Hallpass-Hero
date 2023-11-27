# Cuazar
## Description
Cuazar is a basic testing library for C code. It is designed to be simple, easy to use, and easy to integrate into existing projects.
## Including Cuazar
To include Cuazar in your project simply follow the steps below.
 
## Step 1:
Navigate to your project directory
```bash
cd your/projects/subdir
```

## Step 2:
Add the Cuazar repository as a submodule
#### NOTE:
 Ideally you'd want to add the submodule to a directory called `lib` or `libs` but you can add it anywhere you'd like.
```bash
git submodule add https://github.com/SchoolyB/Cuazar.git
```

## Step 3:
Include the Cuazar header file and source file where ever your main function is located.


```c
// main.c
#include "Cuazar/lib/Cuazar.c"
#include "Cuazar/lib/Cuazar.h"
```

### NOTE:
After including the files mentioned in step 3, you WILL NOT need to include the source file again. You will only need to include the header file `Cuazar.h` in source files where you want to use Cuazar functions. 
```C
// otherFile.c
#include "Cuazar/lib/Cuazar.h"
``` 

## Step 4:
To enable the library
Call `Init_Cuazar()` function in your main function. This will initialize the library and allow you to use the Cuazar functions.
```c
// main.c
#include "Cuazar/lib/Cuazar.c"
#include "Cuazar/lib/Cuazar.h"

int main(int argc, char** argv) {
    Init_Cuazar();
    // Your code here
    return 0;
}
```

## Step 5:
Call the Cuazar functions where needed
```C
// main.c
#include <stdio.h>
#include "Cuazar/lib/Cuazar.c"
#include "Cuazar/lib/Cuazar.h"



int main(int argc, char **argv)
{
    Init_Cuazar();
  /*
  __CUAZAR_RETURN__ tests the return value of a function
  Params:
  1 = The test number. This could be 2,3,47,50,150,etc...
  3 = The Expected return value from the function
  myCoolFunc() = The function whose return value we are testing
  */
  __CUAZAR_RETURN__(1, 3, myCoolFunc());
  return 0;
}

int myCoolFunc(void)
{
  int schoolIsCool = FALSE; // or 0

  if (schoolIsCool == TRUE)
  {
    return 1;
  }
  else
  {
    return 3;
  }
}
```
# Output:
`TEST #1 PASSED`


