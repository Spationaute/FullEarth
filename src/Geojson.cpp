//
// Created by gabriel on 14/07/19.
//

#include "Geojson.h"
using json = nlohmann::json;

Geojson::Geojson(const char* fileTarget) {
    this->readFile(fileTarget);
}

Geojson::~Geojson() {
    cleanData();
}

Poly Geojson::getPolygons() {
    return this->polygons;
}

bool Geojson::parse() {
    if(buffer.size()<1){
        std::cout << "Missing Ressource File, Parsing Failed." << std::endl;
        return false;
    }
    if(buffer[0]!='{' || (*(--buffer.end()))!='}'){
        std::cout << "JSON Invalid... Parsing Failed." << std::endl;
        return false;
    }
    //Clear old pointer
    cleanData();
    //Load the json
    parser = json::parse(buffer);

    //Parse data
    std::cout << "\tLoading Features..." << std::endl;
    std::vector<json> features = parser.find("features").value().get<std::vector<json>>();
    for(auto ii=features.begin();ii<features.end();++ii) {
        Owner* tempOwner = new Owner;
        json element = (*ii);
        tempOwner->properties = new json(element.find("properties").value());
        json geom = element.find("geometry").value();
        std::string coordType = geom.find("type").value().get<std::string>();

        if(coordType=="LineString"){
            json coord = geom.find("coordinates").value();
            std::vector<std::vector<float>> points = coord.get<std::vector<std::vector<float>>>();
            long int n = points.size();
            for (long int ii = 0; ii < n - 1; ++ii) {
                std::vector<float> v = points.at(ii);
                std::vector<float> b = points.at(ii+1);
                lineList.push_back(v);
                tempOwner->lineIds.push_back(lineList.size());
                lineList.push_back(b);
                tempOwner->lineIds.push_back(lineList.size());
            }
        }else if(coordType=="MultiLineString"){
            json coord = geom.find("coordinates").value();
            std::vector<Poly> points = coord.get<std::vector<Poly>>();

            for(auto jj=points.begin();jj!=points.end();++jj) {
                long int n = jj->size();
                for (long int ii = 0; ii < n - 1; ++ii) {
                    std::vector<float> v = jj->at(ii);
                    std::vector<float> b = jj->at(ii + 1);
                    lineList.push_back(v);
                    tempOwner->lineIds.push_back(lineList.size());
                    lineList.push_back(b);
                    tempOwner->lineIds.push_back(lineList.size());
                }
            }
        }else if(coordType=="Polygon"){
            json coord = geom.find("coordinates").value();
            std::vector<Poly> points = coord.get<std::vector<Poly>>();

            for(auto jj=points.begin();jj!=points.end();++jj) {
                long int n = jj->size();
                for (long int ii = 0; ii < n - 1; ++ii) {
                    std::vector<float> v = jj->at(ii);
                    std::vector<float> b = jj->at(ii + 1);
                    polygons.push_back(v);
                    tempOwner->polyIds.push_back(lineList.size());
                    polygons.push_back(b);
                    tempOwner->polyIds.push_back(lineList.size());
                }
            }
        }else if(coordType=="Point"){
            json coord = geom.find("coordinates").value();
            Pts points = coord.get<Pts>();
            pointList.push_back(points);
            tempOwner->ptsIds.push_back(pointList.size());
        }

        ownerList.push_back(tempOwner);
    }
    return true;
}

Lines Geojson::getLines() {
    return lineList;
}

std::vector<Pts> Geojson::getPoints() {
    return pointList;
}

bool Geojson::gatter(const char *url) {
    if(strlen(url)<3){
        std::cout << "No Source Selected or Source Invalid." << std::endl;
        return false;
    }
    this->fileTarget = url;
    this->setBuffer("");
    std::cout << "Loading from \"" << this->fileTarget << "\"" <<std::endl << "\t";
    CURLcode res;
    CURL* curl = curl_easy_init();
    curl_easy_setopt(curl,CURLOPT_URL,url);
    curl_easy_setopt(curl,CURLOPT_TIMEOUT_MS,10000);
    curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION,&Geojson::getData);
    curl_easy_setopt(curl,CURLOPT_WRITEDATA,this);
    res = curl_easy_perform(curl);
    if(res == CURLE_OK){
        std::cout << std::endl << "\tsuccess!" << std::endl;
        curl_easy_cleanup(curl);
        return true;
    }
    std::cout << std::endl;
    std::cout << "Loading Failed: " ;
    std::cout << curl_easy_strerror(res);
    std::cout << std::endl;
    curl_easy_cleanup(curl);
    return false;
}

size_t Geojson::getData(void *buffer, size_t size, size_t nmemb, void *user) {
    static unsigned short step = 0;
    auto* itself = (Geojson*) user;
    char* cinput = new char[nmemb+1];
    memset(cinput,0x00,nmemb+1);
    memccpy(cinput,buffer,size,nmemb);
    std::string inString(cinput);
    itself->appendBuffer(inString);
    std::cout << std::right << std::setw(6) << inString.size() << "/" << std::left << std::setw(6) << nmemb ;
    std::cout << (step%5==0? "\n\t" :"\t");
    delete[] cinput;
    step+=1;
    if(step==UINT8_MAX){
        step=0;
    }
    return inString.size();
}

void Geojson::setBuffer(std::string toSet) {
    this->buffer = toSet;
}

bool Geojson::readFile(const char *fileTarget) {
    std::cout << "Loading \""<<fileTarget<<"\""<<std::endl;
    this->fileTarget = fileTarget;
    std::ifstream file(this->fileTarget);
    if(!file.is_open()){
        std::cout << "Cannot Open \""<< fileTarget << "\"." << std::endl;
        return false;
    }
    std::string content((std::istreambuf_iterator<char>(file)),std::istreambuf_iterator<char>());
    buffer = content;
    file.close();
    return true;
}

Geojson::Geojson() {

}

void Geojson::appendBuffer(std::string toAdd) {
    this->buffer.append(toAdd);
}

std::vector<Owner*> Geojson::getOwners() {
    return this->ownerList;
}

void Geojson::cleanData() {
    //Clear Previous Pointers
    for(auto ii=ownerList.begin();ii!=ownerList.end();++ii){
        Owner* temp = (*ii);
        delete temp->properties;
        delete temp;
    }
    ownerList.clear();
}
