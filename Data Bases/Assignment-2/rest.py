from flask import Flask
from flask_restful import Api, Resource, reqparse
from flask_cors import CORS
import psycopg2

app = Flask(__name__)
api = Api(app)
CORS(app)

# Initialize query variables
select_all_users_query = None
select_user_by_id_query = None
select_user_friends_query = None
select_existing_user_query = None
insert_new_user_query = None
delete_user_query = None

class Users(Resource):
    def get(self):
        conn = psycopg2.connect("host=127.0.0.1 dbname=socialnetwork user=vagrant password=vagrant")
        cur = conn.cursor()

        cur.execute(select_all_users_query)
        d = {"tuples": []}
        for c in cur.fetchall():
            t_to_d = {"userid": c[0], "name": c[1], "birthdate": str(c[2]), "joined": str(c[3])}
            d["tuples"].append(t_to_d)
        return d, 200

class User(Resource):
    def get(self, userid):
        conn = psycopg2.connect("host=127.0.0.1 dbname=socialnetwork user=vagrant password=vagrant")
        cur = conn.cursor()

        cur.execute(select_user_by_id_query, (userid,))
        user_data = cur.fetchone()

        if user_data:
            userid, name, birthdate, joined = user_data
            user_info = {
                "userid": userid,
                "name": name,
                "birthdate": str(birthdate),
                "joined": str(joined),
                "friends": []
            }

            cur.execute(select_user_friends_query, (userid,))
            friends_data = cur.fetchall()
            friends = [friend[0] for friend in friends_data]

            user_info["friends"] = sorted(friends)

            return user_info, 200
        else:
            return {"message": "User not found"}, 404

    def post(self, userid):
        parser = reqparse.RequestParser()
        parser.add_argument("name")
        parser.add_argument("birthdate")
        parser.add_argument("joined")
        args = parser.parse_args()

        conn = psycopg2.connect("host=127.0.0.1 dbname=socialnetwork user=vagrant password=vagrant")
        cur = conn.cursor()

        cur.execute(select_existing_user_query, (userid,))
        existing_user = cur.fetchone()

        if existing_user:
            return "FAILURE -- Userid must be unique", 201
        else:
            cur.execute(insert_new_user_query, (userid, args["name"], args["birthdate"], args["joined"]))
            conn.commit()
            return "SUCCESS", 201

    def delete(self, userid):
        conn = psycopg2.connect("host=127.0.0.1 dbname=socialnetwork user=vagrant password=vagrant")
        cur = conn.cursor()

        cur.execute(select_existing_user_query, (userid,))
        existing_user = cur.fetchone()

        if existing_user:
            cur.execute(delete_user_query, (userid,))
            cur.execute(delete_user_friends_query, (userid, userid))
            conn.commit()

            return "SUCCESS", 201
        else:
            return "FAILURE -- Unknown Userid", 404

api.add_resource(Users, "/users/")
api.add_resource(User, "/user/<string:userid>")

# Initialize query strings
select_all_users_query = "SELECT * FROM users;"
select_user_by_id_query = "SELECT * FROM users WHERE userid = %s;"
select_user_friends_query = "SELECT friendid FROM friends WHERE userid = %s;"
select_existing_user_query = "SELECT userid FROM users WHERE userid = %s;"
insert_new_user_query = "INSERT INTO users (userid, name, birthdate, joined) VALUES (%s, %s, %s, %s);"
delete_user_query = "DELETE FROM users WHERE userid = %s;"
delete_user_friends_query = "DELETE FROM friends WHERE userid = %s OR friendid = %s;"

app.run(debug=True, host="0.0.0.0", port=5000)
