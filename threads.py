# import everything from the module
from weather_threads import *
# to prettily print output
from pprint import pprint
cities = ["Barcelona", "Cairo", "Dubai", "Berlin", "Amsterdam", "Kuala Lumpur"]
pprint(weather_query(cities))