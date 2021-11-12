from flask import Flask, render_template, jsonify
import json
import datetime
from flask_cors import CORS
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
                             cursorclass = pymysql.cursors.DictCursor,
                             ssl={"fake_flag_to_enable_tls":True})

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
            print(f"Data out database: {json.dumps(results, default = convertDateTime)}")
            return json.dumps(results, default = convertDateTime)
    except Exception as e:
        print(e)
        result = "false"
    finally:
        cursor.close()


def convertDateTime(o):
    if isinstance(o, datetime.datetime):
        return o.__str__()

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
@brief      Get all coins from the database
@return     result                  String: If successful, data from the database, else "false"
"""
@app.route('/get/coins')
def get_all_data_coin(): 
    return str(get('SELECT coin_id, full_name FROM Coins'))

@app.route('/remove/coin/<string:coinId>')
def remove_coin(coinId): 
    return send('DELETE FROM Coins WHERE coin_id = upper(\"%s\")' %(coinId))

"""
@brief      Insert if not already in database
@param      coinId                  String: The coins id
@param      fullName                String: Full name of the coin
@return     result                  String: If successful, "true", else "false"
"""
@app.route('/insert/coin/<string:coinId>/<string:fullName>')
def insert_coin(coinId, fullName): 
    return send('call addCoin(upper(\'%s\'), \'%s\')'%(coinId, fullName))

"""
@brief      Insert data of a coin in the database	
@param      coinId                  String: The coins id
@param      timeStamp               String: Timestamp of the insert
@return     price                   String: Current price of the coin
"""
@app.route('/insert/data/<string:coinId>/<string:timeStamp>/<string:price>')
def insert_price(coinId, timeStamp, price):
    return send('call addData(upper(\'%s\'), \'%s\', \'%s\')'%(coinId, timeStamp, price))

"""
@brief      Check if a coin is already in the database
@param      coinId                  String: The coins id
@return     result                  String: If successful, data from the database, else "false"
"""
@app.route('/get/coin/<string:coinId>')
def checkIfCoinThere(coinId):
    result = str(get('SELECT coin_id FROM Coins WHERE coin_id = upper(\"%s\")'%coinId))
    if result == "()":
        return "false"
    else:
        return result

@app.route('/get/<string:coinId>/<string:amount>')
def getPriceCoin(coinId, amount):
    result = str(get('call getData(upper(\'%s\'), %d)'%(coinId, int(amount))))
    if result == "()":
        return "false"
    else:
        return result

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=8112)
