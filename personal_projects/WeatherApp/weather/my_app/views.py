import datetime
import requests

from django.shortcuts import render

# Create your views here.


def index(request):
    API_KEY=open("../../API_KEY",'r').read()
    weather_url="https://api.openweathermap.org/data/2.5/weather?q={}&appid={}"
    forecast_url="https://api.openweathermap.org/data/2.5/onecall?lat={}&lon={}&exclude=current,minutely,hourly,alerts&appid={}"


