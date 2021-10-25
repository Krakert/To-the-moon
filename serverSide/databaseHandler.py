from flask import Flask, render_template, jsonify
import json
from flask_cors import CORS, cross_origin
import pymysql

HOST_ADDRESS = 'oege.ie.hva.nl'
USER_NAME = 'krakers'
PASSWORD = 'QcfY/USWP#uG1yRM'
NAME_DATABASE = 'zkrakers'

app = Flask(__name__)
CORS(app)

"""
@brief  Setup a database connection
@param      host                    String: Host of the database
@param      user                    String: Username
@param      password                String: Password for the database
@param      db                      String: Database name to connect to, user can have more than one db
"""
connection = pymysql.connect(host = HOST_ADDRESS,
                             user = USER_NAME,
                             password = PASSWORD,
                             db = NAME_DATABASE,
                             cursorclass = pymysql.cursors.DictCursor)

"""
@brief      Get data from the database
@param      query                   String: Query to send to the database
@return     result                  Tuple:  If successful, data returned by the database, else "false"
"""


def get(query):
    try:
        with connection.cursor() as cursor:
            cursor.execute(query)
            connection.commit()
            results = cursor.fetchall()
            print(f"Data out database: {json.dumps(results)}")
            return json.dumps(results)
    except Exception as e:
        print(e)
        result = "false"
    finally:
        cursor.close()
    return result

"""
@brief      Send data from the database
@param      query                   String: Query to send to the database
@retrun     result                  String: If successful, "true", else "false"
"""
def send(query):
    try:
        with connection.cursor() as cursor:
            cursor.execute(query)
            connection.commit()
            result =  "true"
    except Exception as e:
        print(e)
        result = "false"
    finally:
        cursor.close()
    return result

"""
@brief      Get all data of table DataCoins
@return     result                  String: If successful, data from the database, else "false" 
"""
@app.route('/get')
def get_all_data(): 
    return str(get('SELECT * FROM `DataCoins`'))

"""
@brief      Get all coins from the database
@return     result                  String: If successful, data from the database, else "false"
"""
@app.route('/get/coins')
def get_all_data_coin(): 
    return str(get('SELECT coin_id, full_name FROM Coins'))

@app.route('/remove/coin/<string:coinId>')
def remove_coin(coinId): 
    return send('DELETE FROM Coins WHERE coin_id = upper(\"%s\")' %(str(coinId)))

"""
@brief      Insert if not already in database
@param      coinId                  String: The coins id
@param      fullName                String: Full name of the coin
@return     result                  String: If successful, "true", else "false"
"""
@app.route('/insert/coin/<string:coinId>/<string:fullName>')
def insert_coin(coinId, fullName): 
    return send('call addCoin(upper(\"%s\"), \"%s\")'%(str(coinId), str(fullName)))


"""
@brief      Check if a coin is already in the database
@param      coinId                  String: The coins id
@return     result                  String: If successful, data from the database, else "false"
"""
@app.route('/get/coin/<string:coinId>')
def checkIfCoinThere(coinId):
    result = str(get('SELECT coin_id FROM Coins WHERE coin_id = upper(\"%s\")'%str(coinId)))
    if result == "()":
        return "false"
    else:
        return result

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=8112)
