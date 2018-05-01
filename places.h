//
// Created by darsh on 4/30/2018.
//

#ifndef LAB3_PLACES_H
#define LAB3_PLACES_H

#include <deque>
#include <functional>
#include <iostream>
#include <unordered_map>
#include <vector>
#include <memory>
#include <unordered_set>


//#include "place.h"
using std::unordered_set;
using std::function;
using std::shared_ptr;
using std::hash;
struct Place2 {
    std::string country_;
    std::string city_;
    std::string accent_city_;
    std::string region_;
    int population_;
    float latitude_;
    float logitude_;
    Place2(std::string& country, std::string& city,
                 std::string& accent_city, std::string& region, int population, float latitude,
                 float longitude):country_{country},city_{city}, accent_city_{accent_city},region_{region},
                                  population_{population_},latitude_{latitude},logitude_{longitude}{
        //std::cout<<"place created in "<<country_<<"\n";
    }
    bool operator == (const Place2 &p) const{
        //std::cout<<"equals\n";
        return (latitude_==p.latitude_)&&(logitude_==p.logitude_);
    }
};
class Places {
public:
    /**
     * Create the loop.
     *
     * @param is the input stream, either stdin or a file
     * @param std_in true if stdin was specified, false if a file
     */
    Places(std::istream& is, bool std_in);

    /**
     * The main loop.
     */
    void main_loop();
    float distance(float lon1, float lon2, float lat2, float lat1);
    const Place2* find();
    float closestPair(std::vector<const Place2*> &pl_v,std::vector<const Place2*> &miny,std::vector<const Place2*> &aux,
    int min, int max);
private:
    /** the input stream */
    std::istream& in_;
    const Place2* p1;
    const Place2* p2;
    float bestDistance;
    /** whether input is from stdin or not */
    bool stdin_;

    /** a map of command strings to lambda expressions to call when
     * invoking the command. */
    //std::unordered_map<std::string, std::string> u;
    std::unordered_map<std::string, std::function<void()>> commands_;

    std::vector<const Place2*> pl_v;
    unordered_set<Place2, function<size_t(const Place2& p)>> place_set{
            10, [](const Place2& p){return std::hash<float>()(p.logitude_)+
                                                       std::hash<float>()(p.latitude_);}
    };
    unordered_set<const Place2*, function<size_t(const Place2*  p)>> find_set{
            10, [](const Place2*  p){return std::hash<std::string>()( (*p).country_)+
                                           std::hash<std::string>()((*p).city_)+
                    std::hash<std::string>()((*p).region_);
                    }
    };
    /*
    unordered_set<shared_ptr<Place2>, function<size_t(const shared_ptr<Place2>& p)>> place_set{
        10, [](const shared_ptr<Place2>& p){return std::hash<float>()(p.get()->logitude_)+
            std::hash<float>()(p.get()->latitude_);}
    };*/
};

#endif //LAB3_PLACES_H
