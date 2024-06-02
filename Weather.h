#ifndef WEATHERAPICPP_WEATHER_H
#define WEATHERAPICPP_WEATHER_H

#include <iostream>

class Weather {
private:
    int daysAmount;

    double latitude;

    double longitude;

    std::string cityName;

    static const int LINE_LENGTH = 99;

    static const int DAY_LENGTH = 24;

public:
    Weather(std::string _cityName, int _daysAmount);

    void printWeather() const;

    std::string getCityName();

    class WeatherException : public std::exception {
    private:
        std::string errorMessage;
    public:
        explicit WeatherException(std::string);

        std::string what();
    };
};


#endif
