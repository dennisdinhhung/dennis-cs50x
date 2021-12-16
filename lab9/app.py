import os
from typing import NewType

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session
import sqlalchemy

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///birthdays.db")

LIST={}

NEW_BIRTHDAY={}

@app.route("/", methods=["GET", "POST"])
def index():
    if request.method == "POST":

        # TODO: Add the user's entry into the database
            # id INTEGER,
            # name TEXT,
            # month INTEGER,
            # day INTEGER,
                # PRIMARY KEY(id)
        
        data = db.execute("SELECT name, month, day FROM birthdays;")

        # acquire data
        name = request.form.get("name")
        if not name:
            return render_template("index.html", error_message = "No name error", data=data)
        date = request.form.get("date")
        if not date:
            return render_template("index.html", error_message = "No date error", data=data)
        month = request.form.get("month")
        if not month:
            return render_template("index.html", error_message = "No month error", data=data)

        # check if date is correct with month
        if month == "2" and date == "30" or date == "30":
            return render_template("index.html", error_message = "Dummy, February has, at max, 29 days", data=data)
        
        #execute sql query
        db.execute("INSERT INTO birthdays (name, month, day) VALUES (?,?,?)", name, month, date)
        
        #redirect site to index (aka reload)
        return redirect("/")

    else:

        # TODO: Display the entries in the database on index.html
        # to load data from birthdays.db into a list
        data = db.execute("SELECT * FROM birthdays;")
        return render_template("index.html", data=data)

@app.route("/delete/<int:id>")
def delete(id):
    if not id:
        return render_template("index.html")
    
    db.execute("DELETE FROM birthdays WHERE id=?", id)
    return redirect("/")


# This line is for the app.py to run using python3
if __name__ == "__main__":
    app.run(debug=True)

