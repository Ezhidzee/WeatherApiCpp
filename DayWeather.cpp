#include "DayWeather.h"

#include <cmath>
#include <numeric>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <algorithm>

DayWeather::DayWeather(std::vector<double> temperatures, std::vector<int> precipitation_probabilities,
                       std::vector<double> precipitations, std::vector<double> visibilities,
                       std::vector<double> wind_speeds, std::vector<std::string> time, std::vector<int> weatherCodes) {
    timesOfDay[0].name = "Night";
    timesOfDay[1].name = "Morning";
    timesOfDay[2].name = "Day";
    timesOfDay[3].name = "Evening";

    date = time[0].substr(8, 2) + " " + months[std::stoi(time[0].substr(5, 2)) - 1];

    for (int i = 0; i < 4; ++i) {
        TimeOfDay &t = timesOfDay[i];

        t.maxTemp = std::round(*std::max_element(temperatures.begin() + 6 * i, temperatures.begin() + 6 * (i + 1)));

        t.minTemp = std::round(*std::min_element(temperatures.begin() + 6 * i, temperatures.begin() + 6 * (i + 1)));

        t.maxWindSpeed = std::round(*std::max_element(wind_speeds.begin() + 6 * i, wind_speeds.begin() + 6 * (i + 1)));

        t.minWindSpeed = std::round(*std::min_element(wind_speeds.begin() + 6 * i, wind_speeds.begin() + 6 * (i + 1)));

        t.visibility = std::round(
                std::accumulate(visibilities.begin() + 6 * i, visibilities.begin() + 6 * (i + 1), 0.0) / 6);

        std::stringstream stream;
        stream << std::fixed << std::setprecision(2)
               << std::accumulate(precipitations.begin() + 6 * i, precipitations.begin() + 6 * (i + 1), 0.0) / 6;
        t.precipitation = stream.str();

        t.precipitationProbability = std::round(std::accumulate(precipitation_probabilities.begin() + 6 * i,
                                                                precipitation_probabilities.begin() + 6 * (i + 1),
                                                                0.0) / 6);

        t.weatherDescription = WMO[mostFrequentCode(weatherCodes.begin() + 6 * i, weatherCodes.begin() + 6 * (i + 1))];
    }
}

void DayWeather::printDayWeather() {
    std::cout << '|' << std::string(LINE_LENGTH, '-') << '|' << std::endl;

    std::cout << '|' << std::string((LINE_LENGTH - date.size()) / 2, ' ') + date +
                        std::string(LINE_LENGTH - date.size() - (LINE_LENGTH - date.size()) / 2, ' ') << '|'
              << std::endl << '|' << std::string(LINE_LENGTH, '-') << '|' << std::endl;

    for (auto &i: timesOfDay) {
        std::cout << '|' << std::string((BLOCK_LENGTH - i.name.size()) / 2, ' ') << i.name
                  << std::string(BLOCK_LENGTH - i.name.size() - (BLOCK_LENGTH - i.name.size()) / 2, ' ');
    }
    std::cout << '|' << std::endl;

    for (auto &i: timesOfDay) {
        std::cout << '|' << std::string((BLOCK_LENGTH - i.weatherDescription.size()) / 2, ' ') + i.weatherDescription +
                            std::string(BLOCK_LENGTH - i.weatherDescription.size() -
                                        (BLOCK_LENGTH - i.weatherDescription.size()) / 2, ' ');
    }
    std::cout << '|' << std::endl;

    for (auto &i: timesOfDay) {
        std::string temp = "Temp: " + std::to_string(i.maxTemp) + '(' + std::to_string(i.minTemp) + ") C";
        std::cout << '|' << std::string((BLOCK_LENGTH - temp.size()) / 2, ' ') + temp +
                            std::string(BLOCK_LENGTH - temp.size() - (BLOCK_LENGTH - temp.size()) / 2, ' ');
    }
    std::cout << '|' << std::endl;

    for (auto &i: timesOfDay) {
        std::string wind = "Wind: " + std::to_string(i.minWindSpeed) + '-' + std::to_string(i.maxWindSpeed) + " km/h";
        std::cout << '|' << std::string((BLOCK_LENGTH - wind.size()) / 2, ' ') + wind +
                            std::string(BLOCK_LENGTH - wind.size() - (BLOCK_LENGTH - wind.size()) / 2, ' ');
    }
    std::cout << '|' << std::endl;

    for (auto &i: timesOfDay) {
        std::string visibility = "Visibility: " + std::to_string(i.visibility) + " km";
        std::cout << '|' << std::string((BLOCK_LENGTH - visibility.size()) / 2, ' ') + visibility +
                            std::string(BLOCK_LENGTH - visibility.size() - (BLOCK_LENGTH - visibility.size()) / 2, ' ');
    }
    std::cout << '|' << std::endl;

    for (auto &i: timesOfDay) {
        std::string precipitation = "Precip: " + i.precipitation + " mm";
        std::cout << '|' << std::string((BLOCK_LENGTH - precipitation.size()) / 2, ' ') + precipitation +
                            std::string(BLOCK_LENGTH - precipitation.size() - (BLOCK_LENGTH - precipitation.size()) / 2,
                                        ' ');
    }
    std::cout << '|' << std::endl;

    for (auto &i: timesOfDay) {
        std::string precipitationProb = "Precip prob: " + std::to_string(i.precipitationProbability) + "%";
        std::cout << '|' << std::string((BLOCK_LENGTH - precipitationProb.size()) / 2, ' ') + precipitationProb +
                            std::string(BLOCK_LENGTH - precipitationProb.size() -
                                        (BLOCK_LENGTH - precipitationProb.size()) / 2, ' ');
    }
    std::cout << '|' << std::endl;
}

int DayWeather::mostFrequentCode(std::vector<int>::iterator i, std::vector<int>::iterator j) {
    std::unordered_map<int, int> amounts;
    for (auto it = i; it < j; ++it) ++amounts[*it];
    int mostFrequentCode = 0, counter = 0;
    for (auto &amount: amounts) {
        if (amount.second > counter) {
            counter = amount.second;
            mostFrequentCode = amount.first;
        }
    }
    return mostFrequentCode;
}
