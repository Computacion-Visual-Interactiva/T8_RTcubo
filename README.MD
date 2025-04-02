# Pasos para compilar y ejecutar

Sigue estos pasos desde la raíz del proyecto:

### 1. Generar los archivos de compilación con CMake
```bash
cmake -B build
```

### 2. Compilar el proyecto
```bash
cmake --build build
```

### 3. Ejecutar el programa y generar la imagen
```bash
./build/Debug/T8_RTCUBo.exe > resultado/image.ppm
```

Después del paso 3 en la carpeta resultado se generara la imagen.pmm que tocara abrir con un visualizador de ppm como: https://www.cs.rhodes.edu/welshc/COMP141_F16/ppmReader.html
