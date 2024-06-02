#ifndef WEATHERAPICPP_DAYWEATHER_H
#define WEATHERAPICPP_DAYWEATHER_H

#include <vector>
#include <string>
#include <unordered_map>

class DayWeather {
private:
    struct TimeOfDay {
        int maxTemp;

        int minTemp;

        int maxWindSpeed;

        int minWindSpeed;

        int visibility;

        std::string precipitation;

        int precipitationProbability;

        std::string name;

        std::string weatherDescription;
    };

    TimeOfDay timesOfDay[4];

    std::string date;

    static int mostFrequentCode(std::vector<int>::iterator, std::vector<int>::iterator);

    inline static std::string months[12] = {"January", "February", "March", "April", "May", "June", "July",
                            "August", "September", "October", "November", "December"};

    inline static std::unordered_map<int, std::string> WMO =
                                      {{0,  "Clear sky"}, {1,  "Mainly clear"},{2,  "Partly cloudy"}, {3,  "Overcast"},
                                      {45, "Fog"}, {48, "Fog"}, {51, "Light drizzle"}, {53, "Drizzle"},
                                      {55, "Dense drizzle"}, {56, "Freezing drizzle"}, {57, "Freezing Drizzle"},
                                      {61, "Slight rain"}, {63, "Rain"}, {65, "Heavy rain"}, {66, "Freezing rain"},
                                      {67, "Freezing rain"}, {71, "Slight snow fall"}, {73, "Snow fall"},
                                      {75, "Heavy snow fall"}, {77, "Snow grains"}, {80, "Slight rain shower"},
                                      {81, "Rain shower"}, {82, "Violent rain shower"}, {85, "Slight snow shower"},
                                      {86, "Heavy snow shower"}, {95, "Thunderstorm"}, {96, "Thunderstorm with hail"},
                                      {99, "Thunderstorm with hail"}};

    static const int LINE_LENGTH = 99;

    static const int BLOCK_LENGTH = 24;

public:
    DayWeather(std::vector<double> temperatures, std::vector<int> precipitation_probabilities,
               std::vector<double> precipitations, std::vector<double> visibilities,
               std::vector<double> wind_speeds, std::vector<std::string> time,
               std::vector<int> weatherCodes);

    void printDayWeather();
};

#endif
