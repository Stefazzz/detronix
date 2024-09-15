from flask import Flask, render_template, request, jsonify, send_from_directory
import os
import mysql.connector

app = Flask(__name__)

db = mysql.connector.connect(host='localhost', port='3306',  user='root', password='', database='detronix')


CARPETAU = os.path.join('uploads')
app.config['CARPETAU']=CARPETAU