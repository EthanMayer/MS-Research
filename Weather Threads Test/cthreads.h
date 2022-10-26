// length of strings
#define STRLEN 256
// construct first command with the city
#define CMD1 "curl -s 'https://api.openweathermap.org/data/2.5/weather?q=%s"
// append api key and extract only needed info and convert to csv
#define CMD2 "&appid=0fcff34380d112fe210e4a1176fa1c7e' | jq -r '{name: .name, temperature: .main.temp, desc: .weather[].description} | [.name, .temperature, .desc] | @csv'"

// error handling
void error(char * msg);
// the function to be parallelized
void * request(void * inp);
// the function to be wrapped
char ** weather_work(char ** cities, int NUM_CITIES);