# 💰 Gestor de Finanzas Personales y Alcancía Contable

[![Language: Spanish](https://img.shields.io/badge/Language-Spanish-yellow.svg)](#-español)
[![Language: English](https://img.shields.io/badge/Language-English-blue.svg)](#-english)

> Una solución ligera, interactiva y automatizada para gestionar presupuestos, gastos diarios y fondos de ahorro.

---

## 🎯 ¿Por qué me animé a hacer este proyecto?

Comencé pensando en cómo gestionar mi dinero de forma sencilla, ya que se me hacía tedioso usar hojas de cálculo en Excel. Busqué plantillas prediseñadas por varios lados, pero ninguna se adaptaba a lo que realmente necesitaba. 

Antes de volver al lápiz y papel, me hice una pregunta: **¿por qué no programarlo yo mismo?** Ya venía practicando bastante lógica en C++ y consolidando ejercicios, así que decidí dar el paso y combinarlo con una interfaz visual limpia en HTML, CSS y JavaScript.

El objetivo fue construir una herramienta práctica que me permitiera:
- **Organizar gastos fijos y variables** (alquiler, servicios, alimentación, estudios) de forma ordenada por fecha y categoría.
- **Gestionar el flujo de mi sueldo neto**, acumulando automáticamente los remanentes del mes anterior.
- **Construir un hábito de ahorro**, separando dinero para metas personales o fondos de emergencia.
- **Consolidar conceptos de programación**, uniendo una interfaz web dinámica (**HTML5, CSS3, JavaScript**) con un motor de cálculo y persistencia robusto en **C++** aplicando Programación Orientada a Objetos (POO), memoria dinámica manual y manipulación de archivos.

---

## 🚀 Características Principales

- **Panel de Control Interactivo:** Visualización inmediata de Total Disponible, Total Gastado, Fondo de Ahorro y Saldo Libre Restante.
- **Alcancía y Fondos:** Módulo para apartar dinero del sueldo, registrar ganancias extra o nutrir un fondo de emergencia.
- **Persistencia Flexible:** Guardado automático local en el navegador (`localStorage`) y exportación a archivos `.txt`.
- **Boleta Formal Contable:** Generación de un reporte detallado y alineado en texto plano (`boleta.txt`) sentia que podia mostrar algo profesional.
- **Motor en C++:** Implementación orientada a objetos con gestión manual de punteros (`char*`, `new[]`, `delete[]`) y cumplimiento de la Regla de los 3.

---

## 🛠️ Tecnologías Utilizadas

- **Frontend:** HTML5, CSS3, JavaScript.
- **Backend / Motor:** C++ (Streams `<fstream>`, Manipuladores `<iomanip>`, POO)

---

## 💻 Ejecución del Motor C++

Para compilar y ejecutar el procesamiento local en C++:

```bash
g++ proyecto-1.cpp -o finanzas
./finanzas



