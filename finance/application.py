import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

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

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""

    # Query database for shares owned by user
    data = db.execute(
        "SELECT stock, SUM(shares) AS shares FROM shares WHERE id = :id GROUP BY stock HAVING SUM(shares) > 0", id=session["user_id"])

    # Represents sum of share values and cash
    total = 0

    # Look up stock details and add more key-value pairs to the dict objects in data
    for item in data:
        stock = lookup(item["stock"])
        item["name"] = stock["name"]
        item["price"] = stock["price"]
        item["total"] = item["shares"] * item["price"]
        total += item["total"]

    # Query database for cash currently available with user
    cash = db.execute("SELECT cash FROM users WHERE id = :id", id=session["user_id"])[0]["cash"]
    total += cash

    return render_template("index.html", data=data, cash=cash, total=total)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure stock symbol was submitted
        if not request.form.get("symbol"):
            return apology("must provide stock symbol")

        # Ensure number of shares was submitted
        elif not request.form.get("shares"):
            return apology("must provide number of shares")

        # Ensure numbers of shares is a positive integer
        elif not request.form.get("shares").isdecimal():
            return apology("number of shares must be a positive integer")

        # Get stock details
        stock = lookup(request.form.get("symbol"))

        # Ensure valid stock symbol was submitted
        if not stock:
            return apology("invalid stock symbol")

        # Number of shares user wants to buy. Converting from float to int in case user enters a value like 1.0
        shares = int(float(request.form.get("shares")))

        # Query database for cash currently available with user
        cash = db.execute("SELECT cash FROM users WHERE id = :id", id=session["user_id"])[0]["cash"]

        # Ensure user has enough cash to buy requested number of shares
        if cash < shares * stock["price"]:
            return apology("insufficient cash")

        # Insert purchase details into database
        db.execute("INSERT INTO shares (id, stock, price, shares, transaction_date) VALUES (:id, :stock, :price, :shares, datetime('now'))",
                   id=session["user_id"], stock=stock["symbol"], price=stock["price"], shares=shares)

        # Update cash available with user in database
        db.execute("UPDATE users SET cash = :cash WHERE id = :id", cash=cash - (shares * stock["price"]), id=session["user_id"])

        # Display an alert message after shares are bought
        flash(f"{shares} {stock['symbol']} {'share' if shares == 1 else 'shares'} bought!")

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("buy.html")


@app.route("/check", methods=["GET"])
def check():
    """Return true if username available, else false, in JSON format"""

    # Ensure username was passed
    if not request.args.get("username"):
        return jsonify(False)

    # Query database for user with username passed
    user = db.execute("SELECT id FROM users WHERE username = :username", username=request.args.get("username"))

    # Return True if username does not exist already, else return False
    return jsonify(True) if not len(user) else jsonify(False)


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    # Query database for user's records of shares
    data = db.execute("SELECT * FROM shares WHERE id = :id ORDER BY transaction_date", id=session["user_id"])

    for item in data:
        # Add transaction type
        item["type"] = "Bought" if item["shares"] > 0 else "Sold"

        # Change number of shares to positive, if it is negative
        if item["shares"] < 0:
            item["shares"] = item["shares"] + (-2 * item["shares"])

    return render_template("history.html", data=data)


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
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

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

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure stock symbol was submitted
        if not request.form.get("symbol"):
            return apology("must provide stock symbol")

        # Get stock details
        stock = lookup(request.form.get("symbol"))

        # Ensure valid stock symbol was submitted
        if not stock:
            return apology("invalid stock symbol")

        return render_template("quoted.html", stock=stock)

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username")

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password")

        # Ensure password confirmation was submitted
        elif not request.form.get("confirmation"):
            return apology("must confirm password")

        # Ensure passwords match
        elif not request.form.get("password") == request.form.get("confirmation"):
            return apology("passwords do not match")

        # Insert new user into database
        user = db.execute("INSERT INTO users (username, hash) VALUES (:username, :hash)",
                          username=request.form.get("username"), hash=generate_password_hash(request.form.get("password")))

        # Ensure username is unique
        if not user:
            return apology("username already exists")

        # Remember which user has registered
        session["user_id"] = user

        # Display an alert message after user is registered
        flash("Registered!")

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure stock symbol was submitted
        if not request.form.get("symbol"):
            return apology("must provide stock symbol")

        # Ensure number of shares was submitted
        elif not request.form.get("shares"):
            return apology("must provide number of shares")

        # Ensure numbers of shares is a positive integer
        elif not request.form.get("shares").isdecimal():
            return apology("number of shares must be a positive integer")

        # Get stock details
        stock = lookup(request.form.get("symbol"))

        # Query database for number of stocks currently available with user
        count = db.execute("SELECT SUM(shares) AS shares FROM shares WHERE id = :id AND stock = :stock",
                           id=session["user_id"], stock=stock["symbol"])[0]["shares"]

        # Number of shares user wants to sell. Converting from float to int in case user enters a value like 1.0
        shares = int(float(request.form.get("shares")))

        # Ensure user has enough shares to sell
        if count < shares:
            return apology("insufficient shares")

        # Insert sale details into database
        db.execute("INSERT INTO shares (id, stock, price, shares, transaction_date) VALUES (:id, :stock, :price, :shares, datetime('now'))",
                   id=session["user_id"], stock=stock["symbol"], price=stock["price"], shares=-shares)

        # Query database for cash currently available with user
        cash = db.execute("SELECT cash FROM users WHERE id = :id", id=session["user_id"])[0]["cash"]

        # Update cash available with user in database
        db.execute("UPDATE users SET cash = :cash WHERE id = :id", cash=cash + (shares * stock["price"]), id=session["user_id"])

        # Display an alert message after shares are sold
        flash(f"{shares} {stock['symbol']} {'share' if shares == 1 else 'shares'} sold!")

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        stocks = db.execute("SELECT stock FROM shares WHERE id = :id GROUP BY id, stock HAVING SUM(shares) > 0",
                            id=session["user_id"])
        return render_template("sell.html", stocks=stocks)


@app.route("/change", methods=["GET", "POST"])
@login_required
def change_password():
    """Change user's password"""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure old password was submitted
        if not request.form.get("old-password"):
            return apology("must provide old password")

        # Ensure new password was submitted
        elif not request.form.get("new-password"):
            return apology("must provide new password")

        # Ensure new password was confirmed
        elif not request.form.get("confirmation"):
            return apology("must confirm new password")

        # Ensure passwords match
        elif not request.form.get("new-password") == request.form.get("confirmation"):
            return apology("passwords do not match")

        # Query database for user's old hash
        hash = db.execute("SELECT hash FROM users WHERE id = :id",
                          id=session["user_id"])[0]["hash"]

        # Ensure old password is correct
        if not check_password_hash(hash, request.form.get("old-password")):
            return apology("incorrect old password", 403)

        # Update password in database
        db.execute("UPDATE users SET hash = :hash WHERE id = :id",
                   hash=generate_password_hash(request.form.get("new-password")), id=session["user_id"])

        # Display an alert message after password is changed
        flash("Password changed!")

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("change.html")


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)