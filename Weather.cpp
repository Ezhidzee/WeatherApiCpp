#include "Weather.h"
#include "DayWeather.h"

#include "cpr/cpr.h"
#include "nlohmann/json.hpp"
#include <vector>

Weather::Weather(std::string _cityName, int _daysAmount) {
    cityName = _cityName;
    daysAmount = _daysAmount;
    cpr::Response coordinates = cpr::Get(cpr::Url{"https://api.api-ninjas.com/v1/city"},
                                         cpr::Header{{"X-Api-Key", "kbfyAP29LmyleFgE7iurgA==GhPdYgoz8M2JXrn2"}},
                                         cpr::Parameters{{"name", cityName}});
    if (nlohmann::json::parse(coordinates.text).empty()) {
        throw WeatherException("A city with this name was not found, try replacing the dash with a space if "
                               "it exists,\nor correct the mistake in the name and try again");
    }

    latitude = nlohmann::json::parse(coordinates.text)[0]["latitude"];
    longitude = nlohmann::json::parse(coordinates.text)[0]["longitude"];
}

void Weather::printWeather() const {
    cpr::Response weatherData = cpr::Get(cpr::Url{"https://api.open-meteo.com/v1/forecast"},
                                         cpr::Parameters{{"latitude",      std::to_string(latitude)},
                                                         {"longitude",     std::to_string(longitude)},
                                                         {"hourly",        "temperature_2m,precipitation_probability,"
                                                                           "precipitation,visibility,windspeed_10m,"
                                                                           "weathercode"},
                                                         {"forecast_days", std::to_string(daysAmount)}});
    nlohmann::json j = nlohmann::json::parse(weatherData.text);
    if (j.empty()) throw WeatherException("Weather data not available for this region.");

    std::vector<double> temperatures;
    for (int i = 0; i < DAY_LENGTH * daysAmount; ++i) temperatures.emplace_back(j["hourly"]["temperature_2m"][i]);
    std::vector<int> precipitation_probabilities;
    for (int i = 0; i < DAY_LENGTH * daysAmount; ++i)
        precipitation_probabilities.emplace_back(j["hourly"]["precipitation_probability"][i]);
    std::vector<double> precipitations;
    for (int i = 0; i < DAY_LENGTH * daysAmount; ++i) precipitations.emplace_back(j["hourly"]["precipitation"][i]);
    std::vector<double> visibilities;
    for (int i = 0; i < DAY_LENGTH * daysAmount; ++i) visibilities.emplace_back(j["hourly"]["visibility"][i]);
    std::vector<double> wind_speeds;
    for (int i = 0; i < DAY_LENGTH * daysAmount; ++i) wind_speeds.emplace_back(j["hourly"]["windspeed_10m"][i]);
    std::vector<std::string> time;
    for (int i = 0; i < DAY_LENGTH * daysAmount; ++i) time.emplace_back(j["hourly"]["time"][i]);
    std::vector<int> weatherCodes;
    for (int i = 0; i < DAY_LENGTH * daysAmount; ++i) weatherCodes.emplace_back(j["hourly"]["weathercode"][i]);

    std::cout << '|' << std::string(LINE_LENGTH, '-') << '|' << std::endl;
    std::cout << '|' << std::string((LINE_LENGTH - cityName.size()) / 2, ' ') + cityName +
                        std::string(LINE_LENGTH - cityName.size() - (LINE_LENGTH - cityName.size()) / 2, ' ') << '|'
              << std::endl;

    for (int i = 0; i < daysAmount; ++i) {
        DayWeather dayWeather(
                std::vector(temperatures.begin() + DAY_LENGTH * i, temperatures.begin() + DAY_LENGTH * (i + 1)),
                std::vector(precipitation_probabilities.begin() + DAY_LENGTH * i,
                            precipitation_probabilities.begin() + DAY_LENGTH * (i + 1)),
                std::vector(precipitations.begin() + DAY_LENGTH * i, precipitations.begin() + DAY_LENGTH * (i + 1)),
                std::vector(visibilities.begin() + DAY_LENGTH * i, visibilities.begin() + DAY_LENGTH * (i + 1)),
                std::vector(wind_speeds.begin() + DAY_LENGTH * i, wind_speeds.begin() + DAY_LENGTH * (i + 1)),
                std::vector(time.begin() + DAY_LENGTH * i, time.begin() + DAY_LENGTH * (i + 1)),
                std::vector(weatherCodes.begin() + DAY_LENGTH * i, weatherCodes.begin() + DAY_LENGTH * (i + 1)));
        dayWeather.printDayWeather();
    }
    std::cout << '|' << std::string(LINE_LENGTH, '-') << '|' << std::endl;
}

std::string Weather::getCityName() { return cityName; }

Weather::WeatherException::WeatherException(std::string _errorMessage) { errorMessage = _errorMessage; }

std::string Weather::WeatherException::what() { return errorMessage; }
