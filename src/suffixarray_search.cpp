#include <divsufsort.h>
#include <sstream>


#include <seqan3/std/filesystem>

#include <seqan3/alphabet/nucleotide/dna5.hpp>
#include <seqan3/argument_parser/all.hpp>
#include <seqan3/core/debug_stream.hpp>
#include <seqan3/io/sequence_file/all.hpp>
#include <seqan3/search/fm_index/fm_index.hpp>
#include <seqan3/search/search.hpp>


// 0 -> pattern < text
// 1 -> gleichheit
// 2 -> pattern > text
// lexikografischer Vergleich
uint32_t compare(const std::vector<seqan3::dna5>& text, const std::vector<seqan3::dna5>& query, uint32_t i)
{
    uint32_t j = 0;

    while(text[i] == query[j])
    {
        i++;
        j++;
        if(i >= text.size() && j >= query.size()) return 1;
        else if(j >= query.size()) return 1;
        else if(i >= text.size()) return 2;
    }
    if(query[j] < text[i]) return 0;
    else return 2;
}

void find(const std::vector<seqan3::dna5>& query, const std::vector<saint_t>& sa, const std::vector<seqan3::dna5>& text)
{
    // std::vector<uint32_t>& hits{};
    // hits.clear();

    if(query.empty() || text.empty()) return;
    // LP - SUCHE ------------------------------------------------------

    uint32_t l = 0;
    uint32_t r = text.size()-1;
    uint32_t m = 0;
    int32_t rp = 0;
    int32_t lp = 0;

    if(compare(text, query, sa[0]) <= 1)
    {
        lp = 0;
    }
    else if(compare(text, query, sa.back()) == 2)
    {
        lp = sa.size();
    }
    else
    {
        while((r-l) > 1)
        {
            m = std::ceil((l + r) * 0.5);

            if(compare(text, query, sa[m]) <= 1)
            {
                r = m;
            }
            else
            {
                l = m;
            }
        }
        lp = r;
    }

    l = 0;
    r = text.size()-1;
    //rp
    if(compare(text, query, sa[0]) < 1)
    {
        rp = -1;
    }
    else if(compare(text, query, sa.back()) >= 1)
    {
        rp = sa.size()-1;
    }
    else
    {
        while((r-l) > 1)
        {
            m = std::ceil((l + r) * 0.5);

            if(compare(text, query, sa[m]) >= 1)
            {
                l = m;
            }
            else
            {
                r = m;
            }
        }
        rp = l;
    }
    for(auto letter : query)
    {
        seqan3::debug_stream<<letter;
    }
    seqan3::debug_stream<<": ";
    for(int32_t i = rp; i >= lp; --i)
    {
        //hits.push_back(sa[i]);
        seqan3::debug_stream<<sa[i]<<" ";
    }
    seqan3::debug_stream<<"\n";
}

int main(int argc, char const* const* argv) {
    seqan3::argument_parser parser{"suffixarray_search", argc, argv, seqan3::update_notifications::off};

    parser.info.author = "SeqAn-Team";
    parser.info.version = "1.0.0";

    auto reference_file = std::filesystem::path{};
    parser.add_option(reference_file, '\0', "reference", "path to the reference file");

    auto query_file = std::filesystem::path{};
    parser.add_option(query_file, '\0', "query", "path to the query file");

    try {
         parser.parse();
    } catch (seqan3::argument_parser_error const& ext) {
        seqan3::debug_stream << "Parsing error. " << ext.what() << "\n";
        return EXIT_FAILURE;
    }

    // loading our files
    auto reference_stream = seqan3::sequence_file_input{reference_file};
    auto query_stream     = seqan3::sequence_file_input{query_file};

    // read reference into memory
    // Attention: we are concatenating all sequences into one big combined sequence
    //            this is done to simplify the implementation of suffix_arrays
    std::vector<seqan3::dna5> reference;
    for (auto& record : reference_stream) {
        auto r = record.sequence();
        reference.insert(reference.end(), r.begin(), r.end());
    }

    // read query into memory
    std::vector<std::vector<seqan3::dna5>> queries;
    for (auto& record : query_stream) {
        queries.push_back(record.sequence());
    }

    //!TODO here adjust the number of searches
    queries.resize(1000000); // will reduce the amount of searches

    // Array that should hold the future suffix array
    std::vector<saint_t> suffixarray;
    suffixarray.resize(reference.size()); // resizing the array, so it can hold the complete SA

    //!TODO !ImplementMe implement suffix array sort
    //Hint, if can use libdivsufsort (already integrated in this repo)
    //      https://github.com/y-256/libdivsufsort
    //      To make the `reference` compatible with libdivsufsort you can simply
    //      cast it by calling:
    //      `sauchar_t const* str = reinterpret_cast<sauchar_t const*>(reference.data());`

     sauchar_t const* str = reinterpret_cast<sauchar_t const*>(reference.data());
     saint_t x = divsufsort(str, suffixarray.data(), suffixarray.size());
    // seqan3::debug_stream<<x<<"\n";

    seqan3::debug_stream<<x<<"\n";



    for (auto& q : queries) {
        //!TODO !ImplementMe apply binary search and find q  in reference using binary search on `suffixarray`
        find(q,suffixarray,reference);

    }

    return 0;
}
