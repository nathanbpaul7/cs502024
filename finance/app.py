import os

from cs50 import SQL
from flask import Flask, jsonify, flash, redirect, render_template, request, session
from flask_session import Session
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    # show portfolio of stocks in HTML table, gather table inputs from database
    stocks = db.execute("SELECT symbol, shares FROM portfolio WHERE user_id = ?", session["user_id"])
    totalValue = 0
    cashRow = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])
    cash = cashRow[0]["cash"]
    for stock in stocks:
        if not lookup(stock["symbol"]):
            return apology("sorry your stock information was faulty", 400)
        quote = lookup(stock["symbol"])
        stock["price"] = quote["price"]
        stock["total"] = quote["price"] * stock["shares"]
        totalValue += stock["total"]
    totalValue += cash

    # boolean to make sure we alert user if there was a recent successful transaction that redirected user to index

    if session.get("bought"):
        session.pop("bought", None)
        return render_template("index.html", stocks=stocks, cash=cash, totalValue=totalValue, bought=1)
    elif session.get("sold"):
        session.pop("sold", None)
        return render_template("index.html", stocks=stocks, cash=cash, totalValue=totalValue, sold=1)
    else:
        return render_template("index.html", stocks=stocks, cash=cash, totalValue=totalValue)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":
        # Ensure symbol was submitted
        if not request.form.get("symbol"):
            return apology("need symbol to initiate purchase", 400)
        # Ensure symbol is a valid stock symbol
        if not lookup(request.form.get("symbol")):
            return apology("must be a valid stock in order to buy.", 400)
        # Ensure they specify a positive integer for shares
        try:
            shares = int(request.form.get("shares"))
        except ValueError:
            return apology("must specify positive integer of shares to purchase", 400)
        if not shares or shares < 1:
            return apology("must specify positive integer of shares to purchase", 400)
        # initialize variables from purchase form
        quote = lookup(request.form.get("symbol"))
        symbol = quote["symbol"]
        sharePrice = quote["price"]
        purchaseTotal = sharePrice * shares
        cashQuery = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])
        cashAvail = cashQuery[0]["cash"]

        # if user has enough funds available, update their cash on hand, track purchase in separate transactions table
        if cashAvail >= purchaseTotal:
            cashAfter = cashAvail - purchaseTotal
            db.execute("UPDATE users SET cash = ? WHERE id = ?", cashAfter, session["user_id"])
            db.execute("INSERT INTO transactions (type, symbol, shares, share_price, total, user_id) VALUES ('buy', ?, ?, ?, ?, ?)", symbol, shares, sharePrice, purchaseTotal, session["user_id"])

            # let's see if they already own some of this stock and update if so, otherwise insert query
            stockOwned = db.execute("SELECT * FROM portfolio WHERE user_id = ? AND symbol = ?", session["user_id"], symbol)
            if stockOwned:
                db.execute("UPDATE portfolio SET shares = shares + ? WHERE user_id = ? AND symbol = ?", shares, session["user_id"], symbol)
            else:
                db.execute("INSERT INTO portfolio (user_id, symbol, shares) VALUES (?, ?, ?)", session["user_id"], symbol, shares)
            session["bought"] = 1
            return redirect("/")
        else:
            return apology("insufficient funds to purchase stocks, please re-evaluate your purchase", 400)

    else:
        return render_template("buy.html")

@app.route("/look")
@login_required
def look():
   symbol = request.args.get("ticker")
   quote = lookup(symbol)
   if not quote:
       return jsonify({"error": "not a valid symbol"}), 400
   else:
        return jsonify(quote)

@app.route("/history")
@login_required
def history():
    transactions = db.execute("SELECT * FROM transactions WHERE user_id = ?", session["user_id"])
    return render_template("history.html", transactions=transactions)


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
        rows = db.execute(
            "SELECT * FROM users WHERE username = ?", request.form.get("username")
        )

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(
            rows[0]["hash"], request.form.get("password")
        ):
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
    if request.method == "POST":
        # Ensure symbol was submitted
        if not request.form.get("symbol"):
            return apology("need symbol to retreive quote", 400)
        # Ensure symbol is a valid stock symbol
        if not lookup(request.form.get("symbol")):
            return apology("must be a valid stock symbol to retrieve quote.", 400)
        quote = lookup(request.form.get("symbol"))
        return render_template("quoted.html", price=quote["price"], symbol=quote["symbol"])
    else:
        return render_template("quote.html")



@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "POST":
        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 400)

        # Ensure username doesn't match an exisiting one
        users = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))
        if users:
            return apology("username already exists, please try another", 400)

        # Ensure password was submitted
        elif not request.form.get("password") or not request.form.get("confirmation"):
            return apology("must provide password", 400)

        elif request.form.get("password") != request.form.get("confirmation"):
            return apology("passwords must match each other, please try again")

        username = request.form.get("username")
        hash = generate_password_hash(request.form.get("password"))

        # Update database to include new user
        db.execute("INSERT INTO users (username, hash) VALUES(?, ?)", username, hash)

        # Send user to login page
        return redirect("/login")

    else:
        return render_template("register.html")



@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    # Sell shares of stocks
    if request.method == "POST":
        # Ensure symbol was submitted
        if not request.form.get("symbol"):
            return apology("need symbol to initiate purchase", 400)
        # Ensure symbol is within their stock portfolio
        stocks = db.execute("SELECT symbol, shares FROM portfolio WHERE user_id = ? AND symbol = ?", session["user_id"], request.form.get("symbol"))
        if not stocks:
            return apology("must be a stock in your portfolio in order to sell", 400)
        # Ensure they specify a positive integer for shares
        try:
            shares = int(request.form.get("shares"))
        except ValueError:
            return apology("must specify positive integer of shares to sell", 400)
        if not shares or shares < 1:
            return apology("must specify positive integer of shares to sell", 400)

        # Ensure they have enough shares in their portfolio to sell posted amount
        if shares > stocks[0]["shares"]:
            return apology("you cannot sell more shares than you currently own", 400)

        # initialize variables from sell form, update database with new cash input
        quote = lookup(request.form.get("symbol"))
        symbol = quote["symbol"]
        sharePrice = quote["price"]
        sellTotal = sharePrice * shares
        db.execute("UPDATE users SET cash = cash + ? WHERE id = ?", sellTotal, session["user_id"])
        db.execute("INSERT INTO transactions (type, symbol, shares, share_price, total, user_id) VALUES ('sell', ?, ?, ?, ?, ?)", symbol, shares, sharePrice, sellTotal, session["user_id"])

        # deal with scenario where they sell all of their shares, meaning delete stock from portfolio
        if shares == stocks[0]["shares"]:
            db.execute("DELETE FROM portfolio WHERE user_id = ? AND symbol = ?", session["user_id"], symbol)
        else:
            db.execute("UPDATE portfolio SET shares = shares - ? WHERE user_id = ? AND symbol = ?", shares, session["user_id"], symbol)
        session["sold"] = 1
        return redirect("/")
    else:
        stocks = db.execute("SELECT symbol, shares FROM portfolio WHERE user_id = ?", session["user_id"])
        return render_template("sell.html", stocks=stocks)
