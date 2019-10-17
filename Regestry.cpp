#include <iostream>
#include <vector>
#include <cmath>
#include <unordered_map>
#include <sstream>

std::vector<std::string> split(const std::string& str, char splitSign = ' ')
{
    std::stringstream ss(str);
    std::string token;
    std::vector<std::string> res;
    while (std::getline(ss, token, splitSign))
        res.push_back(token);
    return res;
}
//
const std::string DEF_NAME = "Unknown";
struct Person
{
    std::string Name = DEF_NAME;
    std::unordered_map<std::string, std::string> Traits;
    // for HasTrait interface consistency
    using Trait = std::pair<std::string, std::string>;
};
template <typename ContType>
bool HasTrait(const ContType &traits, const Person::Trait &soughtTrait)
{
    const auto res = traits.find(soughtTrait.first);
    return res != traits.end() && res->second == soughtTrait.second;
}

Person ConvertRawInputToPerson(const std::string &rawInput)
{
    Person person = {};
    static const int TO_RESERVE = 16;
    person.Traits.reserve(TO_RESERVE);
    const std::vector<std::string> traits = split(rawInput, ' ');
    for(auto &trait : traits)
    {
        std::vector<std::string> keyValuePair = split(trait, '=');
        if(keyValuePair.size() != 2)
            throw std::runtime_error("wrong string ==" + trait);
        std::string key = std::move(keyValuePair[0]);
        std::string value = std::move(keyValuePair[1]);
        if (key == "Name")
            person.Name = std::move(value);
        else
            person.Traits[key] = std::move(value);
    }
    return person;
}

class Registry
{
    std::vector<Person> mPersons;
public:
    template <typename T>
    void Add(T &&person)
    {
        mPersons.emplace_back(std::forward<T>(person));
    }

    Person FindMatchingPersonByRawTraits(const std::string &rawTraits)
    {
        const Person soughtPerson = ConvertRawInputToPerson(rawTraits);
        //
        const std::unordered_map<std::string, std::string> targetTraits = std::move(soughtPerson.Traits);
        const bool checkName = soughtPerson.Name != DEF_NAME;

        for(const auto &person : mPersons)
        {
            if(checkName && person.Name != soughtPerson.Name)
                continue;
            bool found = true;
            for (auto &soughtTrait : targetTraits)
            {
                if (!HasTrait(person.Traits, soughtTrait))
                {
                    found = false;
                    break;
                }
            }
            if(found)
                return person;
        }
        return {};
    }
};

using namespace std;
// Entry point
string GetNameFromRegistryByTraits(const vector<string> &rawRegistry, const string &soughtTraits)
try
{
    Registry registry;
    for(auto &rawInput: rawRegistry)
        registry.Add(ConvertRawInputToPerson(rawInput));

    const Person person = registry.FindMatchingPersonByRawTraits(soughtTraits);
    return person.Name;
}
catch(std::exception &)
{
    return "Error";
}


int main() {
    GetNameFromRegistryByTraits({"Name=Xiaoling Sex=Woman Age=26 Country=CH"}, "Name=Xiaoling");
    return 0;
}