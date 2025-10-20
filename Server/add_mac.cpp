#include <tinyxml2.h>
#include <iostream>
#include <regex>
#include <vector>
#include <algorithm>


using namespace tinyxml2;

static bool isValidMac(const std::string& mac) {
    static const std::regex re("^([0-9A-Fa-f]{2}:){5}[0-9A-Fa-f]{2}$");
    return std::regex_match(mac, re);
}

bool macExists(std::vector<std::pair<std::string, std::string>> existing, std::string mac)
{
    for(auto it=existing.begin(); it!=existing.end(); it++)
    {
        if(it->first==mac)
            return true;
    }

    return false;
}

void addMacPassword(const std::string& filePath, const std::string& mac, const std::string& password)
{
    XMLDocument doc;
    XMLError err = doc.LoadFile(filePath.c_str());
    if (err == XML_ERROR_FILE_NOT_FOUND) {
        auto* decl = doc.NewDeclaration(R"(xml version="1.0" encoding="UTF-8")");
        doc.InsertFirstChild(decl);
        auto* root = doc.NewElement("maclist");
        doc.InsertEndChild(root);
    } else if (err != XML_SUCCESS) {
        throw std::runtime_error("Eroare la deschiderea XML");
    }

    auto* root = doc.FirstChildElement("maclist");
    if (!root) {
        root = doc.NewElement("maclist");
        doc.InsertEndChild(root);
    }

    // Construim o lista (id, mac) existenta pentru a evita duplicatele
    std::vector<std::pair<std::string, std::string>> existing;
    for (auto* it = root->FirstChildElement("item"); it; it = it->NextSiblingElement("item")) {
        const char* a = it->Attribute("mac");
        const char* m = it->Attribute("password");
        if (a && m) {
            std::string macExist(a);
            std::string passExist(m);
            // normalizăm MAC-urile existente la lowercase

            std::string mmac(macExist);
            for(long unsigned int i=0; i<macExist.size(); i++)
                mmac[i]=std::tolower(macExist[i]);

            existing.emplace_back(mmac, passExist);
        }
    }

        // normalizare MAC -> lowercase

        std::string macc(mac);

        for(long unsigned int i=0; i<mac.size(); i++)
            macc[i]=std::tolower(mac[i]);

        if (!isValidMac(macc)) 
            {
            std::cerr << "MAC invalid ignorat: " << mac << "\n";
             return;
            }

        if(!macExists(existing, macc))
        {
  

        XMLElement* item = doc.NewElement("item");
        item->SetAttribute("mac", macc.c_str());
        item->SetAttribute("password", password.c_str());
        root->InsertEndChild(item);

        // Tinem minte ca sa evitam duplicatele în acelasi apel
        existing.emplace_back(macc, password);
    

    if (doc.SaveFile(filePath.c_str()) != XML_SUCCESS) {
        throw std::runtime_error("Eroare la salvarea XML");
    }
    }
}

