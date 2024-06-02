#include "Weather.h"

#include "nlohmann/json.hpp"

#include <iostream>
#include <vector>
#include <conio.h>
#include <chrono>
#include <fstream>

const int KEY_N = 110;
const int KEY_P = 112;
const int KEY_PLUS = 43;
const int KEY_MINUS = 45;
const int KEY_ESC = 27;

int main() {
    nlohmann::json config;
    try {
        std::fstream f("config.json");
        config = nlohmann::json::parse(f);
    } catch (std::exception &e) {
        std::cerr << "Error reading the config, check the integrity of the config.json file" << std::endl;
        system("pause");
        return 0;
    }

    int currentCity = 0;
    std::vector<Weather> cities;

    std::string continueDesire = "y";
    while (continueDesire == "y") {
        system("cls");
        std::string cityName;
        std::cout << "Enter the name of the city: ";
        std::getline(std::cin, cityName);
        if (cityName.empty()) std::getline(std::cin, cityName);

        try {
            cities.emplace_back(cityName, config["daysAmount"]);
            config["citiesNames"].push_back(cityName);
        } catch (Weather::WeatherException &e) {
            std::cout << e.what() << std::endl;
            continue;
        }

        system("cls");
        std::cout << "Would you like to enter another city name? (y/n) ";
        std::cin >> continueDesire;
    }

    system("cls");
    try {
        cities[currentCity].printWeather();
        int key_code;
        auto start = std::chrono::high_resolution_clock::now();
        while (true) {
            auto end = std::chrono::high_resolution_clock::now();
            if (std::chrono::duration_cast<std::chrono::minutes>(end - start).count() >= config["updateFrequency"]) {
                system("cls");
                cities[currentCity].printWeather();
                start = std::chrono::high_resolution_clock::now();
            }
            switch ((key_code = _getch())) {
                case KEY_ESC:
                    return 0;
                    break;
                case KEY_N:
                    if (currentCity != cities.size() - 1) {
                        system("cls");
                        cities[++currentCity].printWeather();
                    }
                    break;
                case KEY_P:
                    if (currentCity != 0) {
                        system("cls");
                        cities[--currentCity].printWeather();
                    }
                    break;
                case KEY_PLUS:
                    if (config["daysAmount"] != 16) {
                        config["daysAmount"] = int(config["daysAmount"]) + 1;
                        for (auto &city: cities) city = Weather(city.getCityName(), config["daysAmount"]);
                        system("cls");
                        cities[currentCity].printWeather();
                    }
                    break;
                case KEY_MINUS:
                    if (config["daysAmount"] != 1) {
                        config["daysAmount"] = int(config["daysAmount"]) - 1;
                        for (auto &city: cities) city = Weather(city.getCityName(), config["daysAmount"]);
                        system("cls");
                        cities[currentCity].printWeather();
                    }
                    break;
            }
        }
    } catch (Weather::WeatherException &e) { std::cout << e.what() << std::endl; }
}