#include <iostream>
#include <fstream>
#include <iomanip>

using namespace std;

const int MAX_REGISTROS = 300;

// ============================================================================
// FUNCIONES AUXILIARES CON ARITMÉTICA DE PUNTEROS
// ============================================================================
int longitudCadena(const char* origen) {
    if (!origen) return 0;
    int len = 0;
    while (*(origen + len) != '\0') {
        len++;
    }
    return len;
}

char* duplicarCadena(const char* origen) {
    if (!origen) return nullptr;
    int len = longitudCadena(origen);
    char* destino = new char[len + 1];
    for (int i = 0; i <= len; i++) {
        *(destino + i) = *(origen + i);
    }
    return destino;
}

bool compararCadenas(const char* a, const char* b) {
    if (!a || !b) return false;
    int i = 0;
    while (*(a + i) != '\0' && *(b + i) != '\0') {
        if (*(a + i) != *(b + i)) return false;
        i++;
    }
    return *(a + i) == *(b + i);
}

// ============================================================================
// CLASE MOVIMIENTO (GASTOS Y AHORROS CON GESTIÓN MANUAL DE MEMORIA)
// ============================================================================
class Movimiento {
private:
    int dia;
    int mes;
    char* tipo;        // "Gasto" o "Ahorro"
    char* categoria;
    char* descripcion;
    double monto;

public:
    Movimiento() : dia(1), mes(1), tipo(nullptr), categoria(nullptr), descripcion(nullptr), monto(0.0) {}

    Movimiento(int d, int m, const char* t, const char* cat, const char* desc, double mon)
        : dia(d), mes(m), monto(mon) {
        tipo = duplicarCadena(t);
        categoria = duplicarCadena(cat);
        descripcion = duplicarCadena(desc);
    }

    ~Movimiento() {
        delete[] tipo;
        delete[] categoria;
        delete[] descripcion;
    }

    Movimiento(const Movimiento& otro) : dia(otro.dia), mes(otro.mes), monto(otro.monto) {
        tipo = duplicarCadena(otro.tipo);
        categoria = duplicarCadena(otro.categoria);
        descripcion = duplicarCadena(otro.descripcion);
    }

    Movimiento& operator=(const Movimiento& otro) {
        if (this != &otro) {
            delete[] tipo;
            delete[] categoria;
            delete[] descripcion;

            dia = otro.dia;
            mes = otro.mes;
            monto = otro.monto;
            tipo = duplicarCadena(otro.tipo);
            categoria = duplicarCadena(otro.categoria);
            descripcion = duplicarCadena(otro.descripcion);
        }
        return *this;
    }

    int getDia() const { return dia; }
    int getMes() const { return mes; }
    double getMonto() const { return monto; }
    const char* getTipo() const { return tipo ? tipo : ""; }
    const char* getCategoria() const { return categoria ? categoria : ""; }
    const char* getDescripcion() const { return descripcion ? descripcion : ""; }
};

// ============================================================================
// CLASE GESTOR DE FINANZAS Y AHORROS
// ============================================================================
class GestorFinanzas {
private:
    double saldoAnterior;
    double sueldoNeto;
    double ahorroBase;
    Movimiento historial[MAX_REGISTROS];
    int totalMovimientos;

public:
    GestorFinanzas() : saldoAnterior(0.0), sueldoNeto(0.0), ahorroBase(0.0), totalMovimientos(0) {}

    void setSaldoAnterior(double s) { saldoAnterior = s; }
    void setSueldoNeto(double sn) { sueldoNeto = sn; }
    void setAhorroBase(double ab) { ahorroBase = ab; }

    bool agregarMovimiento(int d, int m, const char* t, const char* cat, const char* desc, double mon) {
        if (totalMovimientos >= MAX_REGISTROS) {
            cout << "Error: Limite de registros alcanzado.\n";
            return false;
        }
        historial[totalMovimientos] = Movimiento(d, m, t, cat, desc, mon);
        totalMovimientos++;
        return true;
    }

    double calcularTotalDisponible() const {
        return saldoAnterior + sueldoNeto;
    }

    double calcularTotalGastado() const {
        double tot = 0.0;
        for (int i = 0; i < totalMovimientos; i++) {
            if (*(historial[i].getTipo()) == 'G') {
                tot += historial[i].getMonto();
            }
        }
        return tot;
    }

    double calcularAportesAhorro() const {
        double tot = 0.0;
        for (int i = 0; i < totalMovimientos; i++) {
            if (*(historial[i].getTipo()) == 'A') {
                tot += historial[i].getMonto();
            }
        }
        return tot;
    }

    double calcularFondoTotalAhorro() const {
        return ahorroBase + calcularAportesAhorro();
    }

    double calcularSaldoLibreRestante() const {
        return calcularTotalDisponible() - calcularTotalGastado() - calcularAportesAhorro();
    }

    void acumularSiguienteMes(double nuevoSueldo) {
        saldoAnterior = calcularSaldoLibreRestante();
        ahorroBase = calcularFondoTotalAhorro();
        sueldoNeto = nuevoSueldo;
        totalMovimientos = 0;
        cout << "\n[OK] Nuevo mes iniciado.";
        cout << "\n -> Saldo libre traspasado: S/ " << fixed << setprecision(2) << saldoAnterior;
        cout << "\n -> Fondo de ahorro consolidado: S/ " << ahorroBase << "\n";
    }

    // ========================================================================
    // PERSISTENCIA EN TXT
    // ========================================================================
    void guardarEnTxt(const char* ruta) const {
        ofstream file(ruta);
        if (!file.is_open()) {
            cout << "Error al abrir " << ruta << "\n";
            return;
        }

        file << saldoAnterior << "\n" << sueldoNeto << "\n" << ahorroBase << "\n" << totalMovimientos << "\n";
        for (int i = 0; i < totalMovimientos; i++) {
            file << historial[i].getDia() << "|"
                 << historial[i].getMes() << "|"
                 << historial[i].getTipo() << "|"
                 << historial[i].getCategoria() << "|"
                 << historial[i].getDescripcion() << "|"
                 << historial[i].getMonto() << "\n";
        }
        file.close();
        cout << "[OK] Archivo guardado: " << ruta << "\n";
    }

    void cargarDesdeTxt(const char* ruta) {
        ifstream file(ruta);
        if (!file.is_open()) {
            cout << "No se encontro archivo previo. Iniciando con valores en cero.\n";
            return;
        }

        file >> saldoAnterior >> sueldoNeto >> ahorroBase >> totalMovimientos;
        file.ignore();

        char buffer[256];
        for (int i = 0; i < totalMovimientos; i++) {
            file.getline(buffer, 256);
            int idx = 0;

            // 1. Dia
            int d = 0;
            while (*(buffer + idx) >= '0' && *(buffer + idx) <= '9') {
                d = d * 10 + (*(buffer + idx) - '0');
                idx++;
            }
            idx++;

            // 2. Mes
            int m = 0;
            while (*(buffer + idx) >= '0' && *(buffer + idx) <= '9') {
                m = m * 10 + (*(buffer + idx) - '0');
                idx++;
            }
            idx++;

            // 3. Tipo
            char tipo[20];
            int tIdx = 0;
            while (*(buffer + idx) != '|' && *(buffer + idx) != '\0') {
                *(tipo + tIdx) = *(buffer + idx);
                tIdx++;
                idx++;
            }
            *(tipo + tIdx) = '\0';
            idx++;

            // 4. Categoria
            char cat[50];
            int cIdx = 0;
            while (*(buffer + idx) != '|' && *(buffer + idx) != '\0') {
                *(cat + cIdx) = *(buffer + idx);
                cIdx++;
                idx++;
            }
            *(cat + cIdx) = '\0';
            idx++;

            // 5. Descripcion
            char desc[100];
            int dIdx = 0;
            while (*(buffer + idx) != '|' && *(buffer + idx) != '\0') {
                *(desc + dIdx) = *(buffer + idx);
                dIdx++;
                idx++;
            }
            *(desc + dIdx) = '\0';
            idx++;

            // 6. Monto
            double mon = 0.0, dec = 0.1;
            bool esDec = false;
            while (*(buffer + idx) != '\0') {
                if (*(buffer + idx) == '.') {
                    esDec = true;
                } else if (*(buffer + idx) >= '0' && *(buffer + idx) <= '9') {
                    if (!esDec) mon = mon * 10.0 + (*(buffer + idx) - '0');
                    else { mon += (*(buffer + idx) - '0') * dec; dec *= 0.1; }
                }
                idx++;
            }

            historial[i] = Movimiento(d, m, tipo, cat, desc, mon);
        }
        file.close();
        cout << "[OK] Se cargaron " << totalMovimientos << " movimientos desde " << ruta << "\n";
    }

    // ========================================================================
    // GENERACIÓN DE BOLETA FORMAL EN TXT
    // ========================================================================
    void exportarBoletaTxt(const char* ruta) const {
        ofstream file(ruta);
        if (!file.is_open()) return;

        double totalDisp = calcularTotalDisponible();
        double totalGast = calcularTotalGastado();
        double aportesAh = calcularAportesAhorro();
        double totalAh = calcularFondoTotalAhorro();
        double saldoLib = calcularSaldoLibreRestante();

        file << "======================================================================\n";
        file << "             ESTADO DE CUENTA Y ALCANCÍA CONTABLE                     \n";
        file << "======================================================================\n\n";

        file << "----------------------------------------------------------------------\n";
        file << "                         RESUMEN DE FONDOS                            \n";
        file << "----------------------------------------------------------------------\n";
        file << fixed << setprecision(2);
        file << "  (+) Saldo Anterior           :       S/ " << setw(9) << right << saldoAnterior << "\n";
        file << "  (+) Sueldo Neto Mensual      :       S/ " << setw(9) << right << sueldoNeto << "\n";
        file << "  -------------------------------------------------\n";
        file << "  (=) TOTAL INGRESOS / DISP.   :       S/ " << setw(9) << right << totalDisp << "\n";
        file << "  (-) TOTAL GASTOS REALIZADOS  :       S/ " << setw(9) << right << totalGast << "\n";
        file << "  (-) APARTADO A AHORROS       :       S/ " << setw(9) << right << aportesAh << "\n";
        file << "  -------------------------------------------------\n";
        file << "  (=) SALDO LIBRE RESTANTE     :       S/ " << setw(9) << right << saldoLib << "\n";
        file << "  (★) FONDO TOTAL EN AHORROS   :       S/ " << setw(9) << right << totalAh << "\n\n";

        file << "----------------------------------------------------------------------\n";
        file << "                     DETALLE DE MOVIMIENTOS                           \n";
        file << "----------------------------------------------------------------------\n";
        file << left 
             << setw(8)  << "FECHA"
             << setw(10) << "TIPO"
             << setw(16) << "CATEGORÍA"
             << setw(24) << "DESCRIPCIÓN"
             << right << setw(12) << "MONTO" << "\n";
        file << "----------------------------------------------------------------------\n";

        if (totalMovimientos == 0) {
            file << "                   No hay movimientos registrados.                    \n";
        } else {
            for (int i = 0; i < totalMovimientos; i++) {
                char fStr[10];
                int d = historial[i].getDia();
                int m = historial[i].getMes();
                fStr[0] = (d < 10 ? '0' : (d / 10) + '0');
                fStr[1] = (d % 10) + '0';
                fStr[2] = '/';
                fStr[3] = (m < 10 ? '0' : (m / 10) + '0');
                fStr[4] = (m % 10) + '0';
                fStr[5] = '\0';

                bool esGasto = (*(historial[i].getTipo()) == 'G');
                file << left 
                     << setw(8)  << fStr
                     << setw(10) << historial[i].getTipo()
                     << setw(16) << historial[i].getCategoria()
                     << setw(24) << historial[i].getDescripcion()
                     << (esGasto ? "- S/ " : "+ S/ ") << right << setw(7) << historial[i].getMonto() << "\n";
            }
        }
        file << "======================================================================\n";
        file.close();
        cout << "[OK] Boleta formal emitida en: " << ruta << "\n";
    }
};

// ============================================================================
// MAIN
// ============================================================================
int main() {
    GestorFinanzas gestor;
    const char* txtFile = "finanzas.txt";
    const char* boletaFile = "boleta.txt";

    gestor.cargarDesdeTxt(txtFile);

    int opc = 0;
    do {
        cout << "\n=========================================\n";
        cout << "       MOTOR DE FINANZAS Y AHORROS       \n";
        cout << "=========================================\n";
        cout << "1. Recargar datos desde 'finanzas.txt'\n";
        cout << "2. Ver resumen y balance en pantalla\n";
        cout << "3. Generar Boleta Formal ('boleta.txt')\n";
        cout << "4. Registrar Gasto\n";
        cout << "5. Apartar Ahorro / Registrar Extra\n";
        cout << "6. Iniciar Nuevo Mes (Consolidar fondos)\n";
        cout << "7. Guardar y Salir\n";
        cout << "Seleccione una opcion: ";
        cin >> opc;

        if (opc == 1) {
            gestor.cargarDesdeTxt(txtFile);
        } else if (opc == 2) {
            cout << "\n---------------- BALANCE ----------------\n";
            cout << "Total Disponible:  S/ " << fixed << setprecision(2) << gestor.calcularTotalDisponible() << "\n";
            cout << "Total Gastado:     S/ " << gestor.calcularTotalGastado() << "\n";
            cout << "Fondo Ahorros:     S/ " << gestor.calcularFondoTotalAhorro() << "\n";
            cout << "Saldo Libre:       S/ " << gestor.calcularSaldoLibreRestante() << "\n";
            cout << "-----------------------------------------\n";
        } else if (opc == 3) {
            gestor.exportarBoletaTxt(boletaFile);
        } else if (opc == 4 || opc == 5) {
            int d, m;
            char cat[50], desc[100];
            double mon;

            cout << "Dia: "; cin >> d;
            cout << "Mes: "; cin >> m;
            cin.ignore();
            cout << "Categoria: "; cin.getline(cat, 50);
            cout << "Descripcion: "; cin.getline(desc, 100);
            cout << "Monto: S/ "; cin >> mon;

            if (opc == 4) gestor.agregarMovimiento(d, m, "Gasto", cat, desc, mon);
            else gestor.agregarMovimiento(d, m, "Ahorro", cat, desc, mon);
            cout << "[OK] Movimiento anadido.\n";
        } else if (opc == 6) {
            double sn;
            cout << "Nuevo sueldo neto para este mes: S/ "; cin >> sn;
            gestor.acumularSiguienteMes(sn);
        }

    } while (opc != 7);

    gestor.guardarEnTxt(txtFile);
    gestor.exportarBoletaTxt(boletaFile);
    cout << "\nTodo guardado y sincronizado.\n";
    return 0;
}