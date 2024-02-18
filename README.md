# jogoNomeFantasia

# Ubuntu

## Intalar o GLFW
```
sudo apt-get install libglfw3
sudo apt-get install libglfw3-dev
```
## Instalar a GLAD

```


```
## Colocar no MakeFile:
Clang 
GCC
mingW


## Para compilar 
```
gcc main.c -lglfw3 -lGL -lX11 -lpthread -lXrandr -lXI -ldl^C

```

```
filhoalexandre@NewBubunto:~/Documents/jogoNomeFantasia$ gcc main.c -lglfw3 -lGL -lX11 -lpthread -lXran
dr -lXI -ldl
In file included from source/libs/imports.h:6,
                 from main.c:1:
source/libs/glad.h:711:9: error: unknown type name ‘khronos_int8_t’
  711 | typedef khronos_int8_t GLbyte;
      |         ^~~~~~~~~~~~~~
source/libs/glad.h:712:9: error: unknown type name ‘khronos_uint8_t’
  712 | typedef khronos_uint8_t GLubyte;
      |         ^~~~~~~~~~~~~~~
source/libs/glad.h:713:9: error: unknown type name ‘khronos_int16_t’
  713 | typedef khronos_int16_t GLshort;
      |         ^~~~~~~~~~~~~~~
source/libs/glad.h:714:9: error: unknown type name ‘khronos_uint16_t’
  714 | typedef khronos_uint16_t GLushort;
      |         ^~~~~~~~~~~~~~~~
source/libs/glad.h:717:9: error: unknown type name ‘khronos_int32_t’
  717 | typedef khronos_int32_t GLclampx;
      |         ^~~~~~~~~~~~~~~
source/libs/glad.h:719:9: error: unknown type name ‘khronos_float_t’
  719 | typedef khronos_float_t GLfloat;
      |         ^~~~~~~~~~~~~~~
source/libs/glad.h:720:9: error: unknown type name ‘khronos_float_t’
  720 | typedef khronos_float_t GLclampf;
      |         ^~~~~~~~~~~~~~~
source/libs/glad.h:732:9: error: unknown type name ‘khronos_uint16_t’
  732 | typedef khronos_uint16_t GLhalf;
      |         ^~~~~~~~~~~~~~~~
source/libs/glad.h:733:9: error: unknown type name ‘khronos_uint16_t’
  733 | typedef khronos_uint16_t GLhalfARB;
      |         ^~~~~~~~~~~~~~~~
source/libs/glad.h:734:9: error: unknown type name ‘khronos_int32_t’
  734 | typedef khronos_int32_t GLfixed;
      |         ^~~~~~~~~~~~~~~
source/libs/glad.h:745:9: error: unknown type name ‘ptrdiff_t’
  745 | typedef ptrdiff_t GLintptr;
      |         ^~~~~~~~~
source/libs/glad.h:758:9: error: unknown type name ‘ptrdiff_t’
  758 | typedef ptrdiff_t GLintptrARB;
      |         ^~~~~~~~~
source/libs/glad.h:771:9: error: unknown type name ‘ptrdiff_t’
  771 | typedef ptrdiff_t GLsizeiptr;
      |         ^~~~~~~~~
source/libs/glad.h:784:9: error: unknown type name ‘ptrdiff_t’
  784 | typedef ptrdiff_t GLsizeiptrARB;
      |         ^~~~~~~~~
source/libs/glad.h:787:9: error: unknown type name ‘khronos_int64_t’
  787 | typedef khronos_int64_t GLint64;
      |         ^~~~~~~~~~~~~~~
source/libs/glad.h:788:9: error: unknown type name ‘khronos_int64_t’
  788 | typedef khronos_int64_t GLint64EXT;
      |         ^~~~~~~~~~~~~~~
source/libs/glad.h:789:9: error: unknown type name ‘khronos_uint64_t’
  789 | typedef khronos_uint64_t GLuint64;
      |         ^~~~~~~~~~~~~~~~
source/libs/glad.h:790:9: error: unknown type name ‘khronos_uint64_t’
  790 | typedef khronos_uint64_t GLuint64EXT;
      |   
```