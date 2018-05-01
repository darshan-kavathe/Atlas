#include "places.h"
#include <iomanip>
#include <chrono>
#include <fstream>
#include <sstream>
#include <memory>
#include <algorithm>
#include <cmath>
#include <limits>
#include <iostream>
#include <fstream>
#include "places.h"
#define PI 3.14159265
using std::cerr;
using std::cin;
using std::cout;
using std::endl;
using std::ios_base;
using std::istream;
using std::filebuf;
using std::chrono::system_clock;
using std::cout;
using std::endl;
using std::flush;
using std::function;
using std::ios;
using std::istream;
using std::streampos;
using std::string;
using std::ws;
using std::make_shared;
using std::shared_ptr;
using std::vector;

int main(int argc, char* argv[]) {
    filebuf inbuf;

    if (argc >= 2) {
        cerr << "Usage: No arguments" << endl;
        return 0;
    }

    istream is(inbuf.is_open() ? &inbuf : cin.rdbuf());

    Places the_loop(is, !inbuf.is_open());
    the_loop.main_loop();

    return 0;
}
Places::Places(istream& is, bool std_in) :
        in_{is},
        stdin_{std_in},
        commands_{} {
    commands_["help"] = []() {
        cout <<
             "closest: find the closest place to one place\n" <<
             "closestAll: find the closest pair from all places\n" <<
             "distance: find the distance between two places\n" <<
             "find: find a place\n" <<
             "help: display this help message\n" <<
             "read: read a new collection of places\n" <<
             "quit: exit the program\n";
    };


    commands_["read"] = [this]() {
       std::string file_;
        in_>>file_;
        string line;
        int count =0;
        std::ifstream readFile(file_);
        std::string country, city, accent_city, region,population,latitude,longitude;
        place_set.clear();
        while(getline(readFile,line))   {
            count++;
            std::stringstream iss(line);
            getline(iss, country, ',');
            getline(iss, city, ',');
            getline(iss, accent_city, ',');
            getline(iss, region, ',');
            getline(iss, population, ',');
            getline(iss, latitude, ',');
            getline(iss, longitude, ',');
            int pop_i = -1;
            if(population!="") {
                pop_i = std::stoi(population);
            }
            float lat_f=std::stof(latitude);
            float lon_f=std::stof(longitude);


            place_set.emplace(country, city, accent_city, region,pop_i,lat_f,lon_f);




        }
        readFile.close();
        cout<<"places deleted"<<count-place_set.size()<<"\n";
        for(auto&& it:  place_set){
            this->pl_v.push_back(&it);// push_back(make_shared(&it));
            this->find_set.insert(&it);
            //cout<< it.latitude_<<" "<<it.logitude_<<"\n";
        }
        /*
        cout<<"vector\n";
        for(auto it: pl_v){
            cout<< (*it).latitude_<<" "<<(*it).logitude_<<"\n";
        }*/
        std::sort(pl_v.begin(), pl_v.end(),[](const Place2* a,const Place2* b){
            if ((*a).latitude_ < (*b).latitude_){ return true; }
            else if((*a).latitude_ >(*b).latitude_){ return false; }
            else{ if ((*a).logitude_ < (*b).logitude_){ return true; }
                else if((*a).logitude_>(*b).logitude_){ return false; }
                else{cout<<"problem here: they are equal"; }
            }
            return false;
        });


    };

    /*
     *
     */
    commands_["closest"] = [this]() {
        cout<<"Enter Place\n";
        std::string country;
        cout<<"Country: ";
        in_>>country;
        std::string city;
        cout<<"City: ";
        in_>>city;
        std::string region;
        cout<<"Region: ";
        in_>>region;
        bool found = false;
        float lat, lon,lat2,lon2 =0;
        float minimum = 100000000000000000;
        for(auto it: place_set){
            if (it.city_==city && it.country_==country && it.region_==region){
                found = true;
                lat = it.latitude_;
                lon = it.logitude_;
                cout<<"Country: "<<country<<", City: "<<city<<", Region: "<<region;
                cout<<", Latitude: "<<lat<<", Longitude: "<<lon<<"\n";
                break;
            }
        }
        const Place2* pt_p= nullptr;
        if (!found){cout<<city<<" not found";}
        else{
            for(auto& it: place_set){
                    lat2 = it.latitude_;
                    lon2 = it.logitude_;
                    if(!(lat==lat2&&lon==lon2)) {
                        float temp_dist =distance(lon, lon2, lat2,lat);
                        if(temp_dist < minimum){
                            minimum = temp_dist;
                            pt_p=&it;
                        }
                    }
            }
            cout << "Country: " << pt_p->country_ << ", City: " << pt_p->city_ << ", Region: " << pt_p->region_;
            cout << ", Latitude: " << pt_p->latitude_ << ", Longitude: " << pt_p->logitude_ << "\n";
            cout<<minimum<<" miles away\n";
        }

    };

    /*
     *
     */
    commands_["distance"] = [this]() {
        cout << "Enter Place\n";
        std::string country;
        cout << "Country: ";
        in_ >> country;
        std::string city;
        cout << "City: ";
        in_ >> city;
        std::string region;
        cout << "Region: ";
        in_ >> region;
        cout << "Enter Place\n";
        std::string country2;
        cout << "Country: ";
        in_ >> country2;
        std::string city2;
        cout << "City: ";
        in_ >> city2;
        std::string region2;
        cout << "Region: ";
        in_ >> region2;
        bool found = false;
        bool found2 = false;
        float lon1, lon2, lat2, lat1 = 0;
        for (auto it: place_set) {
            if (!found && it.city_ == city && it.country_ == country && it.region_ == region) {
                found = true;
                lon1 = it.logitude_;
                lat1 = it.latitude_;
                cout << "Country: " << country << ", City: " << city << ", Region: " << region;
                cout << ", Latitude: " << lat1 << ", Longitude: " << lon1 << "\n";
            }
            if (!found2 && it.city_ == city2 && it.country_ == country2 && it.region_ == region2) {
                found2 = true;
                lon2 = it.logitude_;
                lat2 = it.latitude_;
                cout << "Country: " << country2 << ", City: " << city2 << ", Region: " << region2;
                cout << ", Latitude: " << lat2 << ", Longitude: " << lon2 << "\n";
            }
        }
        if (!found) { cout << city << " not found"; }
        if (!found2) { cout << city2 << " not found"; }
        if (found && found2) {
            cout << distance(lon1, lon2, lat2,lat1) << " miles away.\n";
        }
    };

    /*
     *
     */
    commands_["find"] = [this]() {
        /*const Place2* it = find();
        if (it!= nullptr) {
            cout << "Country: " << it->country_ << ", City: " << it->city_ << ", Region: " << it->region_;
            cout << ", Latitude: " << it->latitude_ << ", Longitude: " << it->logitude_ << "\n";
        }
        */
        cout<<"Enter Place\n";
        std::string country;
        cout<<"Country: ";
        in_>>country;
        std::string city;
        cout<<"City: ";
        in_>>city;
        std::string region;
        cout<<"Region: ";
        in_>>region;
        bool found = false;
        for(auto it: place_set){
            if (it.city_==city && it.country_==country && it.region_==region){
                found = true;
                cout<<"Country: "<<country<<", City: "<<city<<", Region: "<<region;
                        cout<<", Latitude: "<<it.latitude_<<", Longitude: "<<it.logitude_<<"\n";
                break;
            }
        }
        if (!found){cout<<city<<" not found";}
    };

    commands_["closestAll"] = [this]() {
        std::vector<const Place2*> newX(pl_v);
        std::vector<const Place2*> aux(pl_v.size());
        std::sort(pl_v.begin(), pl_v.end(),[](const Place2* a,const Place2* b){
            if ((*a).logitude_ < (*b).logitude_){ return true; }
            else if((*a).logitude_>(*b).logitude_){ return false; }
            else {
                if ((*a).latitude_ < (*b).latitude_){ return true; }
                else if((*a).latitude_ >(*b).latitude_){ return false; }
                else{cout<<"problem here: they are equal"; }
            }
            return false;
        });
        closestPair(newX,pl_v,aux,0,pl_v.size());
    };
}

void Places::main_loop() {
    string cmd;
    while (true) {
        if (stdin_) {
            cout << "> " << flush;
        } else {
            // do this stream manipulation so we can print the entire line
            // if the command is coming from a file, but we want to reset
            // it before the command is read in later
            in_ >> ws;
            streampos pos{in_.tellg()};
            string line{};
            getline(in_, line);
            cout << "> " << line << endl;
            in_.seekg(pos);
        }

        in_ >> cmd;
        if (cmd == "quit") {
            break;
        } else if (commands_.count(cmd) == 1) {
            auto start = std::chrono::system_clock::now();
            commands_[cmd]();
            auto end = std::chrono::system_clock::now();
            std::chrono::duration<double> diff = end-start;
            cout << "*** elapsed time: "<< int(diff.count()) << " seconds.\n";
        } else {
            cout << "unrecognized command!" << endl;
            // consume rest of line
            string tmp;
            getline(in_, tmp);
        }
    }
}

float Places::distance(float lon1, float lon2, float lat2, float lat1){
    float dlon = (lon2 - lon1)*PI/180.0;
    float dlat = (lat2 - lat1)*PI/180.0;
    float a = powf((sinf(dlat/2.0)),2.0) + cosf(lat1) * cosf(lat2) * powf((sinf(dlon /2.0)),2.0);
    float c = 2.0 * atan2f(sqrtf(a), sqrtf(1.0 - a));
    float d = 3961.0 * c;
    return d;
}

const Place2* Places::find(){
    cout<<"Enter Place\n";
    std::string country;
    cout<<"Country: ";
    in_>>country;
    std::string city;
    cout<<"City: ";
    in_>>city;
    std::string region;
    cout<<"Region: ";
    bool found = false;
    for(auto&& it: place_set){
        if (it.city_==city && it.country_==country && it.region_==region){
            found = true;
            /*
            cout<<"Country: "<<country<<", City: "<<city<<", Region: "<<region;
            cout<<", Latitude: "<<it.latitude_<<", Longitude: "<<it.logitude_<<"\n";
             */
            return &it;
        }
    }
    if (!found){
        cout<<city<<" not found";
        return nullptr;
    }
}

bool less(const Place2* a, const Place2* b) {
    if(a->logitude_ < b->logitude_) return true;
    if(a->logitude_ > b->logitude_) return false;
    return a->latitude_ < b->latitude_;
}

void merge( std::vector<const Place2 *> &a,
            std::vector<const Place2 *> &aux, int lo, int mid, int hi)   {
    int i, j, k;
    for(k = lo; k <= hi; k++)
        aux.at(k) = a.at(k);

    i = lo; j = mid + 1; k = lo;
    while(i <= mid && j <= hi){
        a.at(k++) = less(aux.at(i), aux.at(j)) ? aux.at(i++) : aux.at(j++);
    }
    while(i <= mid) {
        a.at(k++) = aux.at(i++);
    }
}
float Places::closestPair(std::vector<const Place2 *> &newX, std::vector<const Place2 *> &newY,
                         std::vector<const Place2 *> &aux, int min, int max) {
    if(max <= min)
        return std::numeric_limits<float>::infinity();

    int mid = min + (max - min)/2;
    float delta = closestPair(newX , newY, aux, min, mid);
    float dist = closestPair(newX, newY, aux, mid+1, max);
    if(dist < delta)
        delta = dist;

    merge(newY, aux, min, mid, max);
    int M = 0, i, j;
    for(i = min; i <= max; i++)
        if(abs(distance(0,0,newY.at(i)->latitude_,newX.at(mid)->latitude_)) < delta)
            aux.at(M++) = newY.at(i);

    float dist_1, t;
    for(i = 0; i < M; i++)  {
        for(j = i+1; j < M && ((distance(aux.at(j)->logitude_ , aux.at(i)->logitude_,0,0)) < delta); j++) {
            dist_1 = distance(aux.at(i)->logitude_, aux.at(j)->logitude_,aux.at(j)->latitude_, aux.at(i)->latitude_);
                if(dist_1< delta)    {
                delta = dist_1;
                if(delta < bestDistance) {
                    bestDistance = delta;
                    p1 = aux.at(i);
                    p2 = aux.at(j);
                }
            }
        }
    }
    cout << "Country: " << p1->country_ << ", City: " << p1->city_ << ", Region: " << p1->region_;
    cout << ", Latitude: " << p1->latitude_ << ", Longitude: " << p1->logitude_ << "\n";
    cout << "Country: " << p2->country_ << ", City: " << p2->city_ << ", Region: " << p2->region_;
    cout << ", Latitude: " << p2->latitude_ << ", Longitude: " << p2->logitude_ << "\n";
    cout<<delta<<" miles away\n";
}
