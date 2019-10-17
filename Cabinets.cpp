#include <iostream>
#include <list>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <exception>
#include <sstream>
#include <limits>
#include <cmath>

// Cabinet class is implemented as a sort of LRU Cache
class Cabinet{
public:
    using ItemCache = std::unordered_map<int, std::list<uint32_t>::iterator>;
    Cabinet() = delete;
    explicit Cabinet(const int capacity):_capacity(capacity){

    }

    uint32_t insert(const uint32_t item){
        uint32_t res = 0;
        if(_items.size() == _capacity){
            res = _items.back();
            _items.pop_back();
            _itemCache.erase(res);
        }
        _items.push_front(item);
        _itemCache[item] = _items.begin();
        return res;
    }

    void erase(const uint32_t item){
        const auto valNode = _itemCache.find(item);
        _items.erase(valNode->second);
        _itemCache.erase(valNode);
    }

private:
    ItemCache _itemCache;
    std::list<uint32_t> _items;
    int _capacity;
};

class CarpenterOffice{
public:
    explicit CarpenterOffice(const std::vector<int> &capacities): _CABINETS_SIZE(capacities.size()){
        _cabinets.reserve(_CABINETS_SIZE);
        for(auto &capacity: capacities){
            _cabinets.emplace_back(capacity);
        }
    }

    void insert(uint32_t item){
        const auto fromWhere = _findItem(item);
        if(fromWhere == _CABINETS_SIZE){// found outside
            _outside.erase(item);
        }
        else if(fromWhere > -1 && fromWhere < _CABINETS_SIZE){ // found in cabinet
            // item should be erased from it's current cabinet and moved to the first one
            _cabinets[fromWhere].erase(item);
        }

        int whereInsert = 0;
        while(whereInsert < _CABINETS_SIZE){
            uint32_t nextItem = _cabinets[whereInsert].insert(item);
            _itemsCabinets[item] = whereInsert;
            if(nextItem == 0)
                break;
            item = nextItem;
            ++whereInsert;
        }
        // item is stored outside
        if(whereInsert == _CABINETS_SIZE && item != 0){
            _outside.insert(item);
            _itemsCabinets[item] = _CABINETS_SIZE;
        }
    }

    int find(const uint32_t item){
        return _findItem(item);
    }


private:
    // would better use std::optional with std::nullopt for new items
    int _findItem(const uint32_t item){
        const auto cabinet = _itemsCabinets.find(item);
        if(cabinet != _itemsCabinets.end())
            return cabinet->second;
        // check outside
        if(_outside.find(item) != _outside.end())
            return _cabinets.size();
        // item is new
        return -1;
    }

private:
    const size_t _CABINETS_SIZE;
    std::vector<Cabinet> _cabinets;
    std::unordered_set<uint32_t> _outside;
    std::unordered_map<uint32_t, int> _itemsCabinets;
};

std::vector<std::string> split(const std::string& str, const char splitSign = ' ');

//
const int64_t MIN = 0;
const int64_t MAX_CAPACITY = 1024;
const int64_t MAX_CABINETS = 64;
const int64_t MAX_ITEMS_NUM = std::pow(2, 32);
const int64_t MAX_ITEM_VAL = std::pow(2, 32);

template <typename T>
void check(const T v, const T min, const T max){
    if(v <= min ||  v >= max)
        throw std::runtime_error("INPUT_ERROR");
}

int main()
try
{
    std::string cabStr;
    getline(std::cin,cabStr);
    std::vector<std::string> splited = split(cabStr);
    check(splited.size(), static_cast<size_t>(MIN), static_cast<size_t>(MAX_CABINETS));
    //
    std::vector<int> capacities;
    capacities.reserve(splited.size());
    for(const auto &s: splited){
        const int64_t v = std::stoll(s);
        check(v, MIN, MAX_CAPACITY);
        capacities.push_back(v);
    }
    CarpenterOffice office(capacities);
    //
    int64_t numOfItems = 0;
    std::cin>>numOfItems;
    check(numOfItems, MIN, MAX_ITEMS_NUM);
    //
    int counter = 0;
    int64_t item = 0;
    while(counter < numOfItems){
        std::cin>>item;
        check(item, MIN, MAX_ITEM_VAL);
        if(counter == numOfItems - 1){
            const int where = office.find(item);
            if(where == capacities.size())
                std::cout<<"OUTSIDE";
            else if(where == -1)
                std::cout<<"NEW";
            else
                std::cout<<where + 1;
        } else{
            office.insert(item);
        }
        ++counter;
    }
    return 0;
}
catch(const std::runtime_error &err) {
    std::cout<<err.what();
    return 0;
}

std::vector<std::string> split(const std::string& str, const char splitSign)
{
    std::stringstream ss(str);
    std::string token;
    std::vector<std::string> res;
    while (std::getline(ss, token, splitSign))
        res.push_back(token);
    return res;
}
