// Conectar al broker MQTT usando WebSockets
const client = mqtt.connect('ws://3.141.5.41:8080');  //IP PUBLICA

document.addEventListener('DOMContentLoaded', () => {
    const productos = document.querySelectorAll('.compra'); 
    client.on('connect', () => {
        console.log('Conectado al broker MQTT');
    });

    client.on('error', (error) => {
        console.error('Error en la conexión MQTT:', error);
    });

    productos.forEach(btn => {
        const id = btn.id.split('_')[1];  // Extraemos el id del producto, el número después de "btn_"
        const qrId = "qr_" + id;  
        const codigoqr = document.getElementById(qrId);  
        const cerrarBtn = document.getElementById("cerrar_" + id);  // Botón para cerrar el QR

        btn.addEventListener('click', () => {
            // Muestra el QR correspondiente cuando se hace clic en "comprar"
            if (codigoqr) {
                codigoqr.style.display = 'flex';
            }

            // Publica un mensaje en el tema MQTT con el id del producto
            const mensaje = `Producto ${id} comprado`;
            client.publish('detronix/compra', mensaje);  // Publica en el tema adecuado
            console.log("Mensaje enviado al ESP32 con id", id);
        });

        // Función para cerrar el QR cuando se hace clic en el botón "X"
        cerrarBtn.addEventListener('click', () => {
            codigoqr.style.display = 'none';  
            window.location.href = '/';  
        });
    });
});
