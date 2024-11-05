document.addEventListener('DOMContentLoaded', () => { 
    const productos = document.querySelectorAll('.compra');

    productos.forEach(btn => {
        const id = btn.id.split('_')[1];
        const qrId = "qr_" + id;
        const codigoqr = document.getElementById(qrId);
        const cerrarBtn = document.getElementById("cerrar_" + id);

        btn.addEventListener('click', () => {
            if (codigoqr) {
                codigoqr.style.display = 'flex';
            }
        });

        cerrarBtn.addEventListener('click', () => {
            codigoqr.style.display = 'none';
            window.location.href = '/'

        });
    });
});
