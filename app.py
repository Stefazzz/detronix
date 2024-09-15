from flask import Flask, render_template, request, jsonify, send_from_directory
from conexion import *
from modules.productos import misProductos


@app.route('/uploads/<nombre>')
def uploads(nombre):
    return send_from_directory(app.config['CARPETAU'],nombre)

@app.route('/')
def index():
    producto = misProductos.consultar()
    return render_template('index.html', pro = producto)


if __name__ == '__main__':
    app.run(host='0.0.0.0', debug=True, port="5080")