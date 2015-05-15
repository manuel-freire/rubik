# rubik

Ejercicio para EDA (Asignatura de la [FdI UCM](http://www.fdi.ucm.es))

## Contenido del repositorio

* `enunciado.pdf` - enunciado de la práctica
* `test.jar` - permite hacer pruebas gráficas para verificar que la lectura y escritura de cubos, y las rotaciones, funcionan como deben. 
* `LICENSE` - licencia (Apache v2, muy liberal). Esta licencia NO cubre la copia de [processing](https://processing.org) incluida en `test.jar`
* `ayuda` - práctica de ayuda, que se verá en el laboratorio (opcional **sólo** si entregas vía GitHub) del día 14 de mayo

## Cómo hacer pruebas

Lo más complicado, sobre todo al principio, será estar seguro de que eres capaz de leer y escribir cubos de forma correcta. Usa el programa `test.jar` para asegurarte de que estás usando las convenciones propuestas por la práctica. No es *obligatorio* seguir estas convenciones (no afectarán realmente a la funcionalidad que tu eje `Y` esté invertido, por ejemplo), pero seguirlas te facilitará mucho la vida a la hora de hacer pruebas, porque podrás comparar tus salidas con unas que ya sabes que funcionan.

Uso de `test.jar` (requiere Java instalado y en el "path"):

```java
java -jar test.jar 
```

Puedes especificar un argumento adicional: el número de dimensiones a usar (si no escribes nada, se asume un 2). Esto te permite probar que vas por buen camino si decides implementar los opcionales. Por ejemplo, para probar con un cubo de 3x3x3, y en resolución de 800x600 píxeles, escribirías

```java
java -jar test.jar 3 800 600
```

Todos los argumentos son opcionales; si no especificas dimensión, usará dimensión 2. Si no das ancho y alto en píxeles, usará 1000x800. Una vez que lanzas `test.jar`, te aparecerá en todo momento, en la parte superior de la ventana, la representación `aCadena()` del cubo que estás viendo. Además, la irá mostrando por consola (si es que tienes una abierta).

### Teclas en test.jar

Las teclas disponibles son las siguientes:

* Teclas `x`, `y`, `z`: cambian el eje seleccionado. Por defecto, el eje x empieza seleccionado
* Teclas `1`, `2`, ...: realizan un giro sobre el eje seleccionado. `1` realiza un giro en la "loncha" más cercana al origen de coordenadas; números más grandes hacen giros en lonchas más alejadas del mismo.
* Teclas `q`, `w`, ...: realizan giros en sentido contrario a los realizados por las teclas de la fila superior (`1`, `2`, etcétera).

(`test.jar` está escrito usando [processing](https://processing.org), una librería que permite crear visualizaciones interactivas de forma sencilla; no publico los fuentes para no facilitar/confundir en la realización de esta práctica).

