# TPE Cripto. y Seg. 2021 1C.

Implementación en C de secreto compartido con esteganografía para ocultar una imágen secreta en K imágenes portadoras.

## Integrantes
- Matías Sebastián Baiges - 59076
- Eugenia Sol Piñeiro - 59449
- Nicolás Comerci Wolcanyik - 59520

---
## Requisitos previos
- Compilador de C
- Herramienta `make`
- Al menos 5 imágenes `.bmp` (incluída la que se ocultará) en blanco y negro, todas del mismo tamaño

## Compilación
1. Clonar el repositorio de la siguiente manera:
   ```bash
   git clone https://github.com/mbaiges/cripto_tp.git
   ```
2. Dentro de la carpeta raíz del proyecto ejecutar el siguiente comando para compilar:
   ```bash
   make all
   ```
3. Una vez finalizada la compilación, el binario llamado `ss` se encontrará en la carpeta `./target/main` 

## Ejecución
El programa tiene dos modos de uso: distribución del secreto y recuperación del secreto.

### Distribución

En primer lugar, tener preparado un directorio con las imágenes portadoras y, por otra lado, la imagen a distribuir (dicha imagen no puede estar en el mismo directorio que las portadoras).

Una vez hecho esto, ejecutar el siguiente comado:
```bash
./target/main/ss d <secret_image.bmp> <k> <shades_dir>
```
Donde:
- `<secret_image.bmp>` : path de la imagen a distribuir
- `<k>` : cantidad mínima de sombras para recuperar el secreto, donde k debe ser un valor entre 4 y 6 y menor o igual a cantidad de imágenes portadoras del directorio
- `<shades_dir>` : directorio donde se encuentran las imágenes portadoras o sombras

Una vez ejecutado el comando, se habrán modificado las imágenes del directorio con el secreto distribuído en ellas.

### Recuperación
Para recuperar el secreto de un conjunto de imágenes dentro de un directorio, ejecutar el siguiente comando:

```bash
./target/main/ss r <recover.bmp> <k> <shades_dir>
```
Donde:
- `<recover.bmp>` : path donde se encontrará la imagen recuperada
- `<k>` : cantidad de sombras a utilizar para recuperar el secreto
- `<shades_dir>` : directorio donde se encuentran las imágenes portadoras o sombras

