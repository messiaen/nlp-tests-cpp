#include <iostream>
#include <fstream>

#include <nlohmann/json.hpp>
#include "blingfire.h"

std::vector<std::string> SplitTokens(const std::string &tokens, const char delim) {
    std::vector<std::string> o_toks;
    size_t prev = 0;
    size_t c = 0;
    const char *toks = tokens.c_str();
    while (c < tokens.size()) {
        for (;c < tokens.size() && toks[c] != delim; ++c);
        o_toks.push_back(tokens.substr(prev, c - prev));
        prev = c++;
    }
    return o_toks;
}

int main(int argc, char **argv) {
    std::fstream ifs("/home/eigenmoose/code/nlp/data/wiki_zh/AA/wiki_00");
    if (!ifs.is_open()) {
        std::cerr << "Failed to open json lines file" << std::endl;
        return 1;
    }
    std::string json_s;
    std::getline(ifs, json_s);
    if (ifs.bad()) {
        std::cerr << "Failed to read json lines file" << std::endl;
        return 1;
    }
    ifs.close();

    auto json_data = nlohmann::json::parse(json_s);
    //std::string text = json_data.at("text").get<std::string>();
    std::string text = "This is the Bling-Fire tokenizer. 2007年9月日历表_2007年9月农历阳历一览表-万年历";

    std::cout << BlingFire::GetBlingFireTokVersion() << std::endl;
    //std::cout << text.substr(0, 20) << std::endl;

    void *model = BlingFire::LoadModel("/home/eigenmoose/code/BlingFire/ldbsrc/ldb/laser100k.bin");
    if (model == NULL) {
        std::cerr << "Failed to load model" << std::endl;
        return 1;
    }
    std::string o_text;
    o_text.resize(2 * text.size());
    std::vector<int> o_start_offs(o_text.size());
    std::vector<int> o_end_offs(o_text.size());
    int o_len = BlingFire::TextToWordsWithOffsetsWithModel(
        text.c_str(),
         text.size(),
         o_text.data(),
         o_start_offs.data(),
         o_end_offs.data(),
         o_text.size(),
         model);
    
    if (false) {
        std::cerr << "Tokenizer Failed" << std::endl;
        return 1;
    } else {
        o_text.resize(o_len);
        std::cout << "Tokenizer returned o_text of length " << o_len << std::endl;
        std::cout << "Tokens:" << std::endl;
        for (const std::string &t : SplitTokens(o_text, ' ')) {
            std::cout << "\t" << t << std::endl;
        }
        std::cout << o_text << std::endl;
    }

    BlingFire::FreeModel(model);
    return 0;
}