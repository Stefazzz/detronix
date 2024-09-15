from conexion import * 

class productos:

    def __init__(self, conexion):
        self.conexion = conexion
        self.cursor = self.conexion.cursor()

    def consultar(self):
        sql = "SELECT * FROM inventario"
        self.cursor.execute(sql)
        resultado = self.cursor.fetchall()
        return resultado
    


misProductos = productos(db)