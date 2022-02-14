from curses.ascii import isdigit
from multiprocessing.managers import ValueProxy
import os
import re
 
from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

from datetime import datetime

import sqlalchemy


# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True


# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# API KEY: pk_32127c800fc4438fb2f84dec4f0b5e50
# API key cmd: export API_KEY=pk_32127c800fc4438fb2f84dec4f0b5e50
# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")

# DATABASE:
# 
# CREATE TABLE users (
#   id INTEGER, 
#   username TEXT NOT NULL, 
#   hash TEXT NOT NULL, 
#   cash NUMERIC NOT NULL DEFAULT 10000.00, 
#   PRIMARY KEY(id));
# 
# CREATE UNIQUE INDEX username ON users (username);

@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    id = session["user_id"]
    username = db.execute("SELECT username FROM users WHERE id = ?", id)
    data = db.execute("SELECT * FROM stock WHERE username_stock = ?", username[0]["username"])
    
    # get stock price
    for i in range(len(data)):
        stock_price = lookup(data[i]["stock_name"])
        row = data[i]
        row.update({'stock_price': stock_price["price"]})
    
    # get total price
    for i in range(len(data)):
        row = data[i]
        row.update({'total_stock': data[i]["stock_price"]*data[i]["stock_number"]})
    
    # get current user's cash
    cash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])

    # get total
    total = 0
    for i in range(len(data)):
        total += data[i]["total_stock"]

    total += cash[0]["cash"]

    return render_template('index.html', data=data, cash_current = float(cash[0]["cash"]), total=float(total))


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    
    if request.method == "POST":
        symbol = request.form.get("symbol")
        num_shares = request.form.get("shares")

        # check if number of shares is anything other than int
        if (not num_shares.isdigit()):
           return apology("Value has a none digit symbol")

        num_shares = int(num_shares)
        
        if not symbol:
            return apology('Symbol Empty')
        if not num_shares or num_shares <= 0:
            return apology('Share Number Empty')
        if num_shares < 0:
            return apology("Can't have empty numbers of shares")
        
        # to look up quote info
        quote = lookup(symbol)

        # check if the stock exists
        if quote == None:
            return apology("We don't have that stock")

        cash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])
        post_cash = float(cash[0]["cash"]) - (float(quote["price"])*num_shares)

        if post_cash < 0:
            return apology("You are too poor haha")
        
        # Subtract user's cash value by the value of the stock price * stock num
        db.execute("UPDATE users SET cash = ? WHERE id = ?", post_cash, session["user_id"])
        
        # get username
        username = db.execute("SELECT username FROM users WHERE id=?", session["user_id"])
        
        # get purchased symbols
        purchased = db.execute("SELECT stock_name FROM stock WHERE username_stock=?", username[0]["username"])
        
        # check if the stock has alredy been purchased
        if len(purchased) > 0:
            for i in range(len(purchased)):
                if symbol == purchased[i]["stock_name"]:
                    id = db.execute("SELECT id FROM stock WHERE stock_name=?", symbol)
                    stock_number = db.execute("SELECT stock_number FROM stock WHERE id=?", id[0]["id"])
                    db.execute("UPDATE stock SET stock_number=? WHERE id=?", stock_number[0]["stock_number"] + num_shares, id[0]["id"])
                    return redirect("/")
        # insert into stock table the stock name and stock number
        db.execute("INSERT INTO stock (username_stock, stock_name, stock_number) VALUES (?,?,?)", username[0]["username"], symbol, num_shares)

        # get time and date
        time = datetime.now()
        
        # add buy action into history
        db.execute("INSERT INTO history (username, user_action, symbol, stock_num, year, month, day, hour, minutes) \
            VALUES (?,'buy',?,?,?,?,?,?,?)", username[0]['username'], symbol, num_shares, time.year, time.month, time.day, time.hour, time.minute)

        return redirect("/")
    else:
        return render_template('buy.html')


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    
    id = session["user_id"]
    username = db.execute("SELECT username FROM users WHERE id = ?", id)
    history = db.execute("SELECT * FROM history WHERE username = ?", username[0]['username'])
    
    return render_template('history.html', history=history)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "POST":
        symbol = request.form.get("symbol")
        if not symbol:
            return apology("Stock quote field empty")

        quote = lookup(symbol)

        if quote == None:
            return apology("WE don't have that stock")
        else:
            # {'name': 'Apple Inc', 'price': 169.75, 'symbol': 'AAPL'}
            usd_price = usd(quote["price"])
            return render_template('quote.html', quote_info='The price of ' + symbol + ' from ' + quote["name"] + ' is: ' + usd_price)
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "POST":

        # Getting data from the user's register form
        username = request.form.get("username")
        password = request.form.get("password")
        retypepw = request.form.get("confirmation")
        username_database = db.execute("SELECT username FROM users WHERE username=?", username)
        
        # Check if the data is empty
        if not username:
            return apology("username field empty")
        
        elif not password:
            return apology("password field empty")
        
        elif not retypepw:
            return apology("re-type field empty")

        # Check if username is taken
        if len(username_database) > 0:
            if username == username_database[0]["username"]:
                return apology("Username has been taken")

        # Check if pw and retype are the same
        if password != retypepw:
            return apology("Password and re-type password is mismatch")
        
        # Hasing the password
        hash = generate_password_hash(password, method='pbkdf2:sha256', salt_length=8)

        # Insert into the database user info and hashed password
        db.execute("INSERT INTO users (username, hash) VALUES (?,?);", username, hash)
        
        # Redirect user to the index page for them to manually login
        return redirect("/")

    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    if request.method == "POST":
        # get stock from user
        symbol = request.form.get("symbol")
        sell_num = request.form.get("shares")

        # check if number of shares is anything other than int
        if (not sell_num.isdigit()):
           return apology("Value has a none digit symbol")

        sell_num = int(sell_num)

        if not symbol:
            return apology("Symbol field is empty")
        if not sell_num:
            return apology("Share number field is empty")

        # get stock price from db
        username = db.execute("SELECT username FROM users WHERE id = ?", session["user_id"])
        stock_price = lookup(symbol)

        # get current stock owned
        stock_own = db.execute("SELECT stock_number FROM stock WHERE username_stock = ? AND stock_name = ?", username[0]["username"], symbol)
        
        # sell stock
        cash = db.execute("SELECT cash FROM users WHERE username = ?", username[0]["username"])
        total_stock = float(stock_price["price"]) * int(sell_num)
        cash[0]["cash"] += total_stock

        # update cash database
        db.execute("UPDATE users SET cash = ? WHERE username = ?", cash[0]["cash"], username[0]["username"])

        # update stock databse
        stock_left = stock_own[0]["stock_number"] - int(sell_num)
        if stock_left == 0:
            db.execute("DELETE FROM stock WHERE username_stock = ? AND stock_name = ?", username[0]["username"], symbol)
        elif stock_left > 0:
            db.execute("UPDATE stock SET stock_number = ? WHERE username_stock = ? AND stock_name = ?", stock_left, username[0]["username"], symbol)
        else:
            return apology("YOU DON'T HAVE ENOUGH STOCK TO SELL")

        # get time and date
        time = datetime.now()
        
        # add buy action into history
        db.execute("INSERT INTO history (username, user_action, symbol, stock_num, year, month, day, hour, minutes) \
            VALUES (?,'sell',?,?,?,?,?,?,?)", username[0]['username'], symbol, sell_num, time.year, time.month, time.day, time.hour, time.minute)


        return redirect("/")
    else:
        # get all stock symbol from current user
        id = session["user_id"]
        username = db.execute("SELECT username FROM users WHERE id = ?", id)

        symbol = db.execute("SELECT stock_name FROM stock WHERE username_stock = ?", username[0]["username"])
        return render_template("sell.html", symbol=symbol)


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)

# to run app.py with python3
if __name__ == "__main__":
    app.run(debug=True)
