import cs50
import csv

from flask import Flask, jsonify, redirect, render_template, request

# Configure application
app = Flask(__name__)

# Reload templates when they are changed
app.config["TEMPLATES_AUTO_RELOAD"] = True


@app.after_request
def after_request(response):
    """Disable caching"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/", methods=["GET"])
def get_index():
    """ Route to display form """
    return redirect("/form")


@app.route("/form", methods=["GET"])
def get_form():
    """ Route to display form """
    return render_template("form.html")


@app.route("/form", methods=["POST"])
def post_form():
    """ Route to submit form """

    # Render error page if the user failed to fill any of the fields

    if not request.form.get("name"):
        return render_template("error.html", message="Please enter your name.")
    elif not request.form.get("house"):
        return render_template("error.html", message="Please select your house.")
    elif not request.form.get("position"):
        return render_template("error.html", message="Please choose your Quidditch position.")
    else:
        # Write details entered by user to csv file and redirect the user to the sheet route

        with open('survey.csv', 'a', newline='') as csvfile:
            fieldnames = ['name', 'house', 'position']
            writer = csv.DictWriter(csvfile, fieldnames=fieldnames)
            writer.writerow({'name': request.form.get("name"), 'house': request.form.get(
                "house"), 'position': request.form.get("position")})
        return redirect("/sheet")


@app.route("/sheet", methods=["GET"])
def get_sheet():
    """ Route to display past submissions from csv file """

    with open('survey.csv', 'r', newline='') as csvfile:
        fieldnames = ['name', 'house', 'position']
        reader = csv.DictReader(csvfile, fieldnames=fieldnames)
        entries = [row for row in reader]
    return render_template("sheet.html", headers=fieldnames, entries=entries)