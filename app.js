let saldoAnterior = 0.0;
let sueldoNeto = 0.0;
let ahorroBase = 0.0;
let movimientos = [];

window.onload = function() {
    cargarMemoria();
};

function guardarMemoria() {
    localStorage.setItem('fin_saldo_ant', saldoAnterior);
    localStorage.setItem('fin_sueldo', sueldoNeto);
    localStorage.setItem('fin_ahorro_base', ahorroBase);
    localStorage.setItem('fin_movimientos', JSON.stringify(movimientos));
}

function cargarMemoria() {
    const sAnt = localStorage.getItem('fin_saldo_ant');
    const sNet = localStorage.getItem('fin_sueldo');
    const aBase = localStorage.getItem('fin_ahorro_base');
    const movs = localStorage.getItem('fin_movimientos');

    if (sAnt !== null) { 
        saldoAnterior = parseFloat(sAnt) || 0; 
        document.getElementById('saldo-acumulado').value = saldoAnterior; 
    }
    if (sNet !== null) { 
        sueldoNeto = parseFloat(sNet) || 0; 
        document.getElementById('sueldo-neto').value = sueldoNeto; 
    }
    if (aBase !== null) { 
        ahorroBase = parseFloat(aBase) || 0; 
        document.getElementById('ahorro-base').value = ahorroBase; 
    }
    if (movs !== null) { 
        movimientos = JSON.parse(movs) || []; 
    }

    renderizar();
}

function actualizarValores() {
    saldoAnterior = parseFloat(document.getElementById('saldo-acumulado').value) || 0.0;
    sueldoNeto = parseFloat(document.getElementById('sueldo-neto').value) || 0.0;
    ahorroBase = parseFloat(document.getElementById('ahorro-base').value) || 0.0;
    guardarMemoria();
    renderizar();
}

function registrarGasto() {
    const dia = parseInt(document.getElementById('gasto-dia').value) || 1;
    const mes = parseInt(document.getElementById('gasto-mes').value) || 1;
    const cat = document.getElementById('gasto-cat').value;
    const desc = document.getElementById('gasto-desc').value.trim() || "Gasto sin detalle";
    const monto = parseFloat(document.getElementById('gasto-monto').value) || 0.0;

    if (monto <= 0) return alert("Ingrese un monto válido.");

    movimientos.push({ dia, mes, tipo: "Gasto", cat, desc, monto });
    document.getElementById('gasto-desc').value = '';
    document.getElementById('gasto-monto').value = '';
    guardarMemoria();
    renderizar();
}

function registrarAhorro() {
    const dia = parseInt(document.getElementById('ahorro-dia').value) || 1;
    const mes = parseInt(document.getElementById('ahorro-mes').value) || 1;
    const cat = document.getElementById('ahorro-cat').value;
    const desc = document.getElementById('ahorro-desc').value.trim() || "Ahorro programado";
    const monto = parseFloat(document.getElementById('ahorro-monto').value) || 0.0;

    if (monto <= 0) return alert("Ingrese un monto válido.");

    movimientos.push({ dia, mes, tipo: "Ahorro", cat, desc, monto });
    document.getElementById('ahorro-desc').value = '';
    document.getElementById('ahorro-monto').value = '';
    guardarMemoria();
    renderizar();
}

function renderizar() {
    let totalGastado = 0.0;
    let aportesAhorro = 0.0;

    for (let m of movimientos) {
        if (m.tipo === "Gasto") totalGastado += m.monto;
        if (m.tipo === "Ahorro") aportesAhorro += m.monto;
    }

    const totalDisponible = saldoAnterior + sueldoNeto;
    const totalAhorro = ahorroBase + aportesAhorro;
    const saldoLibre = totalDisponible - totalGastado - aportesAhorro;

    document.getElementById('disp-disponible').innerText = 'S/ ' + totalDisponible.toFixed(2);
    document.getElementById('disp-gastado').innerText = 'S/ ' + totalGastado.toFixed(2);
    document.getElementById('disp-ahorro').innerText = 'S/ ' + totalAhorro.toFixed(2);
    document.getElementById('disp-restante').innerText = 'S/ ' + saldoLibre.toFixed(2);

    const tbody = document.getElementById('tabla-cuerpo');
    if (movimientos.length === 0) {
        tbody.innerHTML = '<tr><td colspan="5" style="text-align: center; color: #64748b;">No hay movimientos registrados.</td></tr>';
        return;
    }

    let html = '';
    for (let m of movimientos) {
        const fStr = `${String(m.dia).padStart(2, '0')}/${String(m.mes).padStart(2, '0')}`;
        const esGasto = m.tipo === "Gasto";
        html += `<tr>
            <td>${fStr}</td>
            <td><span class="tag-cat ${esGasto ? 'tag-gasto' : 'tag-ahorro'}">${m.tipo}</span></td>
            <td>${m.cat}</td>
            <td>${m.desc}</td>
            <td class="${esGasto ? 'monto-tabla' : 'monto-ahorro'}">${esGasto ? '-' : '+'} S/ ${m.monto.toFixed(2)}</td>
        </tr>`;
    }
    tbody.innerHTML = html;
}

function reiniciarTodo() {
    if (confirm("¿Deseas reiniciar todos los registros guardados?")) {
        localStorage.clear();
        saldoAnterior = sueldoNeto = ahorroBase = 0.0;
        movimientos = [];
        document.getElementById('saldo-acumulado').value = '0.00';
        document.getElementById('sueldo-neto').value = '';
        document.getElementById('ahorro-base').value = '0.00';
        renderizar();
    }
}

function exportarTxt() {
    let contenido = `${saldoAnterior}\n${sueldoNeto}\n${ahorroBase}\n${movimientos.length}\n`;
    for (let m of movimientos) {
        contenido += `${m.dia}|${m.mes}|${m.tipo}|${m.cat}|${m.desc}|${m.monto}\n`;
    }
    const blob = new Blob([contenido], { type: 'text/plain;charset=utf-8' });
    const a = document.createElement('a');
    a.href = URL.createObjectURL(blob);
    a.download = 'finanzas.txt';
    a.click();
}

function cargarDesdeTxt(event) {
    const file = event.target.files[0];
    if (!file) return;
    const reader = new FileReader();
    reader.onload = function(e) {
        const lineas = e.target.result.split('\n');
        if (lineas.length < 4) return;

        saldoAnterior = parseFloat(lineas[0]) || 0;
        sueldoNeto = parseFloat(lineas[1]) || 0;
        ahorroBase = parseFloat(lineas[2]) || 0;
        const total = parseInt(lineas[3]) || 0;

        document.getElementById('saldo-acumulado').value = saldoAnterior;
        document.getElementById('sueldo-neto').value = sueldoNeto;
        document.getElementById('ahorro-base').value = ahorroBase;

        movimientos = [];
        for (let i = 4; i < 4 + total; i++) {
            if (lineas[i] && lineas[i].trim().length > 0) {
                const p = lineas[i].split('|');
                if (p.length >= 6) {
                    movimientos.push({
                        dia: parseInt(p[0]),
                        mes: parseInt(p[1]),
                        tipo: p[2],
                        cat: p[3],
                        desc: p[4],
                        monto: parseFloat(p[5])
                    });
                }
            }
        }
        guardarMemoria();
        renderizar();
    };
    reader.readAsText(file);
}

function descargarBoletaTxt() {
    let totalGastado = 0.0;
    let aportesAhorro = 0.0;
    for (let m of movimientos) {
        if (m.tipo === "Gasto") totalGastado += m.monto;
        if (m.tipo === "Ahorro") aportesAhorro += m.monto;
    }
    const totalDisponible = saldoAnterior + sueldoNeto;
    const totalAhorrado = ahorroBase + aportesAhorro;
    const saldoLibre = totalDisponible - totalGastado - aportesAhorro;

    const padR = (s, l) => (s + " ".repeat(l)).slice(0, l);
    const padL = (s, l) => (" ".repeat(l) + s).slice(-l);

    let doc = "======================================================================\n";
    doc += "             ESTADO DE CUENTA Y ALCANCÍA CONTABLE                     \n";
    doc += "======================================================================\n\n";
    doc += "----------------------------------------------------------------------\n";
    doc += "                         RESUMEN DE FONDOS                            \n";
    doc += "----------------------------------------------------------------------\n";
    doc += `  (+) Saldo Anterior           :       S/ ${padL(saldoAnterior.toFixed(2), 9)}\n`;
    doc += `  (+) Sueldo Neto Mensual      :       S/ ${padL(sueldoNeto.toFixed(2), 9)}\n`;
    doc += `  (=) TOTAL INGRESOS / DISP.   :       S/ ${padL(totalDisponible.toFixed(2), 9)}\n`;
    doc += `  (-) TOTAL GASTOS REALIZADOS  :       S/ ${padL(totalGastado.toFixed(2), 9)}\n`;
    doc += `  (-) APARTADO A AHORROS       :       S/ ${padL(aportesAhorro.toFixed(2), 9)}\n`;
    doc += "  -------------------------------------------------\n";
    doc += `  (=) SALDO LIBRE RESTANTE     :       S/ ${padL(saldoLibre.toFixed(2), 9)}\n`;
    doc += `  (★) FONDO TOTAL EN AHORROS   :       S/ ${padL(totalAhorrado.toFixed(2), 9)}\n\n`;

    doc += "----------------------------------------------------------------------\n";
    doc += "                     DETALLE DE MOVIMIENTOS                           \n";
    doc += "----------------------------------------------------------------------\n";
    doc += `${padR("FECHA", 8)}${padR("TIPO", 10)}${padR("CATEGORÍA", 16)}${padR("DESCRIPCIÓN", 24)}${padL("MONTO", 12)}\n`;
    doc += "----------------------------------------------------------------------\n";
    for (let m of movimientos) {
        const fStr = `${String(m.dia).padStart(2, '0')}/${String(m.mes).padStart(2, '0')}`;
        doc += `${padR(fStr, 8)}${padR(m.tipo, 10)}${padR(m.cat, 16)}${padR(m.desc, 24)}${padL((m.tipo === "Gasto" ? "-" : "+") + " S/ " + m.monto.toFixed(2), 12)}\n`;
    }
    doc += "======================================================================\n";

    const blob = new Blob([doc], { type: 'text/plain;charset=utf-8' });
    const a = document.createElement('a');
    a.href = URL.createObjectURL(blob);
    a.download = 'boleta.txt';
    a.click();
}