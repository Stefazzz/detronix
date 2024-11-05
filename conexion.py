from flask import Flask, render_template, request, jsonify, send_from_directory
import os
import mysql.connector

app = Flask(__name__)

db = mysql.connector.connect( user='root', password='', host='localhost', database='detronix1')
#db = mysql.connector.connect( user='admin', password='stefa123', host='detronix1.cnkgwq0s8fnb.us-east-2.rds.amazonaws.com', database='db1')

CARPETAU = os.path.join('uploads')
app.config['CARPETAU']=CARPETAU